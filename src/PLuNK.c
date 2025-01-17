#include <"PLuNK.h">

typedef HANDLE serial

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	openPort();
	
		index -> COM port ID number
		
	Open a serial port
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
serial openPort(int index) {
	
	serial comHandle;
	char portId[3];
	char portName[15]= "\\\\.\\COM";
	
	sprintf(portId,"%d",index % 100);
	strcat(portName,&portId);
	
	comHandle = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	if(comHandle == INVALID_HANDLE_VALUE) {
		return NULL;
	}
	
	return comHandle;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	closePort();
	
		comPort -> handle of port
		
	Close a serial port
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
void closePort(serial comPort) {
	
	CloseHandle(comPort);
	
	return;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	setBaudRate();
	
		comPort -> handle of port
		rate -> rate in Baud of serial port
	
	Set the Baud rate of the serial port
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int setBaudRate(serial comPort, int rate) {
	
	DCB dcbSerialParam = {0};
	dcbSerialParam.DCBlength = sizeof(dcbSerialParam);
	BOOL status = GetCommState(comPort, &dcbSerialParam);
	
	if(status == FALSE) {
		return FALSE
	}
	
	dcbSerialParam.BaudRate = rate;
	
	status = SetCommState(comPort, &dcbSerialParam);
	
	return status;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	setDataBits();
	
		comPort -> handle of port
		bits -> length of data in bits
	
	Set data length of serial port in bits
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int setDataBits(serial comPort, int bits) {
	
	DCB dcbSerialParam = {0};
	dcbSerialParam.DCBlength = sizeof(dcbSerialParam);
	BOOL status = GetCommState(comPort, &dcbSerialParam);
	
	if(status == FALSE) {
		return FALSE
	}
	
	dcbSerialParam.ByteSize = bits;
	
	status = SetCommState(comPort, &dcbSerialParam);
	
	return status;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	setStopBits();
	
		comPort -> handle of port
		bits -> length of stop in bits
	
	Set stop length of serial port in bits
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int setStopBits(serial comPort, int bits) {
	
	DCB dcbSerialParam = {0};
	dcbSerialParam.DCBlength = sizeof(dcbSerialParam);
	BOOL status = GetCommState(comPort, &dcbSerialParam);
	
	if(status == FALSE) {
		return FALSE
	}
	
	dcbSerialParam.StopBits = bits;
	
	status = SetCommState(comPort, &dcbSerialParam);
	
	return status;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	setParityBits();
	
		comPort -> handle of port
		parity -> parity
	
	Set serial port parity bits
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int setParityBits(serial comPort, int parity) {
	
	DCB dcbSerialParam = {0};
	dcbSerialParam.DCBlength = sizeof(dcbSerialParam);
	BOOL status = GetCommState(comPort, &dcbSerialParam);
	
	if(status == FALSE) {
		return FALSE
	}
	
	dcbSerialParam.Parity = parity;
	
	status = SetCommState(comPort, &dcbSerialParam);
	
	return status;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	serialWriteData();
	
		comPort -> handle of port
		data -> data to send
	
	Send data via serial port. Returns -1 if error occurs
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int serialWriteData(serial comPort, const char* data) {
	
	DWORD numBytesWrite = strlen(data);
	DWORD numBytesWritten;
	
	BOOL status = WriteFile(comPort, data, numBytesWrite, &numBytesWritten, NULL);
	
	if(status == FALSE) {
		return -1;
	}
	
	return numBytesWritten;
}

/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	serialReadData();
	
		comPort -> handle of port
		data -> place to put received data
		length -> length of data to read
	
	Receive data via serial port. Returns -1 if error occurs
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/
int serialReadData(serial comPort, char* data, int length) {
	
	DWORD mask;
	DWORD numBytesRead;
	
	BOOL status = WaitCommEvent(comPort, &mask, NULL);
	
	if(status == FALSE) {
		return -1;
	}
	
	status = ReadFile(comPort, data, length, &numBytesRead, NULL);
	data[numBytesRead] = 0;
	
	if(status == FALSE) {
		return -1;
	}
	
	return TRUE;
}