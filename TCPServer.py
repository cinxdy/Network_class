from socket import *
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(('',serverPort))
serverSocket.listen(1)
print("The server is ready to receive")
while True:
    connectionSocket, clientAddress = serverSocket.accept()
    message = serverSocket.recvfrom(2048)
    modifiedMessage = message.decode().upper()
    serverSocket.sends(modifiedMessage.encode())
    connectionSocket.close()