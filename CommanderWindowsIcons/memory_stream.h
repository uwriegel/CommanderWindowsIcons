#pragma once
#include <vector>
#include "Objidl.h"
using namespace std;

class Memory_stream : public IStream
{
public:
	virtual ~Memory_stream() {}

	virtual HRESULT __stdcall QueryInterface(REFIID iid, void **object);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	virtual HRESULT __stdcall Read(void *pv, ULONG cb, ULONG *pcbRead);
	virtual HRESULT __stdcall Write(const void *pv, ULONG cb, ULONG *pcbWritten);

	virtual HRESULT __stdcall Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);
	virtual HRESULT __stdcall SetSize(ULARGE_INTEGER libNewSize);

	virtual HRESULT __stdcall CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);
	virtual HRESULT __stdcall Commit(DWORD grfCommitFlags);
	virtual HRESULT __stdcall Revert();
	virtual HRESULT __stdcall LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	virtual HRESULT __stdcall UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
	virtual HRESULT __stdcall Stat(STATSTG *pstatstg, DWORD grfStatFlag);
	virtual HRESULT __stdcall Clone(IStream **ppstm);

	const vector<char> get_bytes();
private:
	vector<char> bytes;
};
