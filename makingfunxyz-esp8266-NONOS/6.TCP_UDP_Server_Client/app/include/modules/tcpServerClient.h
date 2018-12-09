/*MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef TCPSERVERCLIENT_H_
#define TCPSERVERCLIENT_H_

#include "driver/uart.h"
#include "osapi.h"
#include "espconn.h"
#include "mem.h"
#include "os_type.h"

/**
 * TCP Client functions
 */
sint8 tcp_client_send_data(struct espconn *espconn,uint8 *pdata,uint16 length);
void tcp_client_init(struct espconn *espconn,uint8 *remote_ip,struct ip_addr *local_ip, int remote_port);

/**
 * TCP Server functions
 */
sint8 tcp_server_send_data(struct espconn *espconn,uint8 *pdata,uint16 length);
void tcp_server_init(struct espconn *espconn,uint16 local_port);

#endif
