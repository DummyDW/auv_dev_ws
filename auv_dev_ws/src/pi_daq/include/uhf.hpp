/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:36:17
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-05-17 19:24:04
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/uhf.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"

using namespace SIGMA::AUV100;

class UHF : public DaqABC
{
private:
    virtual void Init(const std::string, BaudRate);
public:
    UHF(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~UHF();
};

UHF::UHF(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void UHF::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();
}

UHF::~UHF()
{
    RCLCPP_INFO(this->get_logger(), "UHF serial port is over!");
}