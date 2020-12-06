#include <port/port.h>

int main()
{
    port_version();
    HANDLE com_handle = com_open(4);
    int message_size = 0;

    do
    {
        char *serial_read = com_read(com_handle);
        message_size = find_char_index(serial_read, 32, '\n');
        for (int i = 0; i < 32; i++)		// j < i-1 to remove the dupliated last character
            printf("%c", serial_read[i]);	
    } while (message_size > 0);
    
    
    CloseHandle(com_handle);
    
    return 0;
}