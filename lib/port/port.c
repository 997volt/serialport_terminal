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


COMMTIMEOUTS com_timeouts_init()
{
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout         = 50;
    timeouts.ReadTotalTimeoutConstant    = 50;
    timeouts.ReadTotalTimeoutMultiplier  = 10;
    timeouts.WriteTotalTimeoutConstant   = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    return timeouts;
}

BOOL com_check_timeouts(HANDLE com_handle, COMMTIMEOUTS timeouts)
{
    if (!SetCommTimeouts(com_handle, &timeouts))
    {
        printf("Error setting timeouts\n");
        CloseHandle(com_handle);
        return FALSE;
    }
    printf("Timeouts set succesfully\n");
    return TRUE;
}

BOOL com_check_mask(HANDLE com_handle)
{
	if (!SetCommMask(com_handle, EV_RXCHAR))
    {
        printf("Error setting CommMask");
        CloseHandle(com_handle);
        return FALSE;
    }
    printf("COM mask set succesfully\n");
    return TRUE;
}

BOOL com_wait_for_event(HANDLE com_handle)
{
    DWORD dwEventMask;
    if (!WaitCommEvent(com_handle, &dwEventMask, NULL))
    {
        printf("Error Setting WaitCommEvent");
        CloseHandle(com_handle);
        return FALSE;
    }	

    return TRUE;
}

int com_read(int port_number)
{
    HANDLE com_handle = com_init(port_number);

    if( com_handle != INVALID_HANDLE_VALUE
        && com_dcb_init(com_handle)
        && com_check_timeouts(com_handle, com_timeouts_init())
        && com_check_mask(com_handle)
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
    
    CloseHandle(com_handle);
    return 0;
}



