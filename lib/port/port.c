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
    

   
}



