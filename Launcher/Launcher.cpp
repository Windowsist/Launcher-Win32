
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
// #include <stdlib.h>

#include "Launcher.h"

// int APIENTRY wWinMain
// (
// 	_In_ HINSTANCE /* hInstance */,
// 	_In_opt_ HINSTANCE /* hPrevInstance */,
// 	_In_ LPWSTR /* lpCmdLine */,
// 	_In_ int /* nCmdShow */
// )
UINT Main()
{ // stack: LPWSTR AppPath, WorkingDirectory, CommandLine;
	LPWSTR AppPath = nullptr, WorkingDirectory = nullptr, CommandLine = nullptr;
	__try // ensure AppPath, WorkingDirectory, CommandLine be freed
	{
		{ // stack: wchar_t file[260];
			wchar_t file[260];
			if (!GetModuleFileNameW(nullptr, file, 260))
			{
				RaiseException(GetLastError(), 0, 0, nullptr);
			}
			{ // stack: size_t len;
				int len = lstrlenW(file);
				lstrcpyW(file + len - 3, L"ini");
				{ // stack: HANDLE hFile;
					HANDLE hFile = CreateFileW(file, 0, 0, nullptr, OPEN_EXISTING, 0, nullptr);
					if (hFile == INVALID_HANDLE_VALUE)
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
					else
					{
						if (!CloseHandle(hFile))
						{
							RaiseException(GetLastError(), 0, 0, nullptr);
						}
					}
				} // clean: HANDLE hFile;
				{ // stack: wchar_t launcherdir[260];
					wchar_t launcherdir[260];
					while (len && file[len] != L'\\')
					{
						len--;
					}
					lstrcpynW(launcherdir, file, len + 1);
					if (!SetEnvironmentVariableW(L"LauncherDir", launcherdir))
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
				} // clean: wchar_t launcherdir[260];
			} // clean: size_t len;
			{ // stack: DWORD length, LPWSTR buffer;
				DWORD length;
				LPWSTR buffer = (LPWSTR)Malloc((length = 8) * sizeof(wchar_t));
				__try // ensure buffer be freed
				{ // stack: DWORD lastError;
					DWORD lastError;
					while (true)
					{
						GetPrivateProfileStringW(L"LaunchApp", L"AppPath", nullptr, buffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //buffer too small
						{
							buffer = (LPWSTR)ReAlloc(buffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							AppPath = ExpEnvW(buffer);
							break;
						}
						else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
						{
							break;
						}
						else
						{
							RaiseException(lastError, 0, 0, nullptr);
						}
					}
					while (true)
					{
						GetPrivateProfileStringW(L"LaunchApp", L"WorkingDirectory", nullptr, buffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //buffer too small
						{
							buffer = (LPWSTR)ReAlloc(buffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							WorkingDirectory = ExpEnvW(buffer);
							break;
						}
						else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
						{
							break;
						}
						else
						{
							RaiseException(lastError, 0, 0, nullptr);
						}
					}
					while (true)
					{
						GetPrivateProfileStringW(L"LaunchApp", L"CommandLine", nullptr, buffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //buffer too small
						{
							buffer = (LPWSTR)ReAlloc(buffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							CommandLine = ExpEnvW(buffer);
							break;
						}
						else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
						{
							break;
						}
						else
						{
							RaiseException(lastError, 0, 0, nullptr);
						}
					}
					while (true)
					{
						GetPrivateProfileSectionW(L"EnvironmentVariables", buffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //buffer too small
						{
							buffer = (LPWSTR)ReAlloc(buffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							for (LPWSTR env = buffer; lstrlenW(env); env += lstrlenW(env) + 1)
							{
								LPWSTR enve = ExpEnvW(env);
								__try // ensure enve be freed
								{
									if (!PutEnvW(enve))
									{
										RaiseException(GetLastError(), 0, 0, nullptr);
									}
								}
								__finally // ensure enve be freed
								{
									Free(enve)
								}
							}
							break;
						}
						else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
						{
							break;
						}
						else
						{
							RaiseException(lastError, 0, 0, nullptr);
						}
					}
				} // clean: DWORD lastError;
				__finally // ensure buffer be freed
				{
					Free(buffer)
				}
			} // clean: DWORD length, lastError; LPWSTR buffer;
		}// clean: wchar_t file[260];
		if (!SetEnvironmentVariableW(L"LauncherDir", nullptr))
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
		{ // stack: PROCESS_INFORMATION pi; BOOL success;
			PROCESS_INFORMATION pi;
			BOOL success;
			{ // stack: STARTUPINFOW si
				STARTUPINFOW si = { sizeof(STARTUPINFOW) };
				success = CreateProcessW(AppPath, CommandLine, nullptr, nullptr, FALSE, 0, nullptr, WorkingDirectory, &si, &pi);
			} // clean: STARTUPINFOW si
			if (success)
			{
				if (!CloseHandle(pi.hProcess))
				{
					RaiseException(GetLastError(), 0, 0, nullptr);
				}
				if (!CloseHandle(pi.hThread))
				{
					RaiseException(GetLastError(), 0, 0, nullptr);
				}
			}
			else
			{
				RaiseException(GetLastError(), 0, 0, nullptr);
			}
		} // clean: PROCESS_INFORMATION pi; BOOL success;
	}
	__finally // ensure AppPath, WorkingDirectory, CommandLine be freed
	{
		if (AppPath)
		{
			Free(AppPath)
		}
		if (WorkingDirectory)
		{
			Free(WorkingDirectory)
		}
		if (CommandLine)
		{
			Free(CommandLine)
		}
	}
	return 0;
} // clean: LPWSTR AppPath, WorkingDirectory, CommandLine;

LPWSTR ExpEnvW(LPCWSTR lpSrc)
{
	DWORD len = ExpandEnvironmentStringsW(lpSrc, nullptr, 0);
	if (!len)
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
	LPWSTR value = (LPWSTR)Malloc(len * sizeof(wchar_t));
	if (!ExpandEnvironmentStringsW(lpSrc, value, len))
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
	return value;
}

inline BOOL PutEnvW(LPWSTR lpEnv)
{
	int i, len = lstrlenW(lpEnv);
	for (i = 0; i < len; i++)
	{
		if (lpEnv[i] == L'=')
		{
			break;
		}
	}
	if (i == len)
	{
		return SetEnvironmentVariableW(lpEnv, nullptr); // delete EnvironmentVariable
	}
	else
	{
		lpEnv[i] = L'\0';
		BOOL result = SetEnvironmentVariableW(lpEnv, lpEnv + i + 1);
		lpEnv[i] = L'=';
		return result;
	}
}

__declspec(noinline) UINT __Startup() // noinline: ensure stack will be cleaned before excuting ExitProcess
{
	__try
	{
		hProcessHeap = GetProcessHeap(); // initialize global
		if (!hProcessHeap)
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
		return Main(); // invoke main
	}
	__except (EXCEPTION_EXECUTE_HANDLER) //display error information
	{
		LPWSTR lpBuffer;
		if (!FormatMessageW
		(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			GetExceptionCode(),
			0,
			(LPWSTR)&lpBuffer,
			0,
			nullptr
		))
		{
			if (GetLastError() == ERROR_MUI_FILE_NOT_FOUND) // not system error, unexpected
			{
				if (!FormatMessageW
				(
					FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					nullptr,
					(DWORD)E_UNEXPECTED,
					0,
					(LPWSTR)&lpBuffer,
					0,
					nullptr
				))
				{
					return GetLastError(); // unexpected and unable to display
				}
			}
		}
		else
		{
			MessageBoxW(nullptr, lpBuffer, nullptr, MB_ICONERROR);
			if (LocalFree(lpBuffer))
			{
				return GetLastError();
			}
		}
		return 0;
	}
}

extern "C" DWORD Startup(LPVOID)
{
	ExitProcess(__Startup());
}