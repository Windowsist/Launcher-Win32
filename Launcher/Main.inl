
// int APIENTRY wWinMain
// (
// 	_In_ HINSTANCE /* hInstance */,
// 	_In_opt_ HINSTANCE /* hPrevInstance */,
// 	_In_ LPWSTR /* lpCmdLine */,
// 	_In_ int /* nCmdShow */
// )


UINT Main()
{ // stack: LPWSTR lpAppPath, lpWorkingDirectory, lpCommandLine;
	LPWSTR lpAppPath = nullptr, lpWorkingDirectory = nullptr, lpCommandLine = nullptr;
	__try // ensure lpAppPath, lpWorkingDirectory, lpCommandLine be freed
	{
		{ // stack: wchar_t file[260];
			wchar_t file[260];
			while (!GetModuleFileNameW(nullptr, file, 260))
			{
				RaiseException(GetLastError(), 0, 0, nullptr);
			}
			{ // stack: size_t len;
				int len = lstrlenW(file);
				lstrcpyW(file + len - 3, L"ini");
				{ // stack: HANDLE hFile;
					HANDLE hFile;
					while ((hFile = CreateFileW(file, 0, 0, nullptr, OPEN_EXISTING, 0, nullptr)) == INVALID_HANDLE_VALUE)
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
					while (!CloseHandle(hFile))
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
				} // clean: HANDLE hFile;
				{ // stack: wchar_t launcherdir[260];
					wchar_t launcherdir[260];
					while (len && file[len] != L'\\')
					{
						len--;
					}
					lstrcpynW(launcherdir, file, len + 1);
					while (!SetEnvironmentVariableW(L"LauncherDir", launcherdir))
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
				} // clean: wchar_t launcherdir[260];
			} // clean: size_t len;
			{ // stack: DWORD length, LPWSTR lpBuffer;
				DWORD length = 8;
				// LPWSTR lpBuffer = (LPWSTR)Malloc((length = 8) * sizeof(wchar_t));
				LPWSTR lpBuffer = (LPWSTR)Malloc(length * sizeof(wchar_t));
				__try // ensure lpBuffer be freed
				{ // stack: DWORD lastError;
					DWORD lastError;
					while (true)
					{
						GetPrivateProfileStringW(L"LaunchApp", L"AppPath", nullptr, lpBuffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //lpBuffer too small
						{
							lpBuffer = (LPWSTR)ReAlloc(lpBuffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							lpAppPath = ExpEnvW(lpBuffer);
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
						GetPrivateProfileStringW(L"LaunchApp", L"WorkingDirectory", nullptr, lpBuffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //lpBuffer too small
						{
							lpBuffer = (LPWSTR)ReAlloc(lpBuffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							lpWorkingDirectory = ExpEnvW(lpBuffer);
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
						GetPrivateProfileStringW(L"LaunchApp", L"CommandLine", nullptr, lpBuffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //lpBuffer too small
						{
							lpBuffer = (LPWSTR)ReAlloc(lpBuffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							lpCommandLine = ExpEnvW(lpBuffer);
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
						GetPrivateProfileSectionW(L"EnvironmentVariables", lpBuffer, length, file);
						lastError = GetLastError();
						if (lastError == ERROR_MORE_DATA) //lpBuffer too small
						{
							lpBuffer = (LPWSTR)ReAlloc(lpBuffer, (length *= 2) * sizeof(wchar_t));
						}
						else if (!lastError) //success
						{
							for (LPWSTR lpEnv = lpBuffer; lstrlenW(lpEnv); lpEnv += lstrlenW(lpEnv) + 1)
							{
								LPWSTR lpEnvI = ExpEnvW(lpEnv);
								__try // ensure lpEnvI be freed
								{
									PutEnvW(lpEnvI);
								}
								__finally // ensure lpEnvI be freed
								{
									Free(lpEnvI);
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
				__finally // ensure lpBuffer be freed
				{
					Free(lpBuffer);
				}
			} // clean: DWORD length, lastError; LPWSTR lpBuffer;
		}// clean: wchar_t file[260];
		while (!SetEnvironmentVariableW(L"LauncherDir", nullptr))
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
		{ // stack: PROCESS_INFORMATION pi; BOOL success;
			PROCESS_INFORMATION pi;
			BOOL success;
			do
			{
				{ // stack: STARTUPINFOW si
					STARTUPINFOW si = { sizeof(STARTUPINFOW) };
					success = CreateProcessW(lpAppPath, lpCommandLine, nullptr, nullptr, FALSE, 0, nullptr, lpWorkingDirectory, &si, &pi);
				} // clean: STARTUPINFOW si
				if (success)
				{
					while (!CloseHandle(pi.hProcess))
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
					while (!CloseHandle(pi.hThread))
					{
						RaiseException(GetLastError(), 0, 0, nullptr);
					}
				}
				else
				{
					RaiseException(GetLastError(), 0, 0, nullptr);
				}
			} while (!success);
		} // clean: PROCESS_INFORMATION pi; BOOL success;
	}
	__finally // ensure lpAppPath, lpWorkingDirectory, lpCommandLine be freed
	{
		if (lpAppPath)
		{
			Free(lpAppPath);
		}
		if (lpWorkingDirectory)
		{
			Free(lpWorkingDirectory);
		}
		if (lpCommandLine)
		{
			Free(lpCommandLine);
		}
	}
	return 0;
} // clean: LPWSTR lpAppPath, lpWorkingDirectory, lpCommandLine;
