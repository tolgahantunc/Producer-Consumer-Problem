#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <SDKDDKVer.h>

//number of process that will be created
#define NO_OF_PROCESS 5

int main(int argc, char* argv[])
{
	STARTUPINFO si[NO_OF_PROCESS];
	PROCESS_INFORMATION pi[NO_OF_PROCESS];
	HANDLE processHandles[NO_OF_PROCESS], writePipe[NO_OF_PROCESS], readPipe[NO_OF_PROCESS];
	SECURITY_ATTRIBUTES sa[NO_OF_PROCESS];

	//create handle mapping object and size for shared memory
	int		shmem_size = NO_OF_PROCESS;
	HANDLE	shmem = INVALID_HANDLE_VALUE;

	//create tasks and give allocation to them
	char* producer;
	char* consumer;
	char* swapper;
	producer = (char *)malloc(20);
	consumer = (char *)malloc(20);
	swapper = (char *)malloc(20);

	int i = 0;	//the main purpose of this variable is referring the index number of pipes and child processes
	int x = 0;	//this variable will be use while deciding the tasks randomly
	int bytesToWrite = 0;
	int bytesWritten = 0;

	shmem = CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		shmem_size,
		"shared_memory"
		);

	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		//creating security attributes
		SecureZeroMemory(&sa[i], sizeof(SECURITY_ATTRIBUTES));
		sa[i].bInheritHandle = TRUE;
		sa[i].lpSecurityDescriptor = NULL;
		sa[i].nLength = sizeof(SECURITY_ATTRIBUTES);

		/*	creating the pipe
		buffer size will be equal to the number of processes via the parameter NO_OF_PROCESS
		*/
		if (!CreatePipe(&readPipe[i], &writePipe[i], &sa[i], NO_OF_PROCESS))
		{
			printf("unable to create pipe\n");
			system("pause");
			exit(0);
		}

		//creating variables for child process
		SecureZeroMemory(&si[i], sizeof(STARTUPINFO));
		SecureZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));

		si[i].cb = sizeof(STARTUPINFO);
		si[i].hStdInput = readPipe[i];
		si[i].hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		si[i].hStdError = GetStdHandle(STD_ERROR_HANDLE);
		si[i].dwFlags = STARTF_USESTDHANDLES;

		//create child process
		if (!CreateProcess(NULL, "child.exe", NULL, NULL, TRUE, 0/*CREATE_NEW_CONSOLE*/, NULL, NULL, &si[i], &pi[i]))
		{
			printf("cannot create child process\n");
			system("pause");
			exit(0);
		}
		else
		{
			processHandles[i] = pi[i].hProcess;
			printf("parent has created process number %d\n", i);
		}

	}

	//send tasks through pipe
	while (1)
	{
		//to see well the whole operations, while loop occurs 1 times in 1 second
		Sleep(750);

		/*	parent process decides which operation will be done, using "x = rand() % (2 + 1 - 0) + 0" command,
		and x = 0 refers producer, x = 1 refers consumer, x = 2 refers swapper.
		*/
		switch (x = rand() % (2 + 1 - 0) + 0) {
		case 0:
			/*	check errors
			WriteFile function writes data to the specified file or input / output(I / O) device.
			writePipe[rand() % (NO_OF_PROCESS + 1 - 0) + 0] in this line, the index number of the pipe, that will use
			for operations, is estimating randomly using "rand() % (NO_OF_PROCESS - 1 + 1 - 0" this command.
			*/
			producer = "producer";
			bytesToWrite = strlen(producer);
			bytesToWrite++;

			if (!WriteFile(writePipe[rand() % (NO_OF_PROCESS - 1 + 1 - 0) + 0], producer, bytesToWrite, &bytesWritten, NULL))
			{
				printf("unable to write to pipe\n");
				system("pause");
				exit(0);
			}

			break;

		case 1:
			consumer = "consumer";
			bytesToWrite = strlen(consumer);
			bytesToWrite++;

			if (!WriteFile(writePipe[rand() % (NO_OF_PROCESS - 1 + 1 - 0) + 0], consumer, bytesToWrite, &bytesWritten, NULL))
			{
				printf("unable to write to pipe\n");
				system("pause");
				exit(0);
			}
			break;

		case 2:
			swapper = "swapper";
			bytesToWrite = strlen(swapper);
			bytesToWrite++;

			if (!WriteFile(writePipe[rand() % (NO_OF_PROCESS - 1 + 1 - 0) + 0], swapper, bytesToWrite, &bytesWritten, NULL))
			{
				printf("unable to write to pipe\n");
				system("pause");
				exit(0);
			}
			break;

		default:
			//default operation of pipe is defined as producer
			producer = "producer";
			bytesToWrite = strlen(producer);
			bytesToWrite++;

			if (!WriteFile(writePipe[rand() % (NO_OF_PROCESS - 1 + 1 - 0) + 0], producer, bytesToWrite, &bytesWritten, NULL))
			{
				printf("unable to write to pipe\n");
				system("pause");
				exit(0);
			}
		}
	}

	//release of all handles and allocated areas
	for (i = 0; i < NO_OF_PROCESS; i++)
	{
		free(&si[i]);
		free(&pi[i]);
		free(&sa[i]);
		CloseHandle(readPipe[i]);
		CloseHandle(writePipe[i]);
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
	}

	CloseHandle(shmem);
	free(producer);
	free(consumer);
	free(swapper);

	system("pause");
	return 1;
}