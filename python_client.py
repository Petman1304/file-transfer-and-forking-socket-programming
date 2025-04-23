import socket

if __name__ == '__main__':
    host = 'enter host ip'
    port = 3000

    clientSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    clientSocket.connect((host, port))

    while True:
        filename = input()
        try:
            fp = open(filename, "r")
            data = fp.read()
            if not data:
                break
            while data:
                clientSocket.send(str(data).encode())
                data = fp.read()
            fp.close()
            clientSocket.close()
            exit()

        except IOError:
            print("Invalid filename")