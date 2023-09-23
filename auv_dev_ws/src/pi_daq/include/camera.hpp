/*
 * @Author       : zzhef
 * @Date         : 2023-05-08 10:16:17
 * @LastEditors: zzhef
 * @LastEditTime: 2023-08-29 14:41:34
 * @FilePath: \pi_daq\include\camera.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"
#include <bcm2835.h>

#define PIN12 RPI_BPLUS_GPIO_J8_12  //物理编号
using namespace SIGMA::AUV100;

class Camera : public DaqABC
{
private:
    //virtual void CallBack(interfaces::msg::Uint8Array::SharedPtr);
    virtual void Init(const std::string, BaudRate);
public:
    Camera(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~Camera();
    virtual void PubEvent(char frameHeader, char frameTail);
};

Camera::Camera(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void Camera::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();

    bcm2835_init();
    bcm2835_gpio_fsel(PIN12, BCM2835_GPIO_FSEL_OUTP);

    RCLCPP_INFO(this->get_logger(), "Camera serial port is opening!");
    RCLCPP_INFO(this->get_logger(), "Device is /dev/ttyAMA2");
    RCLCPP_INFO(this->get_logger(), "Baudrate is 9600");
    
    bcm2835_gpio_write(PIN12, LOW);
}

// void Camera::CallBack(interfaces::msg::Uint8Array::SharedPtr dataPtr)
// {

// }

//"Camera_to_Pi"
void Camera::PubEvent(char frameHeader, char frameTail)
{
    //RCLCPP_INFO(this->get_logger(), "enter"); 

    using namespace std;
    if(SerialAvailable())
    {
        //RCLCPP_INFO(this->get_logger(), "have"); 

        interfaces::msg::Uint8Array data;
        vector<uint8_t> tempdata;
        SerialReadBinary(tempdata);//读取串口数据，但一次可能读不完
        data.data.insert(data.data.end(), tempdata.begin(), tempdata.end());//读出的数据追加到data后
        //处理帧头有干扰数据的情况：123F123456...
        if(data.data.front() != frameHeader)
        {
            auto iter = find(data.data.begin(), data.data.end(), frameHeader);
            data.data.erase(data.data.begin(), iter);
        }
        //处理带帧头数据：F123456...
        if(data.data.front() == frameHeader)
        {
            rclcpp::Time now = this->now();
            //循环找帧尾
            while((data.data.back()!= frameTail)&&((this->now()-now)<GetDurTime()))
            {
                tempdata.clear();
                SerialReadBinary(tempdata);
                data.data.insert(data.data.end(),tempdata.begin(),tempdata.end());
            }
            if(data.data.back()!= frameTail)
            {
                auto iter = find(data.data.begin(), data.data.end(), frameTail);
                if(iter != data.data.end())
                    data.data.erase(iter+1, data.data.end());
            }

            while((!data.data.empty())&&data.data.back()== frameTail)
            {
                auto iterbegin1 = find(data.data.begin(), data.data.end(), frameHeader);
                auto iterbegin2 = find(iterbegin1+1, data.data.end(), frameHeader);
                auto iterend = find(data.data.begin(), data.data.end(), frameTail);

                if(iterbegin1 < iterend && iterbegin2 < iterend)
                    data.data.erase(data.data.begin(), iterbegin2);
                else if(iterbegin1 < iterend && iterbegin2 > iterend)
                {
                    auto partdata = interfaces::msg::Uint8Array();
                    partdata.data.assign(iterbegin1, iterend+1);
                    Publish(partdata);
// /********打印接收到的数据********/
std::ostringstream ostr;
ostr << "Camera to Pi:";
for(int i = 0; i < static_cast<int>(partdata.data.size()); i++){
    ostr << char(partdata.data[i]);
}
RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
// /********打印接收到的数据********/
                    data.data.erase(data.data.begin(), iterend+1);
                }

                if(data.data.front() != frameHeader)
                {
                    auto iter = find(data.data.begin(), data.data.end(), frameHeader);
                    data.data.erase(data.data.begin(), iter);
                }
            }
        }
    }
}


Camera::~Camera()
{
    RCLCPP_INFO(this->get_logger(), "Camera serial port is over!");
}