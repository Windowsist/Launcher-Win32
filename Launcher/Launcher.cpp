
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
{
	wchar_t file[260], launcherdir[260];
	if (!GetModuleFileNameW(nullptr, file, 260))
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
	{
		size_t len = lstrlenW(file);
		lstrcpyW(file + len - 3, L"ini");
		{
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
		}
		while (len && file[len] != L'\\')
		{
			len--;
		}
		lstrcpynW(launcherdir, file, len + 1);
	}
	if (!SetEnvironmentVariableW(L"LauncherDir", launcherdir))
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
	LPWSTR AppPath, WorkingDirectory, CommandLine;
	{
		DWORD length, lastError;
		LPWSTR buffer = (LPWSTR)Malloc((length = 8) * sizeof(wchar_t));
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
				AppPath = Expenv(buffer);
				break;
			}
			else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
			{
				AppPath = nullptr;
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
				WorkingDirectory = Expenv(buffer);
				break;
			}
			else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
			{
				WorkingDirectory = nullptr;
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
				CommandLine = Expenv(buffer);
				break;
			}
			else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
			{
				CommandLine = nullptr;
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
					LPWSTR enve = Expenv(env);
					if (!PutenvW(enve))
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
					Free(enve)
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
		Free(buffer)
	}
	if (!SetEnvironmentVariableW(L"LauncherDir", nullptr))
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
	{
		PROCESS_INFORMATION pi;
		BOOL success;
		{
			STARTUPINFOW si = { sizeof(STARTUPINFOW) };
			success = CreateProcessW(AppPath, CommandLine, nullptr, nullptr, FALSE, 0, nullptr, WorkingDirectory, &si, &pi);
		}
		if (!success)
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
		else
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
	}
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
	return 0;
}

void ErrorMsg(DWORD ErrorId)
{
	LPWSTR lpBuffer;
	FormatMessageW
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		ErrorId,
		0,
		(LPWSTR)&lpBuffer,
		0,
		nullptr
	);
	if (!lpBuffer)
	{
		FormatMessageW
		(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			E_UNEXPECTED,
			0,
			(LPWSTR)&lpBuffer,
			0,
			nullptr
		);
	}
	if (!lpBuffer)
	{
		RaiseException(GetLastError(), 0, 0, nullptr);
	}
	else
	{
		MessageBoxW(nullptr, lpBuffer, nullptr, MB_ICONERROR);
		if (LocalFree(lpBuffer))
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
	}
}

LPWSTR Expenv(LPCWSTR lpSrc)
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

BOOL PutenvW(LPWSTR lpEnv)
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

__declspec(noinline) UINT __Startup()
{
	__try
	{
		hHeap = GetProcessHeap();
		if (!hHeap)
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
		return Main();
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		ErrorMsg(GetExceptionCode());
		return 0;
	}
}

extern "C" DWORD Startup(LPVOID)
{
	ExitProcess(__Startup());
	return 0UL;
}