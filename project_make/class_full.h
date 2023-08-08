#include <string>

class MessageService
{
public:
    MessageService(const std::string& unix_socket_path);
    ~MessageService();

    void StartService();
    void StopService();

private:
    std::string unix_socket_path_;
};

class DataManagement
{
public:
    DataManagement(const std::string& unix_socket_path, const std::string& udp_ip, int udp_port);
    ~DataManagement();

    void StartService();
    void StopService();

private:
    std::string unix_socket_path_;
    std::string udp_ip_;
    int udp_port_;
};

class ProcessManager
{
public:
    ProcessManager();
    ~ProcessManager();

    void StartProcess(const std::string& binaryPath, const std::string& name, const std::string& argument);
    void StopProcess();

private:
    
};

class ExceptionHandler
{
public:
    ExceptionHandler();
    ~ExceptionHandler();

    void HandleException(const std::exception& e);
};


class UnixSocketServer {
public:
    UnixSocketServer(const std::string& socket_path);
    ~UnixSocketServer();

    void StartServer();
    void StopServer();
    std::string ReceiveMessage();
    void SendMessage(const std::string& message);
};

class UnixSocketClient {
public:
    UnixSocketClient(const std::string& socket_path);
    ~UnixSocketClient();

    void Connect();
    void Disconnect();
    std::string ReceiveMessage();
    void SendMessage(const std::string& message);
};

class UdpSocketServer {
public:
    UdpSocketServer(const std::string& ip, int port);
    ~UdpSocketServer();

    void StartServer();
    void StopServer();
    std::string ReceiveMessageAsync();
    void SendMessage(const std::string& message);
};

class UdpSocketClient {
public:
    UdpSocketClient(const std::string& ip, int port);
    ~UdpSocketClient();

    void Connect();
    void Disconnect();
    std::string ReceiveMessageAsync();
    void SendMessage(const std::string& message);
};

class ExceptionHandler {
public:
    ExceptionHandler();
    ~ExceptionHandler();

    void HandleException(const std::exception& e);
};
