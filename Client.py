import socket
import sys

PORT = 8888
IP = '127.0.0.1'

def main():
    # Validate port range
    if not (1024 <= PORT <= 65535):
        print(f"Error: Invalid port {PORT}")
        sys.exit(1)

    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        client_socket.connect((IP, PORT))

        client_socket.sendall(b"Hello")

        response = client_socket.recv(5).decode()

        print(f"Server responded: {response}")

        client_socket.close()

    except Exception as e:
        print(f"Client error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
