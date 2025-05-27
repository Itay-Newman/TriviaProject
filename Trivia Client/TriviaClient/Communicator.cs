using System;
using System.Net.Sockets;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class ClientCommunicator
    {
        private static ClientCommunicator _instance;

        private TcpClient _client;

        private NetworkStream _stream;

        private const string SERVER_IP = "127.0.0.1";

        private const int SERVER_PORT = 8888;

        // This is a "singleton" for ClientCommunicator, if _instance is null, it creates a new ClientCommunicator and assigns it. else, it just returns the existing instance. plus, it makes sure only ONE instance ever exists across the program (singleton).
        public static ClientCommunicator Instance => _instance ??= new ClientCommunicator();

        private ClientCommunicator() { }

        public bool IsConnected => _client != null && _client.Connected;

        // async = "Asynchronously"
        // So here the asynchronously connects to the server for example...
        public async Task<bool> ConnectAsync()
        {
            try
            {             
                if (IsConnected)
                    return true;

                _client = new TcpClient();
                await _client.ConnectAsync(SERVER_IP, SERVER_PORT);
               
                _stream = _client.GetStream();
                return true;
            }
            
            catch (Exception ex)
            {
                Console.WriteLine($"[ERROR] ConnectAsync failed: {ex.Message}");
                return false;
            }
        }

        // async sends a request to the server
        public async Task SendRequestAsync(byte messageCode, byte[] messageBody)
        {
            if (_stream == null || !_client.Connected)
                throw new Exception("Not connected to the server.");

            // Converting the length of the message body to a 4-byte array
            byte[] sizeBytes = BitConverter.GetBytes((uint)messageBody.Length);

            // Reversing the byte array if the system architecture is "little-endian" (hehe)
            if (BitConverter.IsLittleEndian)
                Array.Reverse(sizeBytes);

            byte[] finalBuffer = new byte[1 + 4 + messageBody.Length];
            finalBuffer[0] = messageCode;
            Array.Copy(sizeBytes, 0, finalBuffer, 1, 4);
            Array.Copy(messageBody, 0, finalBuffer, 5, messageBody.Length);

            // Write the buffer to the network stream
            await _stream.WriteAsync(finalBuffer, 0, finalBuffer.Length);
        }

        // Async reads a response from the server
        public async Task<(byte messageCode, byte[] messageBody)> ReadResponseAsync()
        {
            if (_stream == null || !_client.Connected)
                throw new Exception("Not connected to the server.");

            byte[] codeBuffer = await ReadExactAsync(1);
            byte messageCode = codeBuffer[0];

            byte[] sizeBuffer = await ReadExactAsync(4);

            // Reversing the byte array if the system architecture is little-endian just like i did before...
            if (BitConverter.IsLittleEndian)
                Array.Reverse(sizeBuffer);

            // Converting the size bytes to an unsigned integer
            uint messageSize = BitConverter.ToUInt32(sizeBuffer, 0);

            // Reading the message body based on the size
            byte[] messageBuffer = await ReadExactAsync((int)messageSize);

            return (messageCode, messageBuffer);
        }

        // Helper method to read an exact number of bytes from the stream
        private async Task<byte[]> ReadExactAsync(int length)
        {
            byte[] buffer = new byte[length];
            int totalRead = 0;

            while (totalRead < length)
            {
                int bytesRead = await _stream.ReadAsync(buffer, totalRead, length - totalRead);
                if (bytesRead == 0)
                    throw new Exception("Connection closed by server.");
                totalRead += bytesRead;
            }

            return buffer;
        }

        public void Disconnect()
        {
            try
            {
                // Reseting the _client and _stream
                _stream?.Close();
                _client?.Close();

                _client = null;
                _stream = null;
            }
            catch (Exception ex)
            {
                Console.WriteLine($"[ERROR] Disconnect failed: {ex.Message}");
            }
        }
    }
}