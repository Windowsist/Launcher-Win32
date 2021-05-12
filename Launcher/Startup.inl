

int filter(DWORD exceptionCode)
{
	LPWSTR lpBuffer;
	if (!FormatMessageW
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		exceptionCode,
		0,
		(LPWSTR)&lpBuffer,
		0,
		nullptr
	))
	{
		DWORD err = GetLastError();
		if (err == ERROR_MUI_FILE_NOT_FOUND) // not system error, unexpected
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
				return EXCEPTION_CONTINUE_SEARCH; // unexpected and unable to display
			}
		}
		else
		{
			return EXCEPTION_CONTINUE_SEARCH; // unexpected and unable to display
		}
	}
	int result = EXCEPTION_CONTINUE_SEARCH;
	switch (MessageBoxW(nullptr, lpBuffer, nullptr, MB_RETRYCANCEL | MB_ICONERROR))
	{
	case IDRETRY:
		result = EXCEPTION_CONTINUE_EXECUTION;
		break;
	case IDCANCEL:
		result = EXCEPTION_EXECUTE_HANDLER;
		break;
	}
	if (LocalFree(lpBuffer))
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return result;
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
	__except (filter(GetExceptionCode())) //display error information
	{
		return ERROR_CANCELLED;
	}
}

extern "C" DWORD Startup(LPVOID)
{
	ExitProcess(__Startup());
}