#include <Windows.h>
#include <stdio.h>

static long long *get_time (char **str, long long *sec) {
	
	double time = 0;
	char suffix = 'm';
	sscanf (argv[1], "%f%c",  &time, &suffix);
	
	switch (suffix) {
		case 's': sec = time; break;
		case 'm': sec = time * 60; break;
		case 'h': sec = time * 3600; break;
		case 'd': sec = time * 86400; break;
		default:
			puts ("Suffix error.");
			return NULL;
	}
	return sec;
}

static HANDLE *set_waitable_timer (long long *sec, HANDLE *hTimer) {
	
	SYSTEMTIME stLoc, stUtc;
	FILETIME ftUtc;

	GetLocalTime (&stLoc);
	if (!TzSpecificLocalTimeToSystemTime (NULL, &stLoc, &stUtc)) {
		puts ("Fail to convert local time to Utc time.");
		return NULL;
	}
	if (!SystemTimeToFileTime (&stUtc, &ftUtc)) {
		puts ("Fail to convert Utc time to file time.");
		return NULL;
	}
	
    LARGE_INTEGER dueTime = {
		.u.LowPart = ftUtc.dwLowDateTime,
		.u.HighPart = ftUtc.dwHighDateTime
	};
    dueTime.QuadPart += (*sec * 10000000LL);
	
	if ((*hTimer = CreateWaitableTimerW (NULL, TRUE, NULL)) == NULL) {
		puts ("Fail to create waitable timer.");
		return NULL;
	}
	
	if (GetLastError () == ERROR_NOT_SUPPORTED) {
		puts ("Waitable timers are not supported by hardware.");
		return NULL;
	}
	
	if (!SetWaitableTimer (*hTimer, &dueTime, 0, NULL, NULL, TRUE)) {
		puts ("Fail to set waitable timer.");
		return NULL;
	}
	
	return *hTimer;
}

int main (int argc, char **argv) {
	
	int argind = 1,
		timer = 0,
		monitor = 1;
		
	if (argc <= argind)
		return -1;
	
	long long sec = 0;
	if (get_time (argv[argind], &sec) == NULL)
		return -1;
	
	
	HANDLE *handle[2];
	if (get_waitable_timer (&sec, handle[timer]) == NULL)
		return -1;
	
	if (get_directory_monitor (handle[monitor]) == NULL)
		return -1;
	
	
	switch (WaitForMultipleObjects (2, handle, FALSE, INFINITE)) {
		
		case handle[timer]:
		
		case handle[monitor]:
			FindNextChange();
		case WAIT_FAILED:
			return -1;
	}
	CloseHandle (hTimer);
	CloseHandle (hfind);
	
	
	execute_command(argv + argind + 1);
	return 0;
}