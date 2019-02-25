from socket import *
from time import ctime
 
ADDR = ('192.168.0.109',6666)
tcpServer = socket(AF_INET,SOCK_STREAM)

i = 0

tcpServer.bind(ADDR)
tcpServer.listen(1000)

while True:
    print('waiting for connection ...')
    client, addr = tcpServer.accept()
    print('Got connection from',addr)
    while True:
        recvData = client.recv(1024).decode('utf-8')
        if not recvData:
            break
        print(recvData)
        i = i + 1
        client.send(("hi this is PC server! message num " + str(i)).encode())
        
    client.close()
    
tcpServer.close()
