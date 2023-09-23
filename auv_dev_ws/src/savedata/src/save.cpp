/*
 * @Author       : zzhef
 * @Date         : 2023-04-25 14:25:33
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-08-29 15:29:01
 * @FilePath     : \auv_dev_ws\src\savedata\src\save.cpp
 * @Description  : 保存树莓派和岸基接收到的数据
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sys/time.h>

#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/uint8_array.hpp"

using std::placeholders::_1;

class Save : public rclcpp::Node 
{
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr PC_Pi_Subscriber_;
    void Shore_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr STM32_Pi_Subscriber_;
    void STM32_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr EX3040_Pi_Subscriber_;
    void EX3040_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Mag_Pi_Subscriber_;
    void Magneto_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr BD_Pi_Subscriber_;
    void BDGPS_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data);
    
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Pi_PC_Subscriber_;
    void Pi_to_Shore_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Pi_Stm32_Subscriber_;
    void Pi_to_STM32_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Pi_BD_Subscriber_;
    void Pi_to_BDGPS_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Pi_Mag_Subscriber_;
    void Pi_to_Magneto_Callback(interfaces::msg::Uint8Array::SharedPtr data);
    
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Pi_P360_Subscriber_;
    void Pi_to_P360_Callback(interfaces::msg::Uint8Array::SharedPtr data);
        
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr P360_Pi_Subscriber_;
    void P360_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data);

    void MakeInfoStr(interfaces::msg::Uint8Array::SharedPtr &data, const char *specialStr);

public:
    Save();
};

Save::Save() : Node("Save")
{
    Pi_PC_Subscriber_= this->create_subscription<interfaces::msg::Uint8Array>("Pi_to_Shore", 10, std::bind(&Save::Pi_to_Shore_Callback, this , _1));
    PC_Pi_Subscriber_= this->create_subscription<interfaces::msg::Uint8Array>("Shore_to_Pi", 10, std::bind(&Save::Shore_to_Pi_Callback, this , _1));
    STM32_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("STM32_to_Pi", 10, std::bind(&Save::STM32_to_Pi_Callback, this , _1));
    EX3040_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("EX3040_to_Pi", 10, std::bind(&Save::EX3040_to_Pi_Callback, this , _1));
    Mag_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("Magneto_to_Pi", 10, std::bind(&Save::Magneto_to_Pi_Callback, this , _1));
    BD_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("BDGPS_to_Pi", 10, std::bind(&Save::BDGPS_to_Pi_Callback, this , _1));
    
    Pi_Stm32_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("Pi_to_STM32", 10, std::bind(&Save::Pi_to_STM32_Callback, this , _1));
    Pi_BD_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("Pi_to_BDGPS", 10, std::bind(&Save::Pi_to_BDGPS_Callback, this , _1));
    Pi_Mag_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("Pi_to_Magneto", 10, std::bind(&Save::Pi_to_Magneto_Callback, this , _1));
    Pi_P360_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("Pi_to_P360", 10, std::bind(&Save::Pi_to_P360_Callback, this , _1));
    
    P360_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("P360_to_Pi", 10, std::bind(&Save::P360_to_Pi_Callback, this , _1));
}

void Save::MakeInfoStr(interfaces::msg::Uint8Array::SharedPtr &data, const char *specialStr)
{
    std::ostringstream ostr;

    struct timespec ts;
    char buf[25]={0};
    memset(&ts, 0, sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *p_tm;
    p_tm = localtime(&(ts.tv_sec));
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %H:%M:%S", p_tm);

    ostr.fill('0');
    ostr << buf << "." << std::setw(2) << ts.tv_nsec/10000000 << " " << specialStr;
    for(int i = 0; i < static_cast<int>(data->data.size()); i++){
        if(i != 0) ostr << " ";
        ostr << std::hex << std::uppercase << std::setw(2) << int(data->data[i]);
    }

    RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
}

/**************************
    打印PC发给树莓派的命令
***************************/
void Save::Shore_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    MakeInfoStr(data, "Cmd from PC to Pi: ");
}

