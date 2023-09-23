/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:34:41
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-22 08:43:23
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/bdgps.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "bdgps.hpp"

void PubBDGPSToPiThread(std::shared_ptr<BDGPS> &node);
void SubPiToBDGPSThread(std::shared_ptr<BDGPS> &node);

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/USB0";
    BaudRate baudRate = BaudRate::B_19200;
    const char pubTopicName[] = "BDGPS_to_Pi";
    const char subTopicName[] = "Pi_to_BDGPS";
    const char nodeName[] = "BDGPS";

    rclcpp::init(argc, argv);
    auto node = std::make_shared<BDGPS>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    
    std::thread bDGPSToPiThreadObj(PubBDGPSToPiThread, std::ref(node));
    std::thread piToBdgpsThreadObj(SubPiToBDGPSThread, std::ref(node));

    bDGPSToPiThreadObj.join();
    piToBdgpsThreadObj.join();

    rclcpp::shutdown();
    exit(0);
}

void PubBDGPSToPiThread(std::shared_ptr<BDGPS> &node)
{
    while(rclcpp::ok())
        node->PubEvent();
}

void SubPiToBDGPSThread(std::shared_ptr<BDGPS> &node)
{
    rclcpp::spin(node);
}