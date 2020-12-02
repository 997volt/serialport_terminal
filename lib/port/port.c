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

    DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;      // Setting BaudRate = 9600
    dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
    dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

    if (GetCommState(com_handle, &dcbSerialParams) == FALSE)
    {
        printf("GetCommState() error\n");
        return 2;
    }    

   
}



