#include "port.h"

void port_version()
{
    printf("port library version = %.1f\n", PORT_VERSION);
}

HANDLE com_init(int port_number)
{
    char com_str[12];
    sprintf(com_str, "\\\\.\\COM%d", port_number);

    return CreateFile( com_str,             
                    GENERIC_READ | GENERIC_WRITE,
                    0, NULL, OPEN_EXISTING, 0, NULL);        
}

BOOL com_check(int port_number)
{
    HANDLE com_handle = com_init(port_number);
    CloseHandle(com_handle);
    return com_handle == INVALID_HANDLE_VALUE;
}

BOOL com_dcb_init(HANDLE com_handle)
{
    DCB dcbSerialParams = { 0 };                         
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (GetCommState(com_handle, &dcbSerialParams))
    {
        dcbSerialParams.BaudRate = CBR_9600;      
        dcbSerialParams.ByteSize = 8;             
        dcbSerialParams.StopBits = ONESTOPBIT;    
        dcbSerialParams.Parity = NOPARITY;   
        return SetCommState(com_handle, &dcbSerialParams);        
    }
    return FALSE;
}

BOOL com_timeouts_init(HANDLE com_handle)
{
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    return SetCommTimeouts(com_handle, &timeouts);
}

HANDLE com_open(int port_number)
{
    HANDLE com_handle = com_init(port_number);

    if( com_handle != INVALID_HANDLE_VALUE
        && com_dcb_init(com_handle)
        && com_timeouts_init(com_handle)
        && SetCommMask(com_handle, EV_RXCHAR))
    {
        printf("COM%d - Serial Port openned succesfully\n", port_number);
        return com_handle;
    }

    printf("Error oppening COM%d\n", port_number);
    return INVALID_HANDLE_VALUE;
}

BOOL com_wait_for_event(HANDLE com_handle)
{
    DWORD dwEventMask;
    if (!WaitCommEvent(com_handle, &dwEventMask, NULL))
    {
        printf("Error Setting WaitCommEvent");
        return FALSE;
    }	
    return TRUE;
}

int com_read(HANDLE com_handle)
{
    if( com_handle != INVALID_HANDLE_VALUE
        && com_wait_for_event(com_handle))
    {
        char  TempChar;                        // Temperory Character
        char  SerialBuffer[256];               // Buffer Containing Rxed Data
        DWORD NoBytesRead;                     // Bytes read by ReadFile()
        int i = 0;	

        do
        {
            BOOL Status = ReadFile(com_handle, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
            SerialBuffer[i] = TempChar;
            i++;
        }while (NoBytesRead > 0); 
        
        for (int j = 0; j < i-1; j++)		// j < i-1 to remove the dupliated last character
            printf("%c", SerialBuffer[j]);	                   	
    }    
    
    return 0;
}



