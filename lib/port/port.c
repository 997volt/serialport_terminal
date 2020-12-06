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

BOOL com_dcb_default_init(HANDLE com_handle)
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

BOOL com_dcb_init(HANDLE com_handle, int baundrate)
{
    DCB dcbSerialParams = { 0 };                         
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (GetCommState(com_handle, &dcbSerialParams))
    {
        dcbSerialParams.BaudRate = baundrate;      
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
        && com_dcb_init(com_handle, 9600)
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

int find_char_index(char array[], int size, char to_find)
{
    for (int i = 0; i < size; i++)
    {
        if (array[i] == to_find)
            return i;
    }
        
    return -1;
}

char * com_read(HANDLE com_handle)
{
    static char serial_buffer[32]; 

    if( com_handle != INVALID_HANDLE_VALUE && com_wait_for_event(com_handle))
    {      
        DWORD read_bytes_count = 1;                     
        for (int i = 0; read_bytes_count > 0; i++)
            ReadFile(com_handle, &serial_buffer[i], sizeof(serial_buffer[i]), &read_bytes_count, NULL);           	
    }    
    
    return serial_buffer;
}



