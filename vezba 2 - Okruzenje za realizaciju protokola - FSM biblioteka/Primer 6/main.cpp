#include <stdio.h>
#include <conio.h>

#include "fsmsystem.h"
#include "logfile.h"
#include "autoexample.h"

/* FSM system instance. */
static FSMSystem sys(1 /* max number of automates types */, 1 /* max number of msg boxes */);

DWORD WINAPI SystemThread(void *data) {
	AutoExample automate[3];

	/* Kernel buffer description block */
	/* number of buffer types */
	const uint8 buffClassNo =  4; 
	/* number of buffers of each buffer type */
	uint32 buffsCount[buffClassNo] = { 50, 50, 50, 10 }; 
	/* buffer size for each buffer type */
 	uint32 buffsLength[buffClassNo] = { 128, 256, 512, 1024}; 
	
	/* Logging setting - to a file in this case */
	LogFile lf("log.log" /*log file name*/, "./log.ini" /* message translator file */);
	LogAutomateNew::SetLogInterface(&lf);

	/* Mandatory kernel initialization */
	printf("[*] Initializing system...\n");
	sys.InitKernel(buffClassNo, buffsCount, buffsLength, 5);

	/* Add automates to the system */
	sys.Add(&automate[0], AUTOEXAMPLE_FSM, 3 /* the number of automates that will be added */, true);
	for (int i=1;i<3;i++) {
		sys.Add(&automate[i], AUTOEXAMPLE_FSM);
	}
	/* Start the first automate - usually it sends the first message, 
	since only automates can send messages */
	//automate[0].Start();

	/* Starts the system - dispatches system messages */
	printf("[*] Starting system...\n");
	sys.Start();

	/* Finish thread */
	return 0;
}

void main() {
	DWORD thread_id;
	HANDLE thread_handle;

	/* Start operating thread. */
	thread_handle = CreateThread(NULL, 0, SystemThread, NULL, 0, &thread_id);

	/* Wait for keypress. */
	getch();

	/* Notify the system to stop - this causes the thread to finish */
	printf("[*] Stopping system...\n");
	sys.StopSystem();

	/* Free the thread handle */
	CloseHandle(thread_handle);
}
