/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:34:32
 * @LastEditors: zzhef
 * @LastEditTime: 2023-08-29 14:45:04
 * @FilePath: \pi_daq\include\bdgps.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"

using namespace SIGMA::AUV100;

class BDGPS : public DaqABC
{
private:
    virtual void Init(const std::string, BaudRate);
public:
    virtual void PubEvent();    //帧尾为校验和，所以重写虚函数
    BDGPS(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~BDGPS();
};

BDGPS::BDGPS(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void BDGPS::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();

    RCLCPP_INFO(this->get_logger(), "BDGPS and pi serial port is opening!");
    RCLCPP_INFO(this->get_logger(), "Device is /dev/USB0");
    RCLCPP_INFO(this->get_logger(), "Baudrate is 19200");
}

//"BDGPS_to_Pi"
void BDGPS::PubEvent()
{
    if(SerialAvailable()){
        interfaces::msg::Uint8Array data;

        SerialReadBinary(data.data);    //不知道为什么一开始一次只能接收到一个字节。可能是波特率只有19200
        
        rclcpp::Time now = this->now();
        GetNowDur() = this->now()-now;

        if(data.data[0] == 0x24){
            while(data.data.size() != 39 && (GetNowDur() < GetDurTime())){
                std::vector<uint8_t> tempdata;
                SerialReadBinary(tempdata);
                data.data.insert(data.data.end(), tempdata.begin(), tempdata.end());
                GetNowDur() = this->now()-now;
            }
            Publish(data);    //"BDGPS_to_Pi"把时间发给pi更新
        }
    }
}

BDGPS::~BDGPS()
{
    RCLCPP_INFO(this->get_logger(), "BDGPS serial port is over!");
}