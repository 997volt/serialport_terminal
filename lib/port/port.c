#include "port.h"

void port_version()
{
    printf("port library version = %.1f\n", PORT_VERSION);
}

BOOL com_check(int port_number)
{
    char com_str[12];
    sprintf(com_str, "\\\\.\\COM%d", port_number);

    HANDLE com_handle = CreateFile( com_str,             
                        GENERIC_READ | GENERIC_WRITE,
                        0, NULL, OPEN_EXISTING, 0, NULL);        

    if (com_handle == INVALID_HANDLE_VALUE)
    {
        printf("Error opening port\n");
        CloseHandle(com_handle);
        return FALSE;
    }

    printf("port opened\n");
    CloseHandle(com_handle);
    return TRUE;
}

int com_read(int port_number)
{
    char com_str[12];
    sprintf(com_str, "\\\\.\\COM%d", port_number);

    HANDLE com_handle = CreateFile( com_str,             
                        GENERIC_READ | GENERIC_WRITE,
                        0, NULL, OPEN_EXISTING, 0, NULL);        

    // check if connected succesfully
    if (com_handle == INVALID_HANDLE_VALUE)
    {
        printf("Error opening port COM%d\n", port_number);
        CloseHandle(com_handle);
        return 1; 
    }
    printf("port opened\n");    

    //parameters
    DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

    if (GetCommState(com_handle, &dcbSerialParams) == FALSE)
    {
        printf("GetCommState() error\n");
        CloseHandle(com_handle);
        return 2;
    }    

    //timeouts
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    
    if (SetCommTimeouts(com_handle, &timeouts) == FALSE)
    {
        printf("Timeouts setting errors\n");
        CloseHandle(com_handle);
        return 3;
    }        
    
    //Configure Windows to Monitor the serial device for Character Reception
	if (SetCommMask(com_handle, EV_RXCHAR) == FALSE)
    {
        printf("Error setting CommMask");
        CloseHandle(com_handle);
        return 4;
    }

    DWORD dwEventMask;                     // Event mask to trigger
    char  TempChar;                        // Temperory Character
    char  SerialBuffer[256];               // Buffer Containing Rxed Data
    DWORD NoBytesRead;                     // Bytes read by ReadFile()
    int i = 0;

    //Wait for the character to be received
	BOOL Status = WaitCommEvent(com_handle, &dwEventMask, NULL); 					

    if (WaitCommEvent(com_handle, &dwEventMask, NULL) == FALSE)
    {
        printf("\n    Error! in Setting WaitCommEvent()");
        CloseHandle(com_handle);
        return 5;
    }			

    do
    {
            Status = ReadFile(com_handle, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
            SerialBuffer[i] = TempChar;
            i++;
    }while (NoBytesRead > 0); 
    
    printf("\n\n    ");
    int j =0;
    for (j = 0; j < i-1; j++)		// j < i-1 to remove the dupliated last character
        printf("%c", SerialBuffer[j]);				
		
    CloseHandle(com_handle);//Closing the Serial Port
}



