#undef UNICODE

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "SerialClass.h"	// Library described above
#include <string>
#include <iostream>

using namespace::std;


// NOTES: Need to send X/Y location data to arduino
// HOW TO: Need to know min/max of X/Y, so can send correct number of digits
//     if each is 3 digits, need to only send 6, or will send line ending which will break stuff
// THOUGHTS ON HOW TO SEND:
//     XXXYYY, treat as long int, break in half?
//        XXXYYY % XXX000 = YYY  <- Can create temp int from \/ that one, multiply by 1000 to get XXX000
//        XXXYYY * 1000   = XXX.YYY, Ys drop because Int
//     Above can be done for four digit numbers easily


// OKAY SO: Need to know how many bits X/Y is in,
// DO LIKE THIS:
//  0XXXXYYYY for automatic, read first bit, know auto, read four bits twice (Or three), store into ints
//  1C------- for manual, read first bit, know manual mode, C = char from key input, ignore rest
//     - ARDUINO PARSES INTS. SEND AS 11, 12, 13, 14, for L/R/U/D
//     - SO ON ARDUINO SIDE, CAN JUST LOOK AT NUMBER: IF LESS THAN 20, DROP INTO ONE IF STATEMENT
//         IF GREATER THAN 20, DEALING WITH 100000000stuff, so drop into breaking that apart
//   NOTE: THIS MEANS X > 0 AND Y > 0. IF X == 0, AND Y < 20, COULD DROP INTO MANUAL MODE
//          Which, really, might not be a big deal. Odds of that happening low, and if does happen,
//          unless y == 11, 12, 13, or 14, it won't do anything. And if it DOES do anything, it wouldn't do much

// If automatic == true, then sends the xValue/yValue of the cursor to the arduino,
//     if automatic == false, then sends the command (w, a, s, or d) to arduino in int form
void sendCommandToArduino(Serial *port, bool autoMatic, char command, int xValue, int yValue)
{
    char sendMeAuto[8];   // Needs to be of length 1 (command bit) + xValue Length + yValue Length, for this, assume's length 4
    char sendMeManual[1]; // Two bits, one to signify that is manual mode, second is what direction to go
                             //  11 = W = Up, 12 = A = Left, 13 = S = Down, 14 = D = Right, 15 = OtherKey = DoNothing
    if(autoMatic)
    {
        sendMeAuto[0] = 0;
        int xBitToSend = xValue / 1000;   // = X.xxx, chopped off because int
        sendMeAuto[1] = xBitToSend;
        xBitToSend = (xValue / 100) % 10; // = XX.xx % 10 = 0X.xx, chopped because int
        sendMeAuto[2] = xBitToSend;
        xBitToSend = (xValue / 10) % 10;  // = XXX.x % 10 = 00X.x, chopped because int
        sendMeAuto[3] = xBitToSend;
        xBitToSend = xValue % 10;         // = XXXX % 10 = 000X, which we want
        sendMeAuto[4] = xBitToSend;

        int yBitToSend = yValue / 1000;   // = Y.yyy, chopped off because int
        sendMeAuto[5] = yBitToSend;
        yBitToSend = (yValue / 100) % 10; // = YY.yy % 10 = 0Y.yy, chopped because int
        sendMeAuto[6] = yBitToSend;
        yBitToSend = (yValue / 10) % 10;  // = YYY.y % 10 = 00Y.y, chopped because int
        sendMeAuto[7] = yBitToSend;
        yBitToSend = yValue % 10;         // = YYYY % 10 = 000Y, which we want
        sendMeAuto[8] = yBitToSend;
    }
    else
    {
        sendMeManual[0] = '1';
        if(command == 'w')
            sendMeManual[1] = '1';
        else if(command == 'a')
            sendMeManual[1] = '2';
        else if(command == 's')
            sendMeManual[1] = '3';
        else if(command == 'd')
            sendMeManual[1] = '4';
        else
            sendMeManual[1] = '5';
    }

	if(autoMatic)
        port->WriteData(sendMeAuto, 9);   // Sends first command bit, then the pair of 4 digit numbers, without line ending
    else
        port->WriteData(sendMeManual, 2); // Sends first command bit, then the following directional bit, without line ending

    //port->WriteData(sendMe, 2);
    //cout << "Please write two digit number (00 = turn off, 01 = turn on, 11 = blink): ";

}
int main() {
	Serial* port = new Serial("COM4");

	if (port->IsConnected()) cout << "Connection established!!!" << endl;

    int tempX = 1111;
    int tempY = 1111;
    bool automatic = true;
    sendCommandToArduino(port, automatic, 'w', tempX, tempY);

	//char data[256] = "";
	//int datalength = 256;
	//int readResult = 0;

	//for (int i = 0; i < 256; ++i) { data[i] = 0; }

	//string str;
	//char command[] = "i";
	/*char sendMe[1];
	sendMe[0] = '1';
	sendMe[0] = '0';
	int msglen = strlen(sendMe);
	while(true){
        port->WriteData(sendMe, 2);
        cout << "Please write two digit number (00 = turn off, 01 = turn on, 11 = blink): ";
        cin >> sendMe[0];
        cin >> sendMe[1];
        system("pause");
        if(sendMe[0] == '1')
            sendMe[0] = '0';
        else
            sendMe[0] = '1';

    }
	//while (1) {
	//	while (port->ReadData(data, 256) != -1) {
	//		printf("%s", data);
	//	}
	//} */
	system("pause");
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
