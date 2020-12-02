#ifndef _PORT_H_
#define _PORT_H_

#include <stdio.h>
#include <windows.h>

void port_version();
HANDLE com_open(int port_number);
BOOL com_check(int port_number);
void com_read(int port_number);
int com_read_one(int port_number);

#endif