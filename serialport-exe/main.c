#include <port/port.h>

int main()
{
    port_version();
    HANDLE com_handle = com_open(4);
    com_read(com_handle);
    CloseHandle(com_handle);
    
    return 0;
}