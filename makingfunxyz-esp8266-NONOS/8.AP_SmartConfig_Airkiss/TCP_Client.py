from socket import *
 
TCP_SERVER_HOST = '192.168.4.1'
TCP_SERVER_PORT = 6666
TCP_SERVER_ADDR = (TCP_SERVER_HOST,TCP_SERVER_PORT)

i = 1

tcpClient = socket(AF_INET,SOCK_STREAM)
tcpClient.connect(TCP_SERVER_ADDR)
 
while i:
    i = i - 1
    tcpClient.send(("{\"SSID\":\"CMCC-Master\",\"PWD\":\"zxcvbnmlk\",\"Author\":\"IAMLIUBO\",\"zhuanlan\":\"zhuanlan.zhihu.com/imliubo-magic-IoT-Tutorial\"}").encode())
    
tcpClient.close()

