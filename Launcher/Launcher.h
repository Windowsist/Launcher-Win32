#pragma once

HANDLE hProcessHeap;

// ExpandEnvironmentString
LPWSTR ExpEnvW(LPCWSTR lpSrc);

// SetEnvironmentVariable
inline BOOL PutEnvW(LPWSTR lpEnv);

#define Malloc(dwBytes)HeapAlloc(hProcessHeap, HEAP_GENERATE_EXCEPTIONS, dwBytes)

#define ReAlloc(lpMem, dwBytes) HeapReAlloc(hProcessHeap, HEAP_GENERATE_EXCEPTIONS, lpMem, dwBytes)

#define Free(lpMem) if (!HeapFree(hProcessHeap, 0, lpMem)) { RaiseException(GetLastError(), 0, 0, nullptr); }
