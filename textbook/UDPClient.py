from socket import *
serverName = 'peace.handong.edu'
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
while True:
    message = input('input lowercase sentence:')
    if message == 'q': break
    clientSocket.sendto(message.encode(), (serverName, serverPort))
    print('success to send a message')
    modifiedMessage, serverAddress = clientSocket.recvfrom(2048)
    print(modifiedMessage.decode())
clientSocket.close()