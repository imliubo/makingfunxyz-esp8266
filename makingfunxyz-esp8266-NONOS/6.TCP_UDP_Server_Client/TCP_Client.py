from socket import *
 
TCP_SERVER_HOST = '192.168.0.107'
TCP_SERVER_PORT = 6666
TCP_SERVER_ADDR = (TCP_SERVER_HOST,TCP_SERVER_PORT)

i = 0

tcpClient = socket(AF_INET,SOCK_STREAM)
tcpClient.connect(TCP_SERVER_ADDR)
 
while True:
    recvData = tcpClient.recv(1024).decode('utf-8')
    if not recvData:
        break
    print(recvData)
    i = i + 1
    tcpClient.send(("hi this is PC client! message num " + str(i)).encode())
    
tcpClient.close()

#这里使用IDLE打开，修改完IP后，保存，可以直接按F5运行。
