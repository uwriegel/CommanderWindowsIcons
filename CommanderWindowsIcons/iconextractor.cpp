#include <Windows.h>
#include <Gdiplus.h>
#include <string>
#include <vector>
#include <memory>
#include "iconextractor.h"
#include "memory_stream.h"	
using namespace std;
using namespace Gdiplus;

ULONG_PTR gdiplus_token{ 0 };
CLSID png_clsid{ 0 };

struct BITMAP_AND_PIXELS {
	BITMAP_AND_PIXELS(int number_of_bits)
		: pixels(number_of_bits) {}
	unique_ptr<Bitmap> bmp;
	vector<int> pixels;
};

CLSID get_encoder_clsid(const wstring& format);
BITMAP_AND_PIXELS create_alpha_channel_bitmap_from_icon(HICON icon);

void initialize() {
	GdiplusStartupInput gdiplus_startup_input;
	gdiplus_token;
	auto status = GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, nullptr);

	if (png_clsid.Data1 == 0)
		png_clsid = get_encoder_clsid(L"image/png"s);
}

void uninitialize() {
	GdiplusShutdown(gdiplus_token);
}

const vector<char> extract_icon(const string& icon_path) {
	SHFILEINFOA file_info{ 0 };
	SHGetFileInfoA(icon_path.c_str(), FILE_ATTRIBUTE_NORMAL, &file_info, sizeof(file_info),
		SHGFI_ICON | SHGFI_SMALLICON | SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME);
	auto icon = file_info.hIcon;
	auto result = create_alpha_channel_bitmap_from_icon(icon);
	Memory_stream ms;
	result.bmp->Save(&ms, &png_clsid);
	auto bytes = ms.get_bytes();
	DestroyIcon(icon);
	return bytes;
}


CLSID get_encoder_clsid(const wstring& format) {
	UINT num{ 0 };
	UINT  size{ 0 };
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return { 0 };

	vector<char> image_codec_info_buffer(size);
	auto image_codec_info = reinterpret_cast<ImageCodecInfo*>(image_codec_info_buffer.data());
	GetImageEncoders(num, size, image_codec_info);
	for (auto i = 0u; i < num; ++i)
		if (format == image_codec_info[i].MimeType)
			return image_codec_info[i].Clsid;
	return { 0 };
}

BITMAP_AND_PIXELS create_alpha_channel_bitmap_from_icon(HICON icon) {
	ICONINFO icon_info{ 0 };
	GetIconInfo(icon, &icon_info);

	auto dc = GetDC(nullptr);

	BITMAP bm{ 0 };
	GetObject(icon_info.hbmColor, sizeof(bm), &bm);

	BITMAPINFO bmi{ 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = bm.bmWidth;
	bmi.bmiHeader.biHeight = -bm.bmHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	int number_of_bits = bm.bmWidth * bm.bmHeight;

	BITMAP_AND_PIXELS result(number_of_bits);
	GetDIBits(dc, icon_info.hbmColor, 0, bm.bmHeight, result.pixels.data(), &bmi, DIB_RGB_COLORS);

	// Check whether the color bitmap has an alpha channel.
	// (On my Windows 7, all file icons I tried have an alpha channel.)
	auto has_alpha{ false };
	for (int i = 0; i < number_of_bits; i++)
		if ((result.pixels[i] & 0xff000000) != 0) {
			has_alpha = TRUE;
			break;
		}

	// If no alpha values available, apply the mask bitmap
	if (!has_alpha) {
		// Extract the mask bitmap
		vector<int> mask_bits(number_of_bits);
		GetDIBits(dc, icon_info.hbmMask, 0, bm.bmHeight, mask_bits.data(), &bmi, DIB_RGB_COLORS);
		// Copy the mask alphas into the color bits
		for (int i = 0; i < number_of_bits; i++)
			if (mask_bits[i] == 0)
				result.pixels[i] |= 0xff000000;
	}

	ReleaseDC(nullptr, dc);
	DeleteObject(icon_info.hbmColor);
	DeleteObject(icon_info.hbmMask);

	// Create GDI+ Bitmap
	result.bmp = make_unique<Bitmap>(bm.bmWidth, bm.bmHeight, bm.bmWidth * 4, PixelFormat32bppARGB, reinterpret_cast<BYTE*>(result.pixels.data()));

	return result;
}
