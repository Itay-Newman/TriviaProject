import socket
import sys
import json
import struct
import time

PORT = 8888
IP = '127.0.0.1'

LOGIN_REQUEST = 20
SIGNUP_REQUEST = 21

def connect_to_server(server_ip, server_port):
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((server_ip, server_port))
        print(f"Connected to server at {server_ip}:{server_port}")
        return client_socket
    except Exception as e:
        print(f"Connection error: {e}")
        return None


def disconnect(client_socket):
    if client_socket:
        client_socket.close()


def send_request(client_socket, request_code, data):
    if not client_socket:
        print("Not connected to server")
        return None

    try:
        # Convert data to JSON
        json_data = json.dumps(data).encode()

        # Create message with proper structure:
        # Message code (1 byte)
        message = bytes([request_code])

        # Message size (4 bytes)
        message += struct.pack(">I", len(json_data))

        # The JSON data itself
        message += json_data

        # Send the message
        client_socket.sendall(message)

        # Receive and parse the response
        return receive_response(client_socket)

    except Exception as e:
        print(f"Error sending request: {e}")
        return None

def receive_response(client_socket):
    """Receive and parse server response"""
    try:
        # Read message code (1 byte)
        code_buffer = client_socket.recv(1)
        if not code_buffer:
            print("Server closed connection")
            return None

        message_code = code_buffer[0]

        # Read message size (4 bytes)
        size_buffer = client_socket.recv(4)
        if len(size_buffer) < 4:
            print("Error reading message size")
            return None

        # Unpack the size
        message_size = struct.unpack(">I", size_buffer)[0]

        # Read the message data
        data = b""
        bytes_received = 0

        while bytes_received < message_size:
            chunk = client_socket.recv(message_size - bytes_received)
            if not chunk:
                break
            data += chunk
            bytes_received += len(chunk)

        # Parse JSON response
        if data:
            response_data = json.loads(data.decode())
            return {"code": message_code, "data": response_data}
        else:
            print("Empty response data")
            return None

    except Exception as e:
        print(f"Error receiving response: {e}")
        return None

def login(client_socket, username, password):
    login_data = {
        "username": username,
        "password": password
    }

    print(f"\nSending login request for user: {username}")
    response = send_request(client_socket, LOGIN_REQUEST, login_data)

    if response:
        print(f"Login response received, code: {response['code']}")
        print(f"Response data: {response['data']}")

        # Check status in response
        if 'status' in response['data']:
            status = response['data']['status']
            if status == 0:
                print("Login successful!")
            else:
                print(f"Login failed with status code: {status}")
                if 'message' in response['data']:
                    print(f"Error message: {response['data']['message']}")

    return response

def signup(client_socket, username, password, email):
    signup_data = {
        "username": username,
        "password": password,
        "email": email
    }

    print(f"\nSending signup request for user: {username}")
    response = send_request(client_socket, SIGNUP_REQUEST, signup_data)

    if response:
        print(f"Signup response received, code: {response['code']}")
        print(f"Response data: {response['data']}")

        if 'status' in response['data']:
            status = response['data']['status']
            if status == 0:
                print("Signup successful!")
            else:
                print(f"Signup failed with status code: {status}")
                if 'message' in response['data']:
                    print(f"Error message: {response['data']['message']}")

    return response

def display_menu():
    print("\n===== Trivia Game Client =====")
    print("1. Login")
    print("2. Signup")
    print("3. Run Test Suite")
    print("4. Exit")
    return input("Choose an option (1-4): ")

def run_test(client_socket):
    """Running a series of tests to check the server functionality"""
    print("\n===== Running Tests =====")

    print("\nTest 1: Login with non-existent user")
    test_username = "nonexistentuser"
    test_password = "password123"
    response = login(client_socket, test_username, test_password)
    if response and 'status' in response['data'] and response['data']['status'] != 0:
        print("Test passed: Server correctly rejected non-existent user")
    else:
        print("Test failed: Server should reject non-existent user")

    print("\nTest 2: Register a new user")
    test_username = f"testuser_{int(time.time())}"
    test_password = "password123"
    test_email = f"{test_username}@example.com"
    response = signup(client_socket, test_username, test_password, test_email)
    if response and 'status' in response['data'] and response['data']['status'] == 0:
        print(f"Test passed: User {test_username} registered successfully")
    else:
        print("Test failed: User registration should succeed")

    print("\nTest 3: Register duplicate user")
    response = signup(client_socket, test_username, test_password, test_email)
    if response and 'status' in response['data'] and response['data']['status'] != 0:
        print("Test passed: Server correctly rejected duplicate username")
    else:
        print("Test failed: Server should reject duplicate username registration")

    print("\nTest Case 4: Login with newly created user")
    response = login(client_socket, test_username, test_password)
    if response and 'status' in response['data'] and response['data']['status'] == 0:
        print("Test passed: User logged in successfully")
    else:
        print("Test failed: User login should succeed")

    print("\nTest 5: Login when already logged in")
    response = login(client_socket, test_username, test_password)

    if response:
        print("Test completed: Check if your system allows multiple logins for the same user")

    print("\nTest 6: Test username validation")
    invalid_username = "u"  # Too short username
    response = signup(client_socket, invalid_username, test_password, "short@example.com")
    print("Check if the server validates username length and format")

    print("\nTests completed!")

def main():
    if not (1024 <= PORT <= 65535):
        print(f"Error: Invalid port {PORT}")
        sys.exit(1)

    try:
        client_socket = connect_to_server(IP, PORT)

        if not client_socket:
            sys.exit(1)

        while True:
            choice = display_menu()

            if choice == '1':  # Login
                username = input("Enter username: ")
                password = input("Enter password: ")
                login(client_socket, username, password)

            elif choice == '2':  # Signup
                username = input("Enter username: ")
                password = input("Enter password: ")
                email = input("Enter email: ")
                signup(client_socket, username, password, email)

            elif choice == '3':  # Run Tests
                run_test(client_socket)

            elif choice == '4':  # Exit
                print("Exiting client...")
                break

            else:
                print("Invalid option. Please try again.")

        disconnect(client_socket)
        print("Disconnected from server")

    except KeyboardInterrupt:
        print("\nClient terminated by user")
    except Exception as e:
        print(f"Client error: {e}")
    finally:
        if 'client_socket' in locals() and client_socket:
            disconnect(client_socket)

if __name__ == "__main__":
    main()