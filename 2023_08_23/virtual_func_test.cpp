#include <iostream>
#include <string>
#include <queue>
#include <memory>

// 设备基类
class Device {
public:
    virtual void sendMessage(const std::string& message) = 0;
};

// 设备A
class DeviceA : public Device {
public:
    void sendMessage(const std::string& message) override {
        std::cout << "DeviceA sends: " << message << std::endl;
    }
};

// 设备B
class DeviceB : public Device {
public:
    void sendMessage(const std::string& message) override {
        std::cout << "DeviceB sends: " << message << std::endl;
    }
};

// 行为树接口
class BehaviorTreeInterface {
public:
    void enqueueMessage(const std::string& message) {
        messageQueue.push(message);
    }
    
    std::string dequeueMessage() {
        if (messageQueue.empty()) {
            return "";  // or you can throw an exception
        }
        std::string msg = messageQueue.front();
        messageQueue.pop();
        return msg;
    }

private:
    std::queue<std::string> messageQueue;
};

// msg_process 类
class MsgProcess {
public:
    MsgProcess(std::shared_ptr<Device> devicePtr, BehaviorTreeInterface& btInterface)
        : device(devicePtr), btInterface(btInterface) {}

    void process() {
        std::string message = btInterface.dequeueMessage();
        if (!message.empty()) {
            device->sendMessage(message);
        }
    }

private:
    std::shared_ptr<Device> device;
    BehaviorTreeInterface& btInterface;
};

int main() {
    DeviceA deviceA;
    DeviceB deviceB;

    BehaviorTreeInterface btInterface;
    btInterface.enqueueMessage("Hello from behavior tree to DeviceA");
    btInterface.enqueueMessage("Hello from behavior tree to DeviceB");

    MsgProcess msgProcessA(std::make_shared<DeviceA>(), btInterface);
    MsgProcess msgProcessB(std::make_shared<DeviceB>(), btInterface);

    msgProcessA.process(); // Processes the message for DeviceA
    msgProcessB.process(); // Processes the message for DeviceB

    return 0;
}
