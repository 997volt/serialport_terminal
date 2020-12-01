#include "port.h"

void port_version()
{
    printf("port library version = %.1f\n", PORT_VERSION);
}

char check_com(int port_number)
{
    char com_str[12], result;
    sprintf(com_str, "\\\\.\\COM%d", port_number);

    HANDLE com_handle = CreateFile( com_str,             
                        GENERIC_READ | GENERIC_WRITE,
                        0, NULL, OPEN_EXISTING, 0, NULL);        

    if (com_handle == INVALID_HANDLE_VALUE)
    {
        printf("Error opening port");
        result = 0;
    }        
    else
    {
        printf("port opened");
        result = 1;
    }

  CloseHandle(com_handle);
  return result;
}