/**************************
    打印树莓派发到Shore的数据
***************************/
void Save::Pi_to_Shore_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    std::ostringstream ostr;

    struct timespec ts;
    char buf[25]={0};
    memset(&ts, 0, sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *p_tm;
    p_tm = localtime(&(ts.tv_sec));
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %H:%M:%S", p_tm);

    ostr.fill('0');
    ostr << buf << "." << std::setw(2) << ts.tv_nsec/10000000 << " " << "Data from Pi to Shore: ";
    for(int i = 0;i<static_cast<int>(data->data.size());i++)
        ostr << char(data->data[i]);

    RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
}

/**************************
    打印32发到树莓派的数据
***************************/
void Save::STM32_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    std::ostringstream ostr;

    struct timespec ts;
    char buf[25]={0};
    memset(&ts, 0, sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *p_tm;
    p_tm = localtime(&(ts.tv_sec));
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %H:%M:%S", p_tm);
    ostr.fill('0');
    ostr << buf << "." << std::setw(2) << ts.tv_nsec/10000000 << " ";

    if(data->data.size()==68)
    {
        ostr << "Data from STM32 to Pi: ";
        for(int i = 0; i < static_cast<int>(data->data.size()); i++)
        {
            if(i != 0) ostr << " ";
            ostr << std::hex << std::uppercase << std::setw(2) << int(data->data[i]);
        }
    }
    else if(data->data.size()==8)
    {
        ostr << "ACK from STM32 to Pi:";
        for(int i = 0;i<static_cast<int>(data->data.size());i++)
            ostr << char(data->data[i]);
    }

	RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
}

/**************************
    打印EX3040发到树莓派的数据
***************************/
void Save::EX3040_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    std::ostringstream ostr;

    struct timespec ts;
    char buf[25]={0};
    memset(&ts, 0, sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *p_tm;
    p_tm = localtime(&(ts.tv_sec));
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %H:%M:%S", p_tm);

    ostr.fill('0');
    ostr << buf << "." << std::setw(2) << ts.tv_nsec/10000000 << " " << "Data from EX3040 to Pi: ";

    for(int i = 0;i<static_cast<int>(data->data.size() - 2);i++) //去掉结尾0D 0A
        ostr << char(data->data[i]);

    RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
}

/**************************
    打印前置磁力仪发到pi的数据
***************************/
void Save::Magneto_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    MakeInfoStr(data, "Data from Front Magneto to Pi: ");
}

/**************************
    打印BDGPS发到pi的数据
***************************/
void Save::BDGPS_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    MakeInfoStr(data, "Data from BDGPS to Pi: ");
}

/**************************
    打印Pi发到STM32的数据
***************************/
void Save::Pi_to_STM32_Callback(interfaces::msg::Uint8Array::SharedPtr data){
    std::ostringstream ostr;

    struct timespec ts;
    char buf[25]={0};
    memset(&ts, 0, sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *p_tm;
    p_tm = localtime(&(ts.tv_sec));
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %H:%M:%S", p_tm);
    ostr.fill('0');
    ostr << buf << "." << std::setw(2) << ts.tv_nsec/10000000 << " ";

    if(data->data.size()==33)
    {
        ostr << "Data from Pi to STM32: ";
        for(int i = 0; i < static_cast<int>(data->data.size()); i++)
        {
            if(i != 0) ostr << " ";
            ostr << std::hex << std::uppercase << std::setw(2) << int(data->data[i]);
        }
    }
    else
    {
        ostr << "Cmd from Pi to STM32: ";
        for(int i = 0; i < static_cast<int>(data->data.size()); i++)
        {
            if(i != 0) ostr << " ";
            ostr << std::hex << std::uppercase << std::setw(2) << int(data->data[i]);
        }
    }

    RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
}

/**************************
    打印Pi发到BDGPS的数据
***************************/
void Save::Pi_to_BDGPS_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    MakeInfoStr(data, "Cmd from Pi to BDGPS: ");
}

/**************************
    打印Pi发到Magneto的数据
***************************/
void Save::Pi_to_Magneto_Callback(interfaces::msg::Uint8Array::SharedPtr data){
    MakeInfoStr(data, "Data from Pi to Magneto: ");
}

/**************************
    打印Pi发到P360的数据
***************************/
void Save::Pi_to_P360_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    MakeInfoStr(data, "Data from Pi to P360: ");
}

/**************************
    打印P360发到Pi的数据
***************************/
void Save::P360_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    MakeInfoStr(data, "Data from P360 to Pi: ");
}

int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Save>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    exit(0);
}