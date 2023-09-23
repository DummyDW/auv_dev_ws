/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:34:59
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-29 12:23:43
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/unm3000.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"

using namespace SIGMA::AUV100;

class UNM3000 : public DaqABC
{
private:
    virtual void Init(const std::string, BaudRate);
public:
    UNM3000(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~UNM3000();
};

UNM3000::UNM3000(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void UNM3000::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();
}


UNM3000::~UNM3000()
{
    RCLCPP_INFO(this->get_logger(), "UNM3000 serial port is over!");
}