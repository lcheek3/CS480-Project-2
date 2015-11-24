#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>
#include <iostream>

using namespace::std;

int main() {
	//Serial* port = new Serial("COM4");

	/*if (port->IsConnected()) cout << "Connection established!!!" << endl;

	char data[256] = "";
	int datalength = 256;
	int readResult = 0;

	for (int i = 0; i < 256; ++i) { data[i] = 0; }

	string str;
	char command[] = "i";
	int msglen = strlen(command);
	port->WriteData(command, msglen);
	while (1) {
		while (port->ReadData(data, 256) != -1) {
			printf("%s", data);
		}
	}
	system("pause");*/
	return 0;
}

/*// application reads from the specified serial port and reports the collected data
int main(int argc, _TCHAR* argv[])
{
	printf("Welcome to the serial test app!\n\n");

	Serial* SP = new Serial("COM4");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");

	char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	int dataLength = 255;
	int readResult = 0;

	while (SP->IsConnected())
	{
		readResult = SP->ReadData(incomingData, dataLength);
		// printf("Bytes read: (0 means no data available) %i\n",readResult);
		incomingData[readResult] = 0;

		printf("%s", incomingData);

		Sleep(500);
	}
	return 0;
}*/