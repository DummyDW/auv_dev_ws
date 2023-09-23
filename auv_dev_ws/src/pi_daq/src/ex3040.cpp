/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:35:29
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-27 20:51:36
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/ex3040.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "ex3040.hpp"

void PubEX3040ToPiThread(std::shared_ptr<EX3040> &node, char &frameHeader, char &frameTail);
void SubPiToEX3040Thread(std::shared_ptr<EX3040> &node);

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/USB1";
    BaudRate baudRate = BaudRate::B_460800;
    const char pubTopicName[] = "EX3040_to_Pi";
    const char subTopicName[] = "Pi_to_EX3040";
    const char nodeName[] = "EX3040";
    char frameHeader = '$';
    char frameTail = '\n';

    rclcpp::init(argc, argv);
    auto node = std::make_shared<EX3040>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    RCLCPP_INFO(node->get_logger(), "EX3040 serial port is opening!");
    RCLCPP_INFO(node->get_logger(), "Device is /dev/USB1");
    RCLCPP_INFO(node->get_logger(), "Baudrate is 460800");

    std::thread ex3040ToPiThreadObj(PubEX3040ToPiThread, std::ref(node), std::ref(frameHeader), std::ref(frameTail));
    std::thread piToEX3040ThreadObj(SubPiToEX3040Thread, std::ref(node));

    ex3040ToPiThreadObj.join();
    piToEX3040ThreadObj.join();

    rclcpp::shutdown();
    exit(0);
}

void PubEX3040ToPiThread(std::shared_ptr<EX3040> &node, char &frameHeader, char &frameTail)
{
    while(rclcpp::ok())
        node->PubEvent(frameHeader, frameTail);
}

void SubPiToEX3040Thread(std::shared_ptr<EX3040> &node)
{
    rclcpp::spin(node);
}