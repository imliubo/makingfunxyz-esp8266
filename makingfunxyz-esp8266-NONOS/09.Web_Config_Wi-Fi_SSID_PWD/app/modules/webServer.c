/*
MIT License

Copyright (c) 2018 imliubo
Base On Dave Gamble and cJSON contributors

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
#include "modules/webServer.h"

/*********************
 *      INCLUDES
 *********************/
#include "ets_sys.h"
#include "osapi.h"
#include "spi_flash.h"
#include "espconn.h"
#include "mem.h"
#include "gpio.h"
#include "user_interface.h"
#include <stdlib.h>
#include <stdio.h>
/*********************
 *      DEFINES
 *********************/
#define INDEX_SIZE 		1437
#define WEBCONFIG_SIZE 	1541
#define WIFIDONE_SIZE	1293

/**********************
 *      TYPEDEFS
 **********************/
static struct espconn webserver_espconn;
static esp_tcp webserver_esptcp;
/**********************
 *  STATIC PROTOTYPES
 **********************/
static void webconfig_get_wifi_ssid_pwd(char* urlparam);
static void softAP_init(void);
static bool parse_url(char *precv, URL_Frame *purl_frame);
static void webserver_recv(void *arg, char *pusrdata, unsigned short length);
static void data_send(void *arg, bool responseOK, char *psend);
static bool parse_url(char *precv, URL_Frame *purl_frame);
static bool save_data(char *precv, uint16 length);
static bool check_data(char *precv, uint16 length);
static void webserver_recon(void *arg, sint8 err);
static void webserver_discon(void *arg);
static void webserver_listen(void *arg);
/************************
 *   STATIC VARIABLES   *
 ************************/
