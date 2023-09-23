/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:36:00
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-04-28 02:14:39
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/stm32.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"

using namespace SIGMA::AUV100;

class Stm32 : public DaqABC
{
private:
    virtual void Init(const std::string, BaudRate);
public:
    Stm32(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~Stm32();
    //virtual void PubEvent(char, char);
};

Stm32::Stm32(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void Stm32::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    //SerialSetTimeout(0);  //默认-1阻塞，0非阻塞
    SerialOpen();
}

Stm32::~Stm32()
{
    RCLCPP_INFO(this->get_logger(), "Stm32 serial port is over!");
}