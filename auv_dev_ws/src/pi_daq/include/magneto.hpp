/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:09:35
 * @LastEditors: zzhef
 * @LastEditTime: 2023-08-29 14:41:07
 * @FilePath: \pi_daq\include\magneto.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"
#include <bcm2835.h>
#include <stdio.h>

#define PIN11 RPI_BPLUS_GPIO_J8_11  //物理编号
using namespace SIGMA::AUV100;

class Magneto : public DaqABC
{
private:
    virtual void CallBack(interfaces::msg::Uint8Array::SharedPtr);
    virtual void Init(const std::string, BaudRate);
        
    // rclcpp::TimerBase::SharedPtr pubTimer_;
    // void Pub100msTimerCallback();
public:
    Magneto(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~Magneto();
    //virtual void PubEvent();
};

Magneto::Magneto(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
    // pubTimer_ = this->create_wall_timer(2ms, std::bind(&Magneto::Pub100msTimerCallback, this)); //1ms = 1000us

}

// void Magneto::Pub100msTimerCallback() 
// {
//     bcm2835_gpio_write(PIN11, LOW);
//     usleep(200);
//     PubEvent('F', '\n', 24);
//     bcm2835_gpio_write(PIN11, HIGH);
// }

void Magneto::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();

    bcm2835_init();
    bcm2835_gpio_fsel(PIN11, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(PIN11, HIGH);
    usleep(10000);
    RCLCPP_INFO(this->get_logger(), "Magneto serial port is opening!");
    RCLCPP_INFO(this->get_logger(), "Device is /dev/ttyAMA0");
    //RCLCPP_INFO(this->get_logger(), "Baudrate is 9600");
    RCLCPP_INFO(this->get_logger(), "Baudrate is 230400");

    RCLCPP_INFO(this->get_logger(), "Magneto is waiting for 'start'...");
    std::vector<uint8_t> startUint8Vec{'s', 't', 'a', 'r', 't'};
    SerialWriteBinary(startUint8Vec);
    usleep(10000);
    bcm2835_gpio_write(PIN11, LOW);

    RCLCPP_INFO(this->get_logger(), "'start' over");
}

void Magneto::CallBack(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    bcm2835_gpio_write(PIN11, HIGH);
    usleep(10000);
    
    SerialWriteBinary(dataPtr->data);

std::ostringstream ostr;
ostr.fill('0');        
ostr << "Magneto sub data:";
for(int i = 0; i < static_cast<int>(dataPtr->data.size()); i++){
    if(i != 0) ostr << " ";
    ostr << "0X" << std::hex << std::uppercase << std::setw(2) << int(dataPtr->data[i]);
}
RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 

    usleep(10000);

    bcm2835_gpio_write(PIN11, LOW);
}

Magneto::~Magneto()
{
    bcm2835_gpio_write(PIN11, HIGH);
    RCLCPP_INFO(this->get_logger(), "Magneto serial port is over!");
}