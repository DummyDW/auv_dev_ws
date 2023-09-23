/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:36:08
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-29 12:08:50
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/stm32.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "stm32.hpp"

void PubSTM32ToPiThread(std::shared_ptr<Stm32> &node, char &frameHeader, char &frameTail);
void SubPiToSTM32Thread(std::shared_ptr<Stm32> &node);

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/ttyAMA1";
    BaudRate baudRate = BaudRate::B_115200;
    const char pubTopicName[] = "STM32_to_Pi";
    const char subTopicName[] = "Pi_to_STM32";
    const char nodeName[] = "STM32";
    char frameHeader = '@';
    char frameTail = '$';

    rclcpp::init(argc, argv);
    auto node = std::make_shared<Stm32>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    RCLCPP_INFO(node->get_logger(), "Stm32 serial port is opening!");
    RCLCPP_INFO(node->get_logger(), "Device is /dev/ttyAMA1");
    RCLCPP_INFO(node->get_logger(), "Baudrate is 115200");
    
    std::thread stm32ToPiThreadObj(PubSTM32ToPiThread, std::ref(node), std::ref(frameHeader), std::ref(frameTail));
    std::thread piToStm32ThreadObj(SubPiToSTM32Thread, std::ref(node));

    stm32ToPiThreadObj.join();
    piToStm32ThreadObj.join();

    rclcpp::shutdown();
    exit(0);
}

void PubSTM32ToPiThread(std::shared_ptr<Stm32> &node, char &frameHeader, char &frameTail)
{
    while(rclcpp::ok())
        node->PubEvent(frameHeader, frameTail);
}

void SubPiToSTM32Thread(std::shared_ptr<Stm32> &node)
{
    rclcpp::spin(node);
}