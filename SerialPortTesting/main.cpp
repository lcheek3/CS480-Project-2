#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <conio.h>
#include <chrono>
#include <thread>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

using namespace::std;
/*
int main() {

	Serial* port = new Serial('COM4');

	if (port->IsConnected()) cout << "Connection established!!!" << endl;

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
	system("pause");
	return 0;
}
*/
// application reads from the specified serial port and reports the collected data
int main()
{
	printf("Welcome to the serial test app!\n\n");
	Serial* SP = new Serial("\\\\.\\COM5");    // adjust as needed

	if (SP->IsConnected())
		printf("We're connected");
	char test[6] = { 'A','4','0','|','4', '0' };
	char test2[6] = { 'A','9','0','|','9', '0' };
	char up[2] = { 'M','2' };
	char down[2] = { 'M','1' };
	char left[2] = { 'M','4' };
	char right[2] = { 'M','3' };
	//char incomingData[256] = "";			// don't forget to pre-allocate memory
	//printf("%s\n",incomingData);
	//int dataLength = in.size();
	//int readResult = 0;
	if (SP->IsConnected())
		SP->WriteData(test,6);
	this_thread::sleep_for(chrono::seconds(5));
	if (SP->IsConnected())
		SP->WriteData(test2, 6);

	while (SP->IsConnected())
	{
		//readResult = SP->WriteData(input, dataLength);
		int c = 0;
		while (1)
		{
			c = 0;

			switch ((c = _getch())) {
			case KEY_UP:
				cout << endl << "Up" << endl;//key up
				SP->WriteData(up, 2);
				break;
			case KEY_DOWN:
				cout << endl << "Down" << endl;   // key down
				SP->WriteData(down, 2);
				break;
			case KEY_LEFT:
				cout << endl << "Left" << endl;  // key right
				SP->WriteData(left, 2);
				break;
			case KEY_RIGHT:
				cout << endl << "Right" << endl;  // key left
				SP->WriteData(right, 2);
				break;
			default:
				//cout << endl << "null" << endl;  // not arrow
				break;
			}

		}

	}
	return 0;
}