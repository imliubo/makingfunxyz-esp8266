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
#include "modules/TCP/tcpServerClient.h"


/****************************
 *   TCP CLIENT FUNCTIONS   *
 ****************************/

/**********************************
 *  TCP CLIENT STATIC PROTOTYPES  *
 **********************************/
void tcp_client_sent_cb(void *arg);
void tcp_client_recv_cb(void *arg,char *pdata,unsigned short length);
void tcp_client_recon_cb(void *arg,sint8 error);
void tcp_client_discon_cb(void *arg);
void tcp_client_connect_cb(void *arg);

/**********************************
 *   TCP CLIENT STATIC VARIABLES  *
 **********************************/
os_timer_t tcp_client_send_data_timer;
struct espconn tcp_client;

/**********************************
 *   TCP CLIENT STATIC FUNCTIONS  *
 **********************************/
void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void *arg){
	os_printf("tcp client send data successful\r\n");
}

void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("tcp client receive tcp server data\r\n");
	os_printf("length: %d \r\n data: %s\r\n",len,pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void *arg,sint8 error){
	os_printf("tcp client connect tcp server error %s\r\n",error);
}

void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void *arg){
	os_printf("tcp client disconnect tcp server successful\r\n");
}

void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void *arg){
	struct espconn *pespconn = arg;

	os_printf("tcp client connect tcp server successful\r\n");
	espconn_regist_recvcb(pespconn,tcp_client_recv_cb);
	espconn_regist_sentcb(pespconn,tcp_client_sent_cb);
	espconn_regist_disconcb(pespconn,tcp_client_discon_cb);

}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS  *
 **********************************/
void ICACHE_FLASH_ATTR
tcp_client_send_data(void *arg,char *pdata,unsigned short length){
	os_printf("tcp client send data tcp server\r\n");
	os_printf("length: %d \r\n data: %s\r\n",length,pdata);
	espconn_sent((struct espconn *)arg,pdata,length);
}

void ICACHE_FLASH_ATTR
tcp_client_init(uint8 *remote_ip,struct ip_addr *local_ip, int remote_port){

	uint32 server_ip = ipaddr_addr(remote_ip);

	tcp_client.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	tcp_client.type = ESPCONN_TCP;

	os_memcpy(tcp_client.proto.tcp->remote_ip,&server_ip,4);
	tcp_client.proto.tcp->local_port = espconn_port();
	os_memcpy(tcp_client.proto.tcp->local_ip,local_ip,4);
	tcp_client.proto.tcp->remote_port = remote_port;

	espconn_regist_connectcb(&tcp_client,tcp_client_connect_cb);
	espconn_regist_reconcb(&tcp_client,tcp_client_recon_cb);

	espconn_connect(&tcp_client);
}

/****************************
 *   TCP SERVER FUNCTIONS   *
 ****************************/
