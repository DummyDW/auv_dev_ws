#include "camera.hpp"

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/ttyAMA2";
    BaudRate baudRate = BaudRate::B_9600;
    const char pubTopicName[] = "Camera_to_Pi";
    const char subTopicName[] = "Pi_to_Camera";
    const char nodeName[] = "Camera";
    char frameHeader = '$';
    char frameTail = '\n';

    rclcpp::init(argc, argv);
    auto node = std::make_shared<Camera>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    while(rclcpp::ok())
    {
        node->PubEvent(frameHeader, frameTail);
        //rclcpp::spin_some(node);
    }
    
    rclcpp::shutdown();
    exit(0);
}