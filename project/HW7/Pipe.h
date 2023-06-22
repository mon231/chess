#pragma once
#pragma region Includes
#include "stdafx.h"

#include <windows.h> //TCHAR isn't needed, it's goal is windows system
#include <string>
#include <stdio.h>
#include <conio.h>

#pragma endregion

#define BUFFER_SIZE		1024 // 1K

using namespace std;

class Pipe
{
private:
	HANDLE hPipe;
	LPTSTR  strPipeName;

public:
	Pipe() {
		// Prepare the pipe name
		strPipeName  = (LPTSTR)TEXT("\\\\.\\pipe\\chessPipe");
		hPipe = HANDLE(NULL);
	}

	bool connect() {
		hPipe = CreateFile(
			strPipeName,			// Pipe name 
			GENERIC_READ |			// Read and write access 
			GENERIC_WRITE,
			0,						// No sharing 
			NULL,					// Default security attributes
			OPEN_EXISTING,			// Opens existing pipe 
			0,						// Default attributes 
			NULL);					// No template file 

		// Break if the pipe handle is valid. 
		if (hPipe != INVALID_HANDLE_VALUE) {
			return true;
		}

		if (/*Exit if an error other than ERROR_PIPE_BUSY occurs*/ GetLastError() != ERROR_PIPE_BUSY || /*All pipe instances are busy, so wait for 5 seconds*/ !WaitNamedPipe(strPipeName, 5000)) {
			printf("Unable to open named pipe %ws w/err 0x%08lx\n", strPipeName, GetLastError());
			return false;
		}

		printf("The named pipe, %ws, is connected.\n", strPipeName);
		return true;
	}

	template<typename T>
	static int get_string_length(const T* str) {//fit for char* and TCHAR*
		if (*str == 0) {
			return 0;
		} else {
			return  1 + get_string_length(1 + str);
		}
	}

	bool send_message_to_graphics(char* msg) {
		//char ea[] = "SSS";
		char* chRequest = msg;	// Client -> Server
		DWORD cbBytesWritten, cbRequestBytes;

		// Send one message to the pipe.

		cbRequestBytes = sizeof(TCHAR) * (get_string_length<char>(chRequest) + 1) / 2; //const form
		
		BOOL bResult = WriteFile(			// Write to the pipe.
			hPipe,						// Handle of the pipe
			chRequest,					// Message to be written
			cbRequestBytes,				// Number of bytes to write
			&cbBytesWritten,			// Number of bytes written
			NULL);						// Not overlapped 

		if (!bResult/*Failed*/ || cbRequestBytes != cbBytesWritten/*Failed*/) {
			printf("WriteFile failed w/err 0x%08lx\n", GetLastError());
			throw std::exception();
		}

		printf("Sends %ld bytes; Message: \"%s\"\n", cbBytesWritten, chRequest);
		return true;
	}

	std::string get_message_from_graphics() {
		DWORD cbBytesRead;
		DWORD cbReplyBytes;
		char chReply[BUFFER_SIZE] = { 0 };		// Server -> Client

		cbReplyBytes = sizeof(char) * BUFFER_SIZE;
		BOOL bResult = ReadFile(			// Read from the pipe.
			hPipe,					// Handle of the pipe
			chReply,				// Buffer to receive the reply
			cbReplyBytes,			// Size of buffer 
			&cbBytesRead,			// Number of bytes read 
			NULL);					// Not overlapped 

		if (!bResult && GetLastError() != ERROR_MORE_DATA) {
			printf("ReadFile failed w/err 0x%08lx\n", GetLastError());
			throw std::exception();
		}

		printf("Receives %ld bytes; Message: \"%s\"\n", cbBytesRead, chReply);
		return chReply;
	}

	void close() {
		CloseHandle(hPipe);
	}
};
