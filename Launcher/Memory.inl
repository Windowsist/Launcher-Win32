
HANDLE hProcessHeap;

inline LPVOID Malloc(SIZE_T dwBytes)
{
	LPVOID result = nullptr;
	while (!result)
	{
		result = HeapAlloc(hProcessHeap, HEAP_GENERATE_EXCEPTIONS, dwBytes);
	}
	return result;
}

inline LPVOID ReAlloc(LPVOID lpMem, SIZE_T dwBytes)
{
	LPVOID result = nullptr;
	while (!result)
	{
		result = HeapReAlloc(hProcessHeap, HEAP_GENERATE_EXCEPTIONS, lpMem, dwBytes);
	}
	return result;
}

__declspec(noinline) VOID Free(LPVOID lpMem)
{
	while (!HeapFree(hProcessHeap, 0, lpMem))
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
}