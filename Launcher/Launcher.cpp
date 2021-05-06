
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <stdlib.h>

#include "Launcher.h"

int APIENTRY wWinMain
(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	wchar_t file[260];
	wchar_t launcherdir[260];
	if (!GetModuleFileNameW(NULL, file, 260))
	{
		ErrorMsg(GetLastError());
		return 0;
	}
	{
		size_t len = wcslen(file);
		wcscpy(file + len - 3, L"ini");
		{
			size_t i = len;
			while (i && file[i] != L'\\')
			{
				i--;
			}
			wcsncpy(launcherdir, file, i);
			launcherdir[i] = L'\0';
		}
	}
	if (!SetEnvironmentVariableW(L"LauncherDir", launcherdir))
	{
		ErrorMsg(GetLastError());
		return 0;
	}
	DWORD length = 8, lastError = 0;;
	LPWSTR buffer = (LPWSTR)calloc(16, sizeof(wchar_t));
	LPWSTR AppPath;
	while (true)
	{
		GetPrivateProfileStringW(L"LaunchApp", L"AppPath", NULL, buffer, length, file);
		lastError = GetLastError();
		if (lastError == ERROR_MORE_DATA) //buffer too small
		{
			buffer = (LPWSTR)_recalloc(buffer, (length *= 2), sizeof(wchar_t));
		}
		else if (!lastError) //success
		{
			AppPath = expenv(buffer);
			break;
		}
		else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
		{
			AppPath = nullptr;
			break;
		}
		else
		{
			ErrorMsg(lastError);
			return 0;
		}
	}
	LPWSTR WorkingDirectory;
	while (true)
	{
		GetPrivateProfileStringW(L"LaunchApp", L"WorkingDirectory", NULL, buffer, length, file);
		lastError = GetLastError();
		if (lastError == ERROR_MORE_DATA) //buffer too small
		{
			buffer = (LPWSTR)_recalloc(buffer, (length *= 2), sizeof(wchar_t));
		}
		else if (!lastError) //success
		{
			WorkingDirectory = expenv(buffer);
			break;
		}
		else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
		{
			WorkingDirectory = nullptr;
			break;
		}
		else
		{
			ErrorMsg(lastError);
			return 0;
		}
	}
	LPWSTR CommandLine;
	while (true)
	{
		GetPrivateProfileStringW(L"LaunchApp", L"CommandLine", NULL, buffer, length, file);
		lastError = GetLastError();
		if (lastError == ERROR_MORE_DATA) //buffer too small
		{
			buffer = (LPWSTR)_recalloc(buffer, (length *= 2), sizeof(wchar_t));
		}
		else if (!lastError) //success
		{
			CommandLine = expenv(buffer);
			break;
		}
		else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
		{
			CommandLine = nullptr;
			break;
		}
		else
		{
			ErrorMsg(lastError);
			return 0;
		}
	}
	while (true)
	{
		GetPrivateProfileSectionW(L"EnvironmentVariables", buffer, length, file);
		lastError = GetLastError();
		if (lastError == ERROR_MORE_DATA) //buffer too small
		{
			buffer = (LPWSTR)_recalloc(buffer, (length *= 2), sizeof(wchar_t));
		}
		else if (!lastError) //success
		{
			for (LPWSTR env = buffer; wcslen(env); env += wcslen(env) + 1)
			{
				LPWSTR enve = expenv(env);
				_wputenv(enve);
				free(enve);
			}
			break;
		}
		else if (lastError == ERROR_FILE_NOT_FOUND) //not defined
		{
			break;
		}
		else
		{
			ErrorMsg(lastError);
			return 0;
		}

	}
	free(buffer);
	if (!SetEnvironmentVariableW(L"LauncherDir", NULL))
	{
		ErrorMsg(GetLastError());
		return 0;
	}
	STARTUPINFOW si = { sizeof(STARTUPINFOW) };
	PROCESS_INFORMATION pi;
	BOOL success = CreateProcessW(AppPath, CommandLine, NULL, NULL, FALSE, 0, NULL, WorkingDirectory, &si, &pi);
	if (!success)
	{
		ErrorMsg(GetLastError());
	}
	else
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	if (AppPath)free(AppPath);
	if (WorkingDirectory)free(WorkingDirectory);
	if (CommandLine)free(CommandLine);
	return 0;
}

void ErrorMsg(DWORD ErrorId)
{
	LPWSTR lpBuffer;
	FormatMessageW
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		ErrorId,
		0,
		(LPWSTR)&lpBuffer,
		0,
		nullptr);
	MessageBoxW(nullptr, lpBuffer, nullptr, MB_ICONERROR);
	LocalFree(lpBuffer);
}

LPWSTR expenv(LPWSTR raw)
{
	DWORD len = ExpandEnvironmentStringsW(raw, NULL, 0);
	LPWSTR value = (LPWSTR)calloc(len, sizeof(wchar_t));
	ExpandEnvironmentStringsW(raw, value, len);
	return value;
}