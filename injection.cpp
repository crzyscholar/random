#include <windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* i = "[*]";
const char* e = "[-]";


unsigned long PID = NULL;
HANDLE hProcess, hThread = NULL;

int main(int argc, char* argv[]){
	if (argc < 2){
		printf("%s usage: program.exe <PID>", e);
		return 1;
	}

	PID = atoi(argv[1]);
	printf("%s trying to open a handle to process (%ld)\n", i, PID);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (hProcess == NULL){
		printf("%s couldn't  get a handle to the process (%ld), error: %ld", e, PID, GetLastError());
		return EXIT_FAILURE;
	}

	return 0;
}