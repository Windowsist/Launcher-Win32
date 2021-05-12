
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

inline VOID PutEnvW(LPWSTR lpEnv)
{
	BOOL result = NULL;
	while (!result)
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
			result = SetEnvironmentVariableW(lpEnv, nullptr); // delete EnvironmentVariable
		}
		else
		{
			lpEnv[i] = L'\0';
			result = SetEnvironmentVariableW(lpEnv, lpEnv + i + 1);
			lpEnv[i] = L'=';
		}
		if (!result)
		{
			RaiseException(GetLastError(), 0, 0, nullptr);
		}
	}
}