#include <iterator>
#include "memory_stream.h"
using namespace std;

const vector<char> Memory_stream::get_bytes() {
	return bytes;
}

HRESULT __stdcall Memory_stream::QueryInterface(REFIID iid, void **object) {
	if (iid == IID_IUnknown) {
		*object = static_cast<IUnknown*>(this);
		return S_OK;
	}
	else if (iid == IID_IStream) {
		*object = static_cast<IStream*>(this);
		return S_OK;
	}
	else if (iid == IID_ISequentialStream) {
		*object = static_cast<ISequentialStream*>(this);
		return S_OK;
	}
	else {
		*object = nullptr;
		return E_NOINTERFACE;
	}
}

ULONG __stdcall Memory_stream::AddRef() {
	return 1;
}

ULONG __stdcall Memory_stream::Release() {
	return 0;
}

HRESULT __stdcall Memory_stream::Read(void *pv, ULONG cb, ULONG *pcbRead) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Write(const void *pv, ULONG cb, ULONG *pcbWritten) {
	auto bs = reinterpret_cast<const char*>(pv);
	copy(bs, bs + cb, back_inserter(bytes));
	*pcbWritten = cb;
	return S_OK;
}

HRESULT __stdcall Memory_stream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::SetSize(ULARGE_INTEGER libNewSize) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Commit(DWORD grfCommitFlags) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Revert() {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Stat(STATSTG *pstatstg, DWORD grfStatFlag) {
	return E_NOTIMPL;
}

HRESULT __stdcall Memory_stream::Clone(IStream **ppstm) {
	return E_NOTIMPL;
}


