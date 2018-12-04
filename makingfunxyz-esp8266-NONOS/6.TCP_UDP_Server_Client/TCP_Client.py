from socket import *
 
TCP_SERVER_HOST = '127.0.0.1'
TCP_SERVER_PORT = 8888
TCP_SERVER_ADDR = (TCP_SERVER_HOST,TCP_SERVER_PORT)
 
tcpClient = socket(AF_INET,SOCK_STREAM)
tcpClient.connect(TCP_SERVER_ADDR)
 
while True:
    sendData = input('> ')
    if not sendData:
        break
    tcpClient.send(sendData.encode('utf-8'))
    recvData = tcpClient.recv(1024).decode('utf-8')
    if not recvData:
        break
    print(recvData)
    
tcpClient.close()
