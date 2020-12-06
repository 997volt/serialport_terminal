#ifndef _PORT_H_
#define _PORT_H_

#include <stdio.h>
#include <windows.h>

void port_version();

HANDLE com_init(int port_number);
BOOL com_check(int port_number);
BOOL com_dcb_init(HANDLE com_handle);
BOOL com_timeouts_init(HANDLE com_handle);

BOOL com_check_mask(HANDLE com_handle);
BOOL com_wait_for_event(HANDLE com_handle);
HANDLE com_open(int port_number);
int com_read(int port_number);

#endif