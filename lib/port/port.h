#ifndef _PORT_H_
#define _PORT_H_

#include <stdio.h>
#include <windows.h>

void port_version(); 
BOOL com_check(int port_number);
int com_read(int port_number);

#endif