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
#include "modules/tcpServerClient.h"


/****************************
 *   TCP CLIENT FUNCTIONS   *
 ****************************/

/**********************************
 *  TCP CLIENT STATIC PROTOTYPES  *
 **********************************/
static void tcp_client_sent_cb(void *arg);
static void tcp_client_recv_cb(void *arg,char *pdata,unsigned short length);
static void tcp_client_recon_cb(void *arg,sint8 error);
static void tcp_client_discon_cb(void *arg);
static void tcp_client_connect_cb(void *arg);

/**********************************
 *   TCP CLIENT STATIC VARIABLES  *
 **********************************/
os_timer_t tcp_client_send_data_timer;
struct espconn tcp_client;
uint8 i;

/**********************************
 *   TCP CLIENT STATIC FUNCTIONS  *
 **********************************/

/**
 * TCP Client数据发送回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void *arg){
	os_printf("tcp client send data successful\r\n");
}

/**
 * TCP Client数据接收回调函数，可以在这处理收到Server发来的数据
 */
static void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("tcp client receive tcp server data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Client重连回调函数，可以在此函数里做重连接处理
 */
static void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void *arg,sint8 error){
	os_printf("tcp client connect tcp server error %d\r\n",error);
	os_timer_disarm(&tcp_client_send_data_timer);//取消定时发送数据定时器
}

/**
 * TCP Client断开连接回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_client_discon_cb(void *arg){
	os_printf("tcp client disconnect tcp server successful\r\n");
	os_timer_disarm(&tcp_client_send_data_timer);
}

/**
 * TCP Client连接成功回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void *arg){
	struct espconn *pespconn = arg;

	os_printf("tcp client connect tcp server successful\r\n");
	espconn_regist_recvcb(pespconn,tcp_client_recv_cb);//注册接收数据回调函数
	espconn_regist_sentcb(pespconn,tcp_client_sent_cb);//注册数据发送完成回调函数
	espconn_regist_disconcb(pespconn,tcp_client_discon_cb);//注册断开连接回调函数

	os_timer_disarm(&tcp_client_send_data_timer);
	os_timer_setfn(&tcp_client_send_data_timer, (os_timer_func_t *) tcp_client_send_data,NULL);//注册Client定时发送数据回调函数
	os_timer_arm(&tcp_client_send_data_timer, 1000, true);//时间设置为1s
}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS  *
 **********************************/
/**
 * TCP Client定时发送数据回调函数
 */
void ICACHE_FLASH_ATTR
tcp_client_send_data(void){
	char buf[256],length;
	os_printf("tcp client send data tcp server\r\n");
	length = os_sprintf(buf,(char *)"Hi this is ESP8266 client! message num %d",i);
	i++;
	espconn_sent(&tcp_client,buf,length);
}

/**
 * TCP Client初始化函数
 * @remote_ip    要连接的TCP Server IP地址
 * @local_ip     ESP8266 IP地址
 * @remote_port  要连接的TCP Server端口号
 */
void ICACHE_FLASH_ATTR
tcp_client_init(uint8 *remote_ip,struct ip_addr *local_ip, int remote_port){

	uint32 server_ip = ipaddr_addr(remote_ip);

	os_printf("tcp client connect to tcp server\r\n");
	tcp_client.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	tcp_client.type = ESPCONN_TCP;

	os_memcpy(tcp_client.proto.tcp->remote_ip,&server_ip,4);//设置要连接的Server IP地址
	tcp_client.proto.tcp->remote_port = remote_port;//设置要连接的Server 端口号
	os_memcpy(tcp_client.proto.tcp->local_ip,local_ip,4);//设置本地IP地址
	tcp_client.proto.tcp->local_port = espconn_port();//设置本地端口号

	espconn_regist_connectcb(&tcp_client,tcp_client_connect_cb);//注册连接成功回调函数
	espconn_regist_reconcb(&tcp_client,tcp_client_recon_cb);//注册断连重新连接回调函数

	espconn_connect(&tcp_client);//Client连接Server
}



/****************************
 *   TCP SERVER FUNCTIONS   *
 ****************************/
/**********************************
 *  TCP SERVER STATIC PROTOTYPES  *
 **********************************/
static void tcp_server_sent_cb(void *arg);
static void tcp_server_recv_cb(void *arg,char *pdata,unsigned short length);
static void tcp_server_recon_cb(void *arg,sint8 error);
static void tcp_server_discon_cb(void *arg);
static void tcp_server_listen_cb(void *arg);

/**********************************
 *   TCP SERVER STATIC VARIABLES  *
 **********************************/
os_timer_t tcp_server_send_data_timer;
struct espconn tcp_server;
uint8 z;

/**********************************
 *   TCP server STATIC FUNCTIONS  *
 **********************************/

/**
 * TCP Server数据发送回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_server_sent_cb(void *arg){
	os_printf("tcp server send data successful\r\n");

}

/**
 * TCP Server数据接收回调函数，可以在这处理收到Client发来的数据
 */
static void ICACHE_FLASH_ATTR
tcp_server_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("tcp server receive tcp client data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

/**
 * TCP Server重连回调函数，可以在此函数里做重连接处理
 */
static void ICACHE_FLASH_ATTR
tcp_server_recon_cb(void *arg,sint8 error){
	os_printf("tcp server connect tcp client error %d\r\n",error);
	os_timer_disarm(&tcp_server_send_data_timer);
}

/**
 * TCP Server断开连接回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_server_discon_cb(void *arg){
	os_printf("tcp server disconnect tcp client successful\r\n");
	os_timer_disarm(&tcp_server_send_data_timer);
}

/**
 * TCP Server监听Client连接回调函数
 */
static void ICACHE_FLASH_ATTR
tcp_server_listen_cb(void *arg){
	struct espconn *pespconn = arg;

	os_printf("tcp server have tcp client connect\r\n");
	espconn_regist_recvcb(pespconn,tcp_server_recv_cb);//注册收到数据回调函数
	espconn_regist_sentcb(pespconn,tcp_server_sent_cb);//注册发送完数据回调函数
	espconn_regist_disconcb(pespconn,tcp_server_discon_cb);//注册断开连接回调函数

	os_timer_disarm(&tcp_server_send_data_timer);
	os_timer_setfn(&tcp_server_send_data_timer, (os_timer_func_t *) tcp_server_send_data,NULL);//注册Server定时发送数据回调函数
	os_timer_arm(&tcp_server_send_data_timer, 1000, true);//设置时间为1s
}

/**********************************
 *   TCP CLIENT GLOBAL FUNCTIONS  *
 **********************************/

/**
 * TCP Server定时发送数据回调函数
 */
void ICACHE_FLASH_ATTR
tcp_server_send_data(void){
	char buf[256],length;
	os_printf("tcp server send data tcp client\r\n");
	length = os_sprintf(buf,(char *)"Hi this is ESP8266 server! message num %d",z);
	z++;
	espconn_send(&tcp_server,buf,length);
}

/**
 * TCP Server初始化函数
 * @local_port 本地监听端口号，与Client连接的端口号一致
 */
void ICACHE_FLASH_ATTR
tcp_server_init(uint16 local_port){

	os_printf("tcp server waiting tcp client connect!\r\n");
	tcp_server.proto.tcp = (esp_tcp *)os_zalloc(sizeof(esp_tcp));
	tcp_server.type = ESPCONN_TCP;

	tcp_server.proto.tcp->local_port = local_port;//设置本地监听的端口号，等待Client连接

	espconn_regist_connectcb(&tcp_server,tcp_server_listen_cb);//注册Server监听回调函数
	espconn_regist_reconcb(&tcp_server,tcp_server_recon_cb);//注册断连重新连接回调函数

	espconn_accept(&tcp_server);//创建Server,开始监听
	espconn_regist_time(&tcp_server,360,0);//超时断开连接时间
}
