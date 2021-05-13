
HANDLE hProcessHeap;

inline LPVOID Malloc(SIZE_T dwBytes)
{
	LPVOID result;
	do
	{
		result = HeapAlloc(hProcessHeap, HEAP_GENERATE_EXCEPTIONS, dwBytes);
	} while (!result);
	return result;
}

inline LPVOID ReAlloc(LPVOID lpMem, SIZE_T dwBytes)
{
	LPVOID result;
	do
	{
		result = HeapReAlloc(hProcessHeap, HEAP_GENERATE_EXCEPTIONS, lpMem, dwBytes);
	} while (!result);
	return result;
}

__declspec(noinline) VOID Free(LPVOID lpMem)
{
	while (!HeapFree(hProcessHeap, 0, lpMem))
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
}