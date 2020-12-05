#ifndef _PORT_H_
#define _PORT_H_

#include <stdio.h>
#include <windows.h>

void port_version();
HANDLE com_open(int port_number);
BOOL com_check_handle(HANDLE com_handle);
BOOL com_check(int port_number);
DCB com_dcb_init();
BOOL com_check_dcb(HANDLE com_handle, DCB dcbSerialParams);
COMMTIMEOUTS com_timeouts_init();
BOOL com_check_timeouts(HANDLE com_handle, COMMTIMEOUTS timeouts);
BOOL com_check_mask(HANDLE com_handle);
BOOL com_wait_for_event(HANDLE com_handle);
int com_read(int port_number);

#endif