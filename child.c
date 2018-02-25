#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <string.h>

//define the size of the buffer, this should be the same with the number of child process (NO_OF_PROCESS in parent.c)
#define BUF_SIZE 5

//this function makes swap operation and this function will call, if need.
void swap(void *vp1, void *vp2, const int size) {
	char *temp = (char *)malloc(sizeof(char)*size);
	memcpy(temp, vp1, size);
	memcpy(vp1, vp2, size);
	memcpy(vp2, temp, size);
	free(temp);
}

int main()
{
	//create a variable to keep which task tells to child process
	char* task;
	task = (char *)malloc(20);

	int i = 0;

	HANDLE	shmem = INVALID_HANDLE_VALUE;
	HANDLE	mutex = INVALID_HANDLE_VALUE;
	mutex = CreateMutex(NULL, FALSE, "mutex");

	//to get process id
	DWORD pid = GetCurrentProcessId();
	unsigned int processID;
	processID = pid;


	while (1)
	{
		for (i = 0; (task[i] = getchar()) != 0; i++)
		{
		}

		//check the message, which sent from parent process, and apply the given operation
		if (strstr(task, "producer") != NULL){
			shmem = OpenFileMapping(
				FILE_MAP_ALL_ACCESS,	// read/write access
				FALSE,					// do not inherit the name
				"shared_memory");	//name of mapping object

			char *buf = (char*)MapViewOfFile(shmem, FILE_MAP_WRITE, 0, 0, BUF_SIZE);

			//mutex lock
			WaitForSingleObject(mutex, 5000);

			//generate random buffer index and a value to write to buffer
			int x = rand() % (BUF_SIZE - 1 + 1 - 0) + 0;	//index number
			int y = rand() % (BUF_SIZE - 1 + 1 - 0) + 0;	//value to write

			/*	check if the x th index of buffer is empty, then write into that index
			if x th index of buffer is not empty, do not produce a value for that index
			*/
			if (buf[x] == 0){
				//write shared memory and written value is 1
				memset(&buf[x], y + 1, 1);
				printf("child %d produced %d as %d th entry in the buffer\n", processID, y + 1, x);

				//mutex unlock
				ReleaseMutex(mutex);
			}
			//release
			UnmapViewOfFile(buf);
			CloseHandle(shmem);
		}

		if (strstr(task, "consumer") != NULL){
			mutex = CreateMutex(NULL, FALSE, "mutex");

			shmem = CreateFileMapping(
				INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE,
				0,
				BUF_SIZE,
				"shared_memory"
				);

			char *buf = (char*)MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);

			//generate random buffer index
			int x = rand() % (BUF_SIZE - 1 + 1 - 0) + 0;

			/*	check if the x th index of buffer is not empty
			if tx th index is empty, this process cannot consume the value of x th index
			*/
			if (!buf[x] == 0){
				//mutex lock
				WaitForSingleObject(mutex, 5000);
				printf("child %d consumed %d from %d th entry from the buffer\n", processID, buf[x], x);
				buf[x] = 0;

				//mutex unlock
				ReleaseMutex(mutex);
			}

			//release
			UnmapViewOfFile(buf);
			CloseHandle(shmem);
		}

		if (strstr(task, "swapper") != NULL){
			mutex = CreateMutex(NULL, FALSE, "mutex");

			shmem = CreateFileMapping(
				INVALID_HANDLE_VALUE,
				NULL,
				PAGE_READWRITE,
				0,
				BUF_SIZE,
				"shared_memory"
				);

			char *buf = (char*)MapViewOfFile(shmem, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);

			//generate random and different buffer indexes
			int x = 0;	//first index
			int y = 0;	//second index
			while (x == y){
				x = rand() % (BUF_SIZE - 1 + 1 - 0) + 0;
				y = rand() % (BUF_SIZE - 1 + 1 - 0) + 0;
			}

			/*	check one of the indexes has a value
			if generated indexes are empty, this process cannot swap the values of x th and y th index
			*/
			if (!buf[x] == 0 || !buf[y] == 0){
				swap(&buf[x], &buf[y], sizeof(int)); //this function defined top of the this codes
				printf("child %d swapped %d th index with %d th index\n", processID, x, y);
			}

			//release
			UnmapViewOfFile(buf);
			CloseHandle(shmem);
		}
	}


	system("pause");
	return 1;
}