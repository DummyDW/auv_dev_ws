/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:36:22
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-06-07 15:50:07
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/uhf.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "uhf.hpp"

void PubUHFToPiThread(std::shared_ptr<UHF> &node, char &frameHeader, char &frameTail, int byteSize);
void SubPiToUHFThread(std::shared_ptr<UHF> &node);

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/USB2";
    BaudRate baudRate = BaudRate::B_19200;
    const char pubTopicName[] = "Shore_to_Pi";
    const char subTopicName[] = "Pi_to_Shore";
    const char nodeName[] = "UHF";
    char frameHeader = '@';
    char frameTail = '$';
    int byteSize = 6;

    rclcpp::init(argc, argv);
    auto node = std::make_shared<UHF>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    RCLCPP_INFO(node->get_logger(), "UHF serial port is opening!");
    RCLCPP_INFO(node->get_logger(), "Device is /dev/USB2");
    RCLCPP_INFO(node->get_logger(), "Baudrate is 19200");

    std::thread uhfToPiThreadObj(PubUHFToPiThread, std::ref(node), std::ref(frameHeader), std::ref(frameTail), byteSize);
    std::thread piToUHFThreadObj(SubPiToUHFThread, std::ref(node));

    uhfToPiThreadObj.join();
    piToUHFThreadObj.join();

    rclcpp::shutdown();
    exit(0);
}

void PubUHFToPiThread(std::shared_ptr<UHF> &node, char &frameHeader, char &frameTail, int byteSize)
{
    while(rclcpp::ok())
        node->PubEvent(frameHeader, frameTail, byteSize);
}

void SubPiToUHFThread(std::shared_ptr<UHF> &node)
{
    rclcpp::spin(node);
}