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
#include "modules/fota_upgrade.h"
#include "espconn.h"


#define pheadbuffer "Connection: keep-alive\r\n\
Cache-Control: no-cache\r\n\
User-Agent: Mozilla/5.0 (Windows NT 5.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/30.0.1599.101 Safari/537.36 \r\n\
Accept: */*\r\n\
Authorization: token\r\n\
Accept-Encoding: gzip,deflate,sdch\r\n\
Accept-Language: zh-CN,zh;q=0.8\r\n\r\n"


LOCAL struct espconn user_conn;
LOCAL struct _esp_tcp user_tcp;

LOCAL void fota_upgrade_begin(struct espconn *pespconn, struct upgrade_server_info *server);


LOCAL void ICACHE_FLASH_ATTR
tcp_client_sent_cb(void *arg)
{
    struct espconn *pespconn = arg;

    INFO("[IAMLIUBO] tcp client sent cb\n");
}

LOCAL void ICACHE_FLASH_ATTR
tcp_client_recon_cb(void *arg, sint8 err)
{
    struct espconn *pespconn = (struct espconn *)arg;

    INFO("[IAMLIUBO] tcp client recon cb\n");

}


LOCAL void ICACHE_FLASH_ATTR
tcp_client_recv_cb(void *arg, char *pusrdata, unsigned short length)
{

    struct espconn *pespconn = arg;

    INFO("[IAMLIUBO] tcp client recv cb %s\n", pusrdata);
}


LOCAL void ICACHE_FLASH_ATTR
tcp_client_connect_cb(void *arg)
{
    struct espconn *pespconn = arg;
    struct upgrade_server_info *server = NULL;

    INFO("[IAMLIUBO] tcp client connect cb\n");
    if (wifi_get_opmode() ==  STATIONAP_MODE ) {
        wifi_set_opmode(STATION_MODE);
    }

    espconn_regist_recvcb(pespconn, tcp_client_recv_cb);
    espconn_regist_sentcb(pespconn, tcp_client_sent_cb);

    server = (struct upgrade_server_info *)os_zalloc(sizeof(struct upgrade_server_info));

    server->upgrade_version[15] = '\0';

    INFO("[IMALIUBO] fota upgarde start\r\n");
    fota_upgrade_begin(pespconn, server);
}

LOCAL void ICACHE_FLASH_ATTR
fota_upgrade_rsp(void *arg)
{
    struct upgrade_server_info *server = arg;
    struct espconn *pespconn = server->pespconn;

    if (server->upgrade_flag == true) {
        INFO("[IAMLIUBO] fota upgarde successfully\n");
        system_upgrade_reboot();
    } else {
        INFO("[IAMLIUBO] fota upgrade failed\n");
    }

    os_free(server->url);
    server->url = NULL;
    os_free(server);
    server = NULL;
}


LOCAL void ICACHE_FLASH_ATTR
fota_upgrade_begin(struct espconn *pespconn, struct upgrade_server_info *server)
{
    uint8 user_bin[30] = {0};

    server->pespconn = pespconn;

    os_memcpy(server->ip, pespconn->proto.tcp->remote_ip, 4);

    server->port = OTA_SERVER_PORT;


    server->check_cb = fota_upgrade_rsp;
    server->check_times = 120000;

    if (server->url == NULL) {
        server->url = (uint8 *)os_zalloc(512);
    }

    if (system_upgrade_userbin_check() == UPGRADE_FW_BIN1) {
        os_memcpy(user_bin, "user2.4096.new.6.bin", 21);
    } else if (system_upgrade_userbin_check() == UPGRADE_FW_BIN2) {
        os_memcpy(user_bin, "user1.4096.new.6.bin", 21);
    }

    os_sprintf(server->url, "GET /api/download_firmware?firmware_bin=%s HTTP/1.0\r\nHost: "IPSTR":%d\r\n"pheadbuffer"",
               user_bin,IP2STR(server->ip));
    
    INFO("[IAMLIUBO] %s\n",server->url);


    if (system_upgrade_start(server) == false) {
        INFO("[IAMLIUBO] upgrade is already started\n");
    }
}


void ICACHE_FLASH_ATTR
fota_init(void)
{
    user_conn.proto.tcp = &user_tcp;
    user_conn.type = ESPCONN_TCP;
    user_conn.state = ESPCONN_NONE;

    uint32 ota_server_ip = ipaddr_addr(OTA_SERVER_IP);
    
    os_memcpy(user_conn.proto.tcp->remote_ip, &ota_server_ip, 4);
    user_conn.proto.tcp->local_port = espconn_port();

    user_conn.proto.tcp->remote_port = OTA_SERVER_PORT;

    espconn_regist_connectcb(&user_conn, tcp_client_connect_cb);
    espconn_regist_reconcb(&user_conn, tcp_client_recon_cb);

    espconn_connect(&user_conn);
}