static char *precvbuffer;
static uint32 dat_sumlength = 0;

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void ICACHE_FLASH_ATTR
user_webserver_init(uint32 port){

	softAP_init();

	webserver_espconn.type = ESPCONN_TCP;
	webserver_espconn.state = ESPCONN_NONE;
	webserver_espconn.proto.tcp = &webserver_esptcp;
	webserver_espconn.proto.tcp->local_port = port;
	espconn_regist_connectcb(&webserver_espconn, webserver_listen);
	espconn_regist_reconcb(&webserver_espconn,webserver_recon);

	espconn_accept(&webserver_espconn);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
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
webserver_recv(void *arg, char *pusrdata, unsigned short length)
{
    URL_Frame *pURL_Frame = NULL;
    char *pParseBuffer = NULL;
    char *html = NULL;
    SpiFlashOpResult ret = 0;

    os_printf("\r\n\r\nlength:%d\r\n", length);
    os_printf("recv:%s", pusrdata);


    pURL_Frame = (URL_Frame *)os_zalloc(sizeof(URL_Frame));

    parse_url(pusrdata, pURL_Frame);
    os_printf("\r\nType[%d]\r\n", pURL_Frame->Type);
    os_printf("pSelect[%s]\r\n", pURL_Frame->pSelect);
    os_printf("pCommand[%s]\r\n", pURL_Frame->pCommand);
    os_printf("pFilename[%s]\r\n", pURL_Frame->pFilename);

    switch (pURL_Frame->Type) {
        case GET:
        	os_printf("We have a GET request.\n");
        		if(pURL_Frame->pFilename[0] == 0){
					html = (char *)os_zalloc(INDEX_SIZE);
					if(html == NULL){
						os_printf("os_zalloc error!\r\n");
						goto _temp_exit;
					}
					// Flash read/write has to be aligned to the 4-bytes boundary
					ret = spi_flash_read(508*4096, (uint32 *)html, INDEX_SIZE);
					if(ret != SPI_FLASH_RESULT_OK){
						os_printf("spi_flash_read err:%d\r\n", ret);
						os_free(html);
						html = NULL;
						goto _temp_exit;
					}
					html[INDEX_SIZE] = 0;   // put 0 to the end
					data_send(arg, true, html);
					os_free(html);
					html = NULL;
				}
				if(strncmp(pURL_Frame->pFilename, "WebConfig.html", strlen("WebConfig.html")) == 0){
					html = (char *)os_zalloc(WEBCONFIG_SIZE);
					if(html == NULL){
						os_printf("os_zalloc error!\r\n");
						goto _temp_exit;
					}
					// Flash read/write has to be aligned to the 4-bytes boundary
					ret = spi_flash_read(510*4096, (uint32 *)html, WEBCONFIG_SIZE);
					if(ret != SPI_FLASH_RESULT_OK){
						os_printf("spi_flash_read err:%d\r\n", ret);
						os_free(html);
						html = NULL;
						goto _temp_exit;
					}
					html[WEBCONFIG_SIZE] = 0;
					data_send(arg, true, html);
					os_free(html);
					html = NULL;
				}
            break;

        case POST:
        	os_printf("We have a POST request.\r\n");
        	if(strncmp(pURL_Frame->pCommand, "connect-wifi", strlen("connect-wifi")) == 0){
        		os_printf("connect wifi\r\n");
        		webconfig_get_wifi_ssid_pwd(pusrdata);
        		html = (char *)os_zalloc(WIFIDONE_SIZE);
                if(html == NULL){
                	os_printf("os_zalloc error!\r\n");
                    goto _temp_exit;
                }
                ret = spi_flash_read(512*4096, (uint32 *)html, WIFIDONE_SIZE);
                if(ret != SPI_FLASH_RESULT_OK){
                	os_printf("spi_flash_read err:%d\r\n", ret);
                    os_free(html);
                    html = NULL;
                    goto _temp_exit;
                }
                html[WIFIDONE_SIZE] = 0;
                data_send(arg, true, html);
                os_free(html);
                html = NULL;
        	}
            break;
    }
    _temp_exit:
        ;
    if(pURL_Frame != NULL){
        os_free(pURL_Frame);
        pURL_Frame = NULL;
    }

}

static void ICACHE_FLASH_ATTR
webconfig_get_wifi_ssid_pwd(char* urlparam)
{
    char *p = NULL, *q = NULL;
    char ssid[10], pass[15];

    os_memset(ssid, 0, sizeof(ssid));
    os_memset(pass, 0, sizeof(pass));

    p = (char *)os_strstr(urlparam, "SSID=");
    q = (char *)os_strstr(urlparam, "PASSWORD=");
    if ( p == NULL || q == NULL ){
        return;
    }
    os_memcpy(ssid, p + 5, q - p - 6);
    os_memcpy(pass, q + 9, os_strlen(urlparam) - (q - urlparam) - 9);
    os_printf("ssid[%s]pass[%s]\r\n", ssid, pass);

    wifi_set_opmode(STATION_MODE);
    struct station_config stConf;
    stConf.bssid_set = 0;
    os_memset(&stConf.ssid, 0, sizeof(stConf.ssid));
    os_memset(&stConf.password, 0, sizeof(stConf.password));

    os_memcpy(&stConf.ssid, ssid, os_strlen(ssid));
    os_memcpy(&stConf.password, pass, os_strlen(pass));

    wifi_station_set_config(&stConf);
    //重启
    system_restart();
}
/******************************************************************************
 * FunctionName : parse_url
 * Description  : parse the received data from the server
 * Parameters   : precv -- the received data
 *                purl_frame -- the result of parsing the url
 * Returns      : none
*******************************************************************************/
static bool ICACHE_FLASH_ATTR
parse_url(char *precv, URL_Frame *purl_frame)
{
    char *str = NULL;
    uint8 length = 0;
    char *pbuffer = NULL;
    char *pbufer = NULL;

    if (purl_frame == NULL || precv == NULL) {
        return false;
    }

    pbuffer = (char *)os_strstr(precv, "Host:");

    if (pbuffer != NULL) {
        length = pbuffer - precv;
        pbufer = (char *)os_zalloc(length + 1);
        pbuffer = pbufer;
        os_memcpy(pbuffer, precv, length);
        os_memset(purl_frame->pSelect, 0, URLSize);
        os_memset(purl_frame->pCommand, 0, URLSize);
        os_memset(purl_frame->pFilename, 0, URLSize);

        if (os_strncmp(pbuffer, "GET ", 4) == 0) {
            purl_frame->Type = GET;
            pbuffer += 4;
        } else if (os_strncmp(pbuffer, "POST ", 5) == 0) {
            purl_frame->Type = POST;
            pbuffer += 5;
        }else{
        	return false;
        }

        pbuffer ++;
        str = (char *)os_strstr(pbuffer, "HTTP");

        if (str != NULL) {
            length = str - pbuffer - 1;
            os_memcpy(purl_frame->pFilename, pbuffer, length);
        }

        os_free(pbufer);
    }

    pbuffer = (char *)os_strstr(precv, "SSID");
    if (pbuffer != NULL) {
    	purl_frame->Type = POST;
    	os_memcpy(purl_frame->pCommand, "connect-wifi", strlen("connect-wifi"));
    	os_free(pbufer);
    }

}
/******************************************************************************
 * FunctionName : data_send
 * Description  : processing the data as http format and send to the client or server
 * Parameters   : arg -- argument to set for client or server
 *                responseOK -- true or false
 *                psend -- The send data
 * Returns      :
*******************************************************************************/
static void ICACHE_FLASH_ATTR
data_send(void *arg, bool responseOK, char *psend)
{
    uint16 length = 0;
    char *pbuf = NULL;
    char httphead[256];
    struct espconn *ptrespconn = arg;
    os_memset(httphead, 0, 256);

    if (responseOK) {
        os_sprintf(httphead,
                   "HTTP/1.0 200 OK\r\nContent-Length: %d\r\nServer: lwIP/1.4.0\r\n",
                   psend ? os_strlen(psend) : 0);

        if (psend) {
            os_sprintf(httphead + os_strlen(httphead),
                       "Content-type: text/html; charset=utf-8\r\nPragma: no-cache\r\n\r\n");
            length = os_strlen(httphead) + os_strlen(psend);
            pbuf = (char *)os_zalloc(length + 1);
            os_memcpy(pbuf, httphead, os_strlen(httphead));
            os_memcpy(pbuf + os_strlen(httphead), psend, os_strlen(psend));
        } else {
            os_sprintf(httphead + os_strlen(httphead), "\n");
            length = os_strlen(httphead);
        }
    } else {
        os_sprintf(httphead, "HTTP/1.0 400 BadRequest\r\nContent-Length: 0\r\nServer: lwIP/1.4.0\r\n\n");
        length = os_strlen(httphead);
    }

    if (psend) {
        espconn_sent(ptrespconn, pbuf, length);
    } else {
        espconn_sent(ptrespconn, httphead, length);
    }

    if (pbuf) {
        os_free(pbuf);
        pbuf = NULL;
    }
}

static bool ICACHE_FLASH_ATTR
save_data(char *precv, uint16 length)
{
    bool flag = false;
    char length_buf[10] = {0};
    char *ptemp = NULL;
    char *pdata = NULL;
    uint16 headlength = 0;
    static uint32 totallength = 0;

    ptemp = (char *)os_strstr(precv, "\r\n\r\n");

    if (ptemp != NULL) {
        length -= ptemp - precv;
        length -= 4;
        totallength += length;
        headlength = ptemp - precv + 4;
        pdata = (char *)os_strstr(precv, "Content-Length: ");

        if (pdata != NULL) {
            pdata += 16;
            precvbuffer = (char *)os_strstr(pdata, "\r\n");

            if (precvbuffer != NULL) {
                os_memcpy(length_buf, pdata, precvbuffer - pdata);
                dat_sumlength = atoi(length_buf);
            }
        } else {
        	if (totallength != 0x00){
        		totallength = 0;
        		dat_sumlength = 0;
        		return false;
        	}
        }
        if ((dat_sumlength + headlength) >= 1024) {
        	precvbuffer = (char *)os_zalloc(headlength + 1);
            os_memcpy(precvbuffer, precv, headlength + 1);
        } else {
        	precvbuffer = (char *)os_zalloc(dat_sumlength + headlength + 1);
        	os_memcpy(precvbuffer, precv, os_strlen(precv));
        }
    } else {
        if (precvbuffer != NULL) {
            totallength += length;
            os_memcpy(precvbuffer + os_strlen(precvbuffer), precv, length);
        } else {
            totallength = 0;
            dat_sumlength = 0;
            return false;
        }
    }

    if (totallength == dat_sumlength) {
        totallength = 0;
        dat_sumlength = 0;
        return true;
    } else {
        return false;
    }
}

static bool ICACHE_FLASH_ATTR
check_data(char *precv, uint16 length)
{
        //bool flag = true;
    char length_buf[10] = {0};
    char *ptemp = NULL;
    char *pdata = NULL;
    char *tmp_precvbuffer;
    uint16 tmp_length = length;
    uint32 tmp_totallength = 0;

    ptemp = (char *)os_strstr(precv, "\r\n\r\n");

    if (ptemp != NULL) {
        tmp_length -= ptemp - precv;
        tmp_length -= 4;
        tmp_totallength += tmp_length;

        pdata = (char *)os_strstr(precv, "Content-Length: ");

        if (pdata != NULL){
            pdata += 16;
            tmp_precvbuffer = (char *)os_strstr(pdata, "\r\n");

            if (tmp_precvbuffer != NULL){
                os_memcpy(length_buf, pdata, tmp_precvbuffer - pdata);
                dat_sumlength = atoi(length_buf);
                os_printf("A_dat:%u,tot:%u,lenght:%u\n",dat_sumlength,tmp_totallength,tmp_length);
                if(dat_sumlength != tmp_totallength){
                    return false;
                }
            }
        }
    }
    return true;
}

/******************************************************************************
 * FunctionName : webserver_recon
 * Description  : the connection has been err, reconnection
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
static ICACHE_FLASH_ATTR
void webserver_recon(void *arg, sint8 err)
{
    struct espconn *pesp_conn = arg;

    os_printf("webserver's %d.%d.%d.%d:%d err %d reconnect\n", pesp_conn->proto.tcp->remote_ip[0],
    		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
    		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port, err);
}

/******************************************************************************
 * FunctionName : webserver_recon
 * Description  : the connection has been err, reconnection
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
static ICACHE_FLASH_ATTR
void webserver_discon(void *arg)
{
    struct espconn *pesp_conn = arg;

    os_printf("webserver's %d.%d.%d.%d:%d disconnect\n", pesp_conn->proto.tcp->remote_ip[0],
        		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
        		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port);
}

void ICACHE_FLASH_ATTR webserver_sent(void *arg){
	struct espconn *pesp_conn = arg;

    os_printf("webserver's %d.%d.%d.%d:%d sent\n", pesp_conn->proto.tcp->remote_ip[0],
        		pesp_conn->proto.tcp->remote_ip[1],pesp_conn->proto.tcp->remote_ip[2],
        		pesp_conn->proto.tcp->remote_ip[3],pesp_conn->proto.tcp->remote_port);
}
/******************************************************************************
 * FunctionName : user_accept_listen
 * Description  : server listened a connection successfully
 * Parameters   : arg -- Additional argument to pass to the callback function
 * Returns      : none
*******************************************************************************/
static void ICACHE_FLASH_ATTR
webserver_listen(void *arg)
{
    struct espconn *pesp_conn = arg;

    espconn_regist_recvcb(pesp_conn, webserver_recv);
    espconn_regist_disconcb(pesp_conn, webserver_discon);
    espconn_regist_sentcb(pesp_conn, webserver_sent);
}
