/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:35:42
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-27 21:05:19
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/p360.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"

using namespace SIGMA::AUV100;

class P360 : public DaqABC
{
private:
    virtual void Init(const std::string, BaudRate);
public:
    P360(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual void PubEvent();
    virtual ~P360();
};

P360::P360(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void P360::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();

    RCLCPP_INFO(this->get_logger(), "P360 serial port is opening!");
    RCLCPP_INFO(this->get_logger(), "Device is /dev/USB3");
    RCLCPP_INFO(this->get_logger(), "Baudrate is 115200");
}

void P360::PubEvent()
{
    interfaces::msg::Uint8Array data;
    while(!SerialAvailable());  //串口无数据时无限循环
    SerialReadBinary(data.data);
    RCLCPP_INFO(this->get_logger(), "have");
    if(data.data[0] == 0x42 && data.data[1] == 0x52)
    {
        while(data.data.size() != 1224)
        {
            std::vector<uint8_t> tempdata;
            SerialReadBinary(tempdata);
            data.data.insert(data.data.end(),tempdata.begin(),tempdata.end());
        }
        Publish(data);
    }
}

P360::~P360()
{
    RCLCPP_INFO(this->get_logger(), "P360 serial port is over!");
}