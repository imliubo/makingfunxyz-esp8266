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
#include "modules/udp.h"


/****************************
 *   UDP FUNCTIONS   *
 ****************************/

/**********************************
 *  UDP STATIC PROTOTYPES  *
 **********************************/
static void udp_sent_cb(void *arg);
static void udp_recv_cb(void *arg,char *pdata,unsigned short length);

/**********************************
 *   UDP STATIC VARIABLES  *
 **********************************/

/**********************************
 *   UDP STATIC FUNCTIONS  *
 **********************************/


static void ICACHE_FLASH_ATTR
udp_sent_cb(void *arg){
	os_printf("udp send data successful\r\n");
}


static void ICACHE_FLASH_ATTR
udp_recv_cb(void *arg,char *pdata,unsigned short len){
	os_printf("udp receive data\r\n");
	os_printf("length: %d \r\ndata: %s\r\n",len,pdata);

	//TO DO

	/**
	 *process the receive data
	 */
}

/**********************************
 *   UDP GLOBAL FUNCTIONS  *
 **********************************/

sint8 ICACHE_FLASH_ATTR
udp_send_data(struct espconn *espconn,uint8 *pdata,uint16 length){
    if (espconn == NULL) {
    	return ESPCONN_ARG;
    }
    espconn_send(espconn,pdata,length);
}

/**
 * UDP Client³õÊ¼»¯º¯Êý
 * @remote_ip    ÒªÁ¬½ÓµÄudp Server IPµØÖ·
 * @local_ip     ESP8266 IPµØÖ·
 * @remote_port  ÒªÁ¬½ÓµÄudp Server¶Ë¿ÚºÅ
 */
void ICACHE_FLASH_ATTR
udp_init(struct espconn *espconn,uint8 *remote_ip,struct ip_addr *local_ip,int remote_port){

	struct ip_info esp_8266_ipconfig;
	uint32 udp_remote_ip = ipaddr_addr(remote_ip);

    wifi_set_broadcast_if(STATION_MODE);
    espconn->proto.udp = (esp_udp *) os_zalloc(sizeof(esp_udp));
    espconn->type = ESPCONN_UDP;
    os_memcpy(espconn->proto.udp->remote_ip, &udp_remote_ip, 4);
    espconn->proto.udp->remote_port = remote_port;
    espconn->proto.udp->local_port = UDP_LOCAL_PORT;   

    espconn_regist_recvcb(espconn, udp_recv_cb);        
    espconn_regist_sentcb(espconn, udp_sent_cb);     
    espconn_create(espconn);

}
