from socket import *
from time import ctime
 
ADDR = ('localhost',8888)
tcpServer = socket(AF_INET,SOCK_STREAM)
 
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
        sendData = input('>')
        if not sendData:
            break
        client.send(sendData.encode('utf-8'))
        
    client.close()
    
tcpServer.close()
