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

        public static ClientCommunicator Instance => _instance ??= new ClientCommunicator();

        private ClientCommunicator() { }

        public bool IsConnected => _client != null && _client.Connected;

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

        public async Task SendRequestAsync(byte messageCode, byte[] messageBody)
        {
            if (_stream == null || !_client.Connected)
                throw new Exception("Not connected to the server.");

            byte[] sizeBytes = BitConverter.GetBytes((uint)messageBody.Length);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(sizeBytes);

            byte[] finalBuffer = new byte[1 + 4 + messageBody.Length];
            finalBuffer[0] = messageCode;
            Array.Copy(sizeBytes, 0, finalBuffer, 1, 4);
            Array.Copy(messageBody, 0, finalBuffer, 5, messageBody.Length);

            await _stream.WriteAsync(finalBuffer, 0, finalBuffer.Length);
        }

        public async Task<(byte messageCode, byte[] messageBody)> ReadResponseAsync()
        {
            if (_stream == null || !_client.Connected)
                throw new Exception("Not connected to the server.");

            byte[] codeBuffer = await ReadExactAsync(1);
            byte messageCode = codeBuffer[0];

            byte[] sizeBuffer = await ReadExactAsync(4);
            if (BitConverter.IsLittleEndian)
                Array.Reverse(sizeBuffer);

            uint messageSize = BitConverter.ToUInt32(sizeBuffer, 0);
            byte[] messageBuffer = await ReadExactAsync((int)messageSize);

            return (messageCode, messageBuffer);
        }

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