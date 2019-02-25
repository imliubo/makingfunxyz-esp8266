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
#include "modules/AP_Web_SmartConfig_Airkiss_test.h"

/*********************
 *      INCLUDES
 *********************/
#include "modules/tcpServerClient.h"
#include "user_interface.h"
#include "Utils/cJSON.h"
#include "modules/wifi.h"
/*********************
 *      DEFINES
 *********************/
#define DEVICE_TYPE 		"gh_9e2cff3dfa51" //wechat public number
#define DEVICE_ID 			"122475" //model ID
#define DEFAULT_LAN_PORT 	12476
/**********************
 *      TYPEDEFS
 **********************/
struct espconn tcp_server;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void softAP_init(void);
static void WifiStatus_Check(void);
static void wifiConnectCb(uint8_t status);
static void smartconfig_done(sc_status status, void *pdata);
/************************
 *   STATIC VARIABLES   *
 ************************/

os_timer_t wifistate_checktimer;
os_timer_t send_data_timer;

/*-------------------------
 *  SmartConfig Airliss   *
 -------------------------*/
LOCAL esp_udp ssdp_udp;
LOCAL struct espconn pssdpudpconn;
LOCAL os_timer_t ssdp_time_serv;

uint8_t  lan_buf[200];
uint16_t lan_buf_len;
uint8 	 udp_sent_cnt = 0;


/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

/**
 * AP配网模式测试代码
 */
void ICACHE_FLASH_ATTR
AP_test(void){

	os_printf("AP config test\r\n");

	wifi_station_disconnect();
	softAP_init();//

	tcp_server_init(&tcp_server,TCP_LOCAL_PORT);
}

/**
 * AP配网模式处理TCP client发送来的数据
 */
void ICACHE_FLASH_ATTR
AP_recv_data_process(uint8 *pdata){

	cJSON *root = cJSON_Parse(pdata);

	if(root != NULL){
		uint8 *SSID = cJSON_GetObjectItem(root,"SSID")->valuestring;//解析SSID key对应的Value值，也就是我们发送的Wi-Fi名称
		uint8 *PWD = cJSON_GetObjectItem(root,"PWD")->valuestring;//解析PWD key对应的Value值，也就是我们发送的Wi-Fi密码
		uint8 *Author = cJSON_GetObjectItem(root,"Author")->valuestring;//
		uint8 *zhuanlan = cJSON_GetObjectItem(root,"zhuanlan")->valuestring;//
		os_printf("\r\nAuthor: %s\r\nzhuluan: %s\r\n",Author,zhuanlan);

		os_printf("\r\nrecv:\r\nSSID: %s   \r\nPWD: %s\r\n",SSID,PWD);//打印我们收到的密码

	    wifi_set_opmode(STATION_MODE);//设置WiFi模式为STATION模式

	    WIFI_Connect(SSID, PWD, wifiConnectCb);//连接目标WiFi
	}else{
		os_printf("json data format error!\r\n");//收到的数据不正确就打印
	}

	cJSON_Delete(root);//解析完JSON后一定要记得释放！
}

/**
 * SmartConfig 测试代码
 */
void ICACHE_FLASH_ATTR
SmartConfig_test(void){
	os_printf("SmartConfig test\r\n");
	smartconfig_set_type(SC_TYPE_ESPTOUCH);//设置快连模式类型，必须在smartconfig_start之前调用
	wifi_set_opmode(STATION_MODE);
	smartconfig_start(smartconfig_done);
}

/**
 * Airkiss 测试代码
 */
void ICACHE_FLASH_ATTR
Airkiss_test(void){
	os_printf("Airkiss test\r\n");
	smartconfig_set_type(SC_TYPE_AIRKISS);//设置快连模式类型，必须在smartconfig_start之前调用
	wifi_set_opmode(STATION_MODE);
	smartconfig_start(smartconfig_done);
}

/**
 * SmartConfig Airkiss 测试代码
 */
void ICACHE_FLASH_ATTR
SmartConfig_Airkiss_test(void){
	os_printf("SmartConfig Airkiss test\r\n");
	smartconfig_set_type(SC_TYPE_ESPTOUCH_AIRKISS);//设置快连模式类型，必须在smartconfig_start之前调用
	wifi_set_opmode(STATION_MODE);
	smartconfig_start(smartconfig_done);
}

/**
 * Web Config 测试代码
 */
void ICACHE_FLASH_ATTR
Web_config_test(void){
	os_printf("Web Config not write\r\n");
}
/************************
 *   STATIC FUNCTIONS   *
 ************************/

/*======================
 *  AP TEST FUNCTIONS   *
 =======================*/

/*
 * softAP模式初始化代码
 */
static void ICACHE_FLASH_ATTR
softAP_init(void){
	struct softap_config soft_ap_Config;

	wifi_set_opmode_current(SOFTAP_MODE);//设置为AP模式，不保存到flash
//	wifi_set_opmode(SOFTAP_MODE);//设置为AP模式，并保存到flash

	soft_ap_Config.ssid_len = 14;//热点名称长度，与你实际的名称长度一致就好
	os_strcpy(soft_ap_Config.ssid,"zhihu-IAMLIUBO");//实际热点名称设置，可以根据你的需要来
	os_strcpy(soft_ap_Config.password,"imliubo123");//热点密码设置
	soft_ap_Config.authmode = AUTH_WPA2_PSK;//设置权限模式，AUTH_WPA2_PSK这是一种加密算法
	soft_ap_Config.beacon_interval = 100;//信标间隔，默认为100
	soft_ap_Config.channel = 1;//信道，共支持1~13个信道
	soft_ap_Config.max_connection = 2;//最大连接数量，最大支持四个，默认四个
	soft_ap_Config.ssid_hidden = 0;//隐藏SSID，0：不隐藏  1：隐藏

	wifi_softap_set_config_current(&soft_ap_Config);//设置 Wi-Fi SoftAP 接口配置，不保存到 Flash
//	wifi_softap_set_config(&soft_ap_Config);//设置 Wi-Fi SoftAP 接口配置，保存到 Flash

	os_printf("\r\nSSID: zhihu-IAMLIUBO\r\nPWD: imliubo123\r\n");
}

static void ICACHE_FLASH_ATTR
WifiStatus_Check(void){
	uint8 wifiStatus;
	wifiStatus = wifi_station_get_connect_status();
	if (wifiStatus == STATION_GOT_IP) {
		os_printf("WiFi connection is successful!\r\n");
		os_timer_disarm(&wifistate_checktimer);

	}else{
		os_printf("WiFi connection failed!\r\n");
	}
}

/**
 * Wi-Fi连接回调函数
 */
static void ICACHE_FLASH_ATTR
wifiConnectCb(uint8_t status){

	os_timer_disarm(&wifistate_checktimer);
	os_timer_setfn(&wifistate_checktimer, (os_timer_func_t *) WifiStatus_Check,NULL);
	os_timer_arm(&wifistate_checktimer, 2000, true);
}

/*===================================
 *  Smartconfig Airkiss FUNCTIONS   *
 ====================================*/

/**
 *
 */
const airkiss_config_t akconf =
{
	(airkiss_memset_fn)&memset,
	(airkiss_memcpy_fn)&memcpy,
	(airkiss_memcmp_fn)&memcmp,
	0,
};

/**
 *
 */
LOCAL void ICACHE_FLASH_ATTR
airkiss_wifilan_time_callback(void)
{
	uint16 i;
	airkiss_lan_ret_t ret;

	if ((udp_sent_cnt++) >30) {
		udp_sent_cnt = 0;
		os_timer_disarm(&ssdp_time_serv);//airkiss_wifilan_time_callback运行三十次后停止运行
		//return;
	}
	//设置本地udp发送端口号，IP地址设置成255.255.255.255是进行广播，UDP有单播，多播和广播三种模式
	ssdp_udp.remote_port = DEFAULT_LAN_PORT;
	ssdp_udp.remote_ip[0] = 255;
	ssdp_udp.remote_ip[1] = 255;
	ssdp_udp.remote_ip[2] = 255;
	ssdp_udp.remote_ip[3] = 255;
	lan_buf_len = sizeof(lan_buf);

	ret = airkiss_lan_pack(AIRKISS_LAN_SSDP_NOTIFY_CMD,
		DEVICE_TYPE, DEVICE_ID, 0, 0, lan_buf, &lan_buf_len, &akconf);
	if (ret != AIRKISS_LAN_PAKE_READY) {
		os_printf("Pack lan packet error!");
		return;
	}

	ret = espconn_sendto(&pssdpudpconn, lan_buf, lan_buf_len);
	if (ret != 0) {
		os_printf("UDP send error!");
	}
	os_printf("Finish send notify!\n");//UDP发送完成
}

/**
 *
 */
LOCAL void ICACHE_FLASH_ATTR
airkiss_wifilan_recv_callbk(void *arg, char *pdata, unsigned short len)
{
	uint16 i;
	remot_info* pcon_info = NULL;

	airkiss_lan_ret_t ret = airkiss_lan_recv(pdata, len, &akconf);
	airkiss_lan_ret_t packret;

	switch (ret){
	case AIRKISS_LAN_SSDP_REQ:
		espconn_get_connection_info(&pssdpudpconn, &pcon_info, 0);
		os_printf("remote ip: %d.%d.%d.%d \r\n",pcon_info->remote_ip[0],pcon_info->remote_ip[1],
			                                    pcon_info->remote_ip[2],pcon_info->remote_ip[3]);
		os_printf("remote port: %d \r\n",pcon_info->remote_port);

        pssdpudpconn.proto.udp->remote_port = pcon_info->remote_port;
		os_memcpy(pssdpudpconn.proto.udp->remote_ip,pcon_info->remote_ip,4);
		ssdp_udp.remote_port = DEFAULT_LAN_PORT;

		lan_buf_len = sizeof(lan_buf);
		packret = airkiss_lan_pack(AIRKISS_LAN_SSDP_RESP_CMD,
			DEVICE_TYPE, DEVICE_ID, 0, 0, lan_buf, &lan_buf_len, &akconf);

		if (packret != AIRKISS_LAN_PAKE_READY) {
			os_printf("Pack lan packet error!");
			return;
		}

		os_printf("\r\n\r\n");
		for (i=0; i<lan_buf_len; i++)
			os_printf("%c",lan_buf[i]);
		os_printf("\r\n\r\n");

		packret = espconn_sendto(&pssdpudpconn, lan_buf, lan_buf_len);
		if (packret != 0) {
			os_printf("LAN UDP Send err!");
		}

		break;
	default:
		os_printf("Pack is not ssdq req!%d\r\n",ret);
		break;
	}
}

/**
 *
 */
void ICACHE_FLASH_ATTR
airkiss_start_discover(void)
{
	ssdp_udp.local_port = DEFAULT_LAN_PORT;//设置本地端口
	pssdpudpconn.type = ESPCONN_UDP;//设置通信方式为UDP
	pssdpudpconn.proto.udp = &(ssdp_udp);//
	espconn_regist_recvcb(&pssdpudpconn, airkiss_wifilan_recv_callbk);//注册收到数据回调函数
	espconn_create(&pssdpudpconn);//创建一个UDP传输

	os_timer_disarm(&ssdp_time_serv);
	os_timer_setfn(&ssdp_time_serv, (os_timer_func_t *)airkiss_wifilan_time_callback, NULL);//注册airkiss定时回调函数
	os_timer_arm(&ssdp_time_serv, 1000, 1);//1s
}

/**
 *
 */
static void ICACHE_FLASH_ATTR
smartconfig_done(sc_status status, void *pdata)
{
    switch(status) {
        case SC_STATUS_WAIT://连接未开始，请勿在此阶段开始连接
            os_printf("SC_STATUS_WAIT\n");
            break;
        case SC_STATUS_FIND_CHANNEL://请在此阶段开启APP进行配对
            os_printf("SC_STATUS_FIND_CHANNEL\n");
            break;
        case SC_STATUS_GETTING_SSID_PSWD://获取到Wi-Fi名称和密码
            os_printf("SC_STATUS_GETTING_SSID_PSWD\n");
			sc_type *type = pdata;
            if (*type == SC_TYPE_ESPTOUCH) {//判断类型，在这一步发来的数据pdata中应该有配置类型
                os_printf("SC_TYPE:SC_TYPE_ESPTOUCH\n");
            } else {
                os_printf("SC_TYPE:SC_TYPE_AIRKISS\n");
            }
            break;
        case SC_STATUS_LINK://开始连接Wi-Fi
            os_printf("SC_STATUS_LINK\n");
            struct station_config *sta_conf = pdata;

	        wifi_station_set_config(sta_conf);
	        wifi_station_disconnect();
	        wifi_station_connect();
            break;
        case SC_STATUS_LINK_OVER://获取到IP，连接路由完成
            os_printf("SC_STATUS_LINK_OVER\n");
            if (pdata != NULL) {//连接完成，如果使用的是SmartConfig，此时手机会将自己的IP地址发给ESP8266
				//SC_TYPE_ESPTOUCH
                uint8 phone_ip[4] = {0};

                os_memcpy(phone_ip, (uint8*)pdata, 4);
                os_printf("Phone ip: %d.%d.%d.%d\n",phone_ip[0],phone_ip[1],phone_ip[2],phone_ip[3]);
            } else {//练成完成，如果是使用的Airkiss方式，到这一步还没有完成，还会跟微信进行数据交互，应该告知微信配网完成之类的
            	//SC_TYPE_AIRKISS - support airkiss v2.0
				airkiss_start_discover();//开始Airkiss内网发现
            }
            smartconfig_stop();//SmartConfig 完成
            break;
    }

}


