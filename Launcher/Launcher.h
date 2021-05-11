#pragma once

HANDLE hHeap;

LPWSTR Expenv(LPCWSTR lpSrc);

BOOL PutenvW(LPWSTR lpEnv);

#define Malloc(dwBytes)HeapAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS, dwBytes)

#define ReAlloc(lpMem, dwBytes) HeapReAlloc(hHeap, HEAP_GENERATE_EXCEPTIONS, lpMem, dwBytes)

#define Free(lpMem) if (!HeapFree(hHeap, 0, lpMem)) { RaiseException(GetLastError(), 0, 0, nullptr); }
