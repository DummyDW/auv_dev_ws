/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 02:20:45
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-05-25 19:01:31
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/magneto.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "magneto.hpp"

void PubMagnetoToPiThread(std::shared_ptr<Magneto> &node, char &frameHeader, char &frameTail);
void SubPiToMagnetoThread(std::shared_ptr<Magneto> &node);

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/ttyAMA0";
    //BaudRate baudRate = BaudRate::B_9600;
    BaudRate baudRate = BaudRate::B_230400;
    const char pubTopicName[] = "Magneto_to_Pi";
    const char subTopicName[] = "Pi_to_Magneto";
    const char nodeName[] = "Magneto";
    // char frameHeader = '2';
    // char frameTail = '\n';
    char frameHeader = 'F';
    char frameTail = '\n';

    rclcpp::init(argc, argv);
    auto node = std::make_shared<Magneto>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);

    std::thread magnetoToPiThreadObj(PubMagnetoToPiThread, std::ref(node), std::ref(frameHeader), std::ref(frameTail));
    std::thread pitoMagnetoThreadObj(SubPiToMagnetoThread, std::ref(node));

    magnetoToPiThreadObj.join();
    pitoMagnetoThreadObj.join();

    rclcpp::shutdown();
    exit(0);
}

void PubMagnetoToPiThread(std::shared_ptr<Magneto> &node, char &frameHeader, char &frameTail)
{
    while(rclcpp::ok())
        node->PubEvent(frameHeader, frameTail, 24);
    //node->PubEvent(frameHeader, frameTail, 73);
}

void SubPiToMagnetoThread(std::shared_ptr<Magneto> &node)
{
    rclcpp::spin(node);
}