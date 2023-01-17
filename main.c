#include <Windows.h>
#include <synchapi.h>
#include <stdio.h>

#define _CRT_NO_SECURE_WARNINGS

#define TIMER_CANNOT_CREATE		0xFFFF0000L
#define TIMER_CANNOT_SET		0xFFFF0005L
#define TIMER_SUCCESS			0x0

DWORD CustomSleep(DWORD dwMilliseconds) {
	HANDLE hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
	if(hTimer == NULL) {
		wprintf(L"[!] Cannot create waitable timer\n");

		return TIMER_CANNOT_CREATE;
	}

	LARGE_INTEGER lTime;
	ZeroMemory(&lTime, sizeof(lTime));

	lTime.QuadPart = ((LONGLONG)dwMilliseconds) * -10000;

	if(!SetWaitableTimer(hTimer, &lTime, 0, NULL, NULL, FALSE)) {
		wprintf(L"[!] Cannot set timer to provided time. Received status: 0x%x\n", GetLastError());

		CloseHandle(hTimer);
		return TIMER_CANNOT_SET;
	}

	DWORD dwActive = WaitForSingleObject(hTimer, INFINITE);
	if(dwActive == WAIT_FAILED) {
		DWORD dwErr = GetLastError();
		wprintf(L"[!] Received error in WaitForSingleObject function, status: 0x%x\n", dwErr);

		CloseHandle(hTimer);
		return dwErr;
	}

	CloseHandle(hTimer);
	return TIMER_SUCCESS;
}

int main(void) {
	wprintf(L"[+] Sleep for 5 seconds..\n");

	DWORD dwStatus = CustomSleep(5000);
	if(dwStatus) {
		wprintf(L"[!] No, I will stay awake..\n");
		return 1;
	}

	wprintf(L"[+] The Sleeper..\n");

	return 0;
	
}