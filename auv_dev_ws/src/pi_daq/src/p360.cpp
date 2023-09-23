/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:35:49
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-22 10:36:34
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/p360.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "p360.hpp"

int main(int argc, char **argv)
{
    const std::string deviceName = "/dev/USB3";
    BaudRate baudRate = BaudRate::B_115200;
    const char pubTopicName[] = "P360_to_Pi";
    const char subTopicName[] = "Pi_to_P360";
    const char nodeName[] = "P360";

    rclcpp::init(argc, argv);
    auto node = std::make_shared<P360>(deviceName, baudRate, pubTopicName, subTopicName, nodeName);
    while(rclcpp::ok())
    {
        node->PubEvent();
        rclcpp::spin_some(node);
    }

    rclcpp::shutdown();
    exit(0);
}