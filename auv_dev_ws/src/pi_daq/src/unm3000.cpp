/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:35:07
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-22 12:47:02
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/unm3000.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "unm3000.hpp"

void PubUNM3000ToPiThread(std::shared_ptr<UNM3000> &node, char &frameHeader, char &frameTail, int byteSize);
void SubPiToUNM3000Thread(std::shared_ptr<UNM3000> &node);

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/USB3";
    BaudRate baudRate = BaudRate::B_19200;
    const char pubTopicName[] = "UNM3000_to_Pi";
    const char subTopicName[] = "Pi_to_UNM3000";
    const char nodeName[] = "UNM3000";
    char frameHeader = '@';
    char frameTail = '$';
    int byteSize = 6;

    rclcpp::init(argc, argv);
    auto node = std::make_shared<UNM3000>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    
    RCLCPP_INFO(node->get_logger(), "UNM3000 serial port is opening!");
    RCLCPP_INFO(node->get_logger(), "Device is /dev/USB3");
    RCLCPP_INFO(node->get_logger(), "Baudrate is 19200");

    std::thread unm3000ToPiThreadObj(PubUNM3000ToPiThread, std::ref(node), std::ref(frameHeader), std::ref(frameTail), byteSize);
    std::thread piToUNM3000ThreadObj(SubPiToUNM3000Thread, std::ref(node));

    unm3000ToPiThreadObj.join();
    piToUNM3000ThreadObj.join();

    rclcpp::shutdown();
    exit(0);
}

void PubUNM3000ToPiThread(std::shared_ptr<UNM3000> &node, char &frameHeader, char &frameTail, int byteSize)
{
    while(rclcpp::ok())
        node->PubEvent(frameHeader, frameTail, byteSize);
}

void SubPiToUNM3000Thread(std::shared_ptr<UNM3000> &node)
{
    rclcpp::spin(node);
}