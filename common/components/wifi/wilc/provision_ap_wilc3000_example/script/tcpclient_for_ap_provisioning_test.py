from socket import *
serverName = '192.168.5.1'
serverPort = 80
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName,serverPort))
import time
time.sleep(1)
sentence = "apply,NETGEAR48,2,littleoctopus7" #raw_input('Input lowercase sentence:')
clientSocket.send(sentence)
modifiedSentence = clientSocket.recv(1024)
print 'From Server:', modifiedSentence
clientSocket.close()