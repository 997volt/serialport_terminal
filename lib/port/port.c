#include "port.h"

void port_version()
{
    printf("port library version = %.1f\n", PORT_VERSION);
}

HANDLE com_open(int port_number)
{
    char com_str[12];
    sprintf(com_str, "\\\\.\\COM%d", port_number);

    return CreateFile( com_str,             
                    GENERIC_READ | GENERIC_WRITE,
                    0, NULL, OPEN_EXISTING, 0, NULL);        
}

BOOL com_check_handle(HANDLE com_handle)
{
    if (com_handle == INVALID_HANDLE_VALUE)
    {
        printf("Error opening port\n");
        CloseHandle(com_handle);
        return FALSE;
    }

    printf("port opened succesfully\n");
    return TRUE;
}

BOOL com_check(int port_number)
{
    HANDLE com_handle = com_open(port_number);

    BOOL result = com_check_handle(com_handle);
    CloseHandle(com_handle);
    return result;
}

DCB com_dcb_init()
{
    DCB dcbSerialParams = { 0 };                         
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;      
    dcbSerialParams.ByteSize = 8;             
    dcbSerialParams.StopBits = ONESTOPBIT;    
    dcbSerialParams.Parity = NOPARITY;        
    return dcbSerialParams;
}

BOOL com_check_dcb(HANDLE com_handle, DCB dcbSerialParams)
{
    if (!GetCommState(com_handle, &dcbSerialParams))
    {
        printf("Error initializing DCB\n");
        CloseHandle(com_handle);
        return FALSE;
    }  

    printf("DCB initialized succesfully\n");
    return TRUE;
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

int com_read(int port_number)
{
    HANDLE com_handle = com_open(port_number);
    DCB dcbSerialParams = com_dcb_init();
    COMMTIMEOUTS timeouts = com_timeouts_init();

    if( com_check_handle(com_handle)
        && GetCommState(com_handle, &dcbSerialParams)
        && SetCommTimeouts(com_handle, &timeouts)
        && SetCommMask(com_handle, EV_RXCHAR))
    {
        DWORD dwEventMask;                     // Event mask to trigger
        char  TempChar;                        // Temperory Character
        char  SerialBuffer[256];               // Buffer Containing Rxed Data
        DWORD NoBytesRead;                     // Bytes read by ReadFile()
        int i = 0;				

        if (!WaitCommEvent(com_handle, &dwEventMask, NULL))
        {
            printf("\n    Error! in Setting WaitCommEvent()");
            CloseHandle(com_handle);
            return 1;
        }			

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



