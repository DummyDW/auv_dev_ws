/*
 * @Author       : zzhef
 * @Date         : 2023-04-29 17:12:18
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-08-17 19:36:12
 * @FilePath     : \auv_dev_ws\src\savedata\src\magsave.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include <vector>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <sys/time.h>
#include <iomanip>

#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/uint8_array.hpp"

using std::placeholders::_1;
using namespace std;

std::vector<std::string> stringSplit(const std::string& str, char delim);

class MagSave : public rclcpp::Node 
{
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr STM32_Pi_Subscriber_;
    void STM32_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr EX3040_Pi_Subscriber_;
    void EX3040_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr Mag_Pi_Subscriber_;
    void Magneto_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr);

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr BD_EX_Subscriber_;
    void BDGPS_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr);

    rclcpp::TimerBase::SharedPtr writeFileTimer_;
    void WriteFile500msTimerCallback();

    float bdgpsLongititude;
    float bdgpsLatitude;
    float frontMagX;
    float frontMagY;
    float frontMagZ;
    float frontMagS;

    union float32_byte{
      float float32;
      u_char f[4];
    };
    float32_byte leftMagX;
    float32_byte leftMagY;
    float32_byte leftMagZ;
    float leftMagS;
    float32_byte rightMagX;
    float32_byte rightMagY;
    float32_byte rightMagZ;
    float rightMagS;

    string exlatitude;
    string exlongititude;

public:
    MagSave();
};

MagSave::MagSave() : Node("MagSave")
{
    STM32_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("STM32_to_Pi", 10, std::bind(&MagSave::STM32_to_Pi_Callback, this , _1));
    EX3040_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("EX3040_to_Pi", 10, std::bind(&MagSave::EX3040_to_Pi_Callback, this , _1));
    Mag_Pi_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("Magneto_to_Pi", 10, std::bind(&MagSave::Magneto_to_Pi_Callback, this , _1));
    BD_EX_Subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>("BDGPS_to_EX3040", 10, std::bind(&MagSave::BDGPS_to_Pi_Callback, this , _1));
    writeFileTimer_ = this->create_wall_timer(500ms, std::bind(&MagSave::WriteFile500msTimerCallback, this));
}

void MagSave::WriteFile500msTimerCallback()
{
    leftMagS = sqrt(leftMagX.float32 * leftMagX.float32 + leftMagY.float32 * leftMagY.float32 + leftMagZ.float32 * leftMagZ.float32);
    rightMagS = sqrt(rightMagX.float32 * rightMagX.float32 + rightMagY.float32 * rightMagY.float32 + rightMagZ.float32 * rightMagZ.float32);

    leftMagS = sqrt(leftMagX.float32 * leftMagX.float32 + leftMagY.float32 * leftMagY.float32 + leftMagZ.float32 * leftMagZ.float32);
    rightMagS = sqrt(rightMagX.float32 * rightMagX.float32 + rightMagY.float32 * rightMagY.float32 + rightMagZ.float32 * rightMagZ.float32);

    std::ostringstream ostr;
    ostr.fill('0');

    struct timespec ts;
    char buf[25]={0};
    memset(&ts, 0, sizeof(struct timespec));
    clock_gettime(CLOCK_REALTIME, &ts);
    struct tm *p_tm;
    p_tm = localtime(&(ts.tv_sec));
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %H:%M:%S", p_tm);

    ostr << buf << "." << std::setw(2) << ts.tv_nsec/10000000 << ",";

    ostr << frontMagX <<','<< frontMagY <<','<< frontMagZ <<','<< frontMagS <<','<< 
    leftMagX.float32 <<','<< leftMagY.float32 <<','<< leftMagZ.float32 <<','<< leftMagS <<','<< 
    rightMagX.float32 <<','<< rightMagY.float32 <<','<< rightMagZ.float32 <<','<< rightMagS <<','<<
    setiosflags(ios::fixed) << setprecision(7) <<
    bdgpsLongititude <<','<< bdgpsLatitude <<','<< exlongititude <<','<< exlatitude << '\n';
    
    RCLCPP_INFO(this->get_logger(), ostr.str().c_str());
}

/**************************
    更新左右磁力仪磁场数据
***************************/
void MagSave::STM32_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    leftMagX.f[0] = dataPtr->data[12];
    leftMagX.f[1] = dataPtr->data[13];
    leftMagX.f[2] = dataPtr->data[14];
    leftMagX.f[3] = dataPtr->data[15];
    leftMagY.f[0] = dataPtr->data[16];
    leftMagY.f[1] = dataPtr->data[17];
    leftMagY.f[2] = dataPtr->data[18];
    leftMagY.f[3] = dataPtr->data[19];
    leftMagZ.f[0] = dataPtr->data[20];
    leftMagZ.f[1] = dataPtr->data[21];
    leftMagZ.f[2] = dataPtr->data[22];
    leftMagZ.f[3] = dataPtr->data[23];
    rightMagX.f[0] = dataPtr->data[24];
    rightMagX.f[1] = dataPtr->data[25];
    rightMagX.f[2] = dataPtr->data[26];
    rightMagX.f[3] = dataPtr->data[27];
    rightMagY.f[0] = dataPtr->data[28];
    rightMagY.f[1] = dataPtr->data[29];
    rightMagY.f[2] = dataPtr->data[30];
    rightMagY.f[3] = dataPtr->data[31];
    rightMagZ.f[0] = dataPtr->data[32];
    rightMagZ.f[1] = dataPtr->data[33];
    rightMagZ.f[2] = dataPtr->data[34];
    rightMagZ.f[3] = dataPtr->data[35];
}

/**************************
    更新EX3040经纬度
***************************/
void MagSave::EX3040_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    if(dataPtr->data[3] == 'F')
    {
        std::ostringstream ostr;
        std::vector<std::string> strVector;
        for(int i = 0; i < static_cast<int>(dataPtr->data.size()-2); i++) //去掉末尾\r\n
            ostr << char(dataPtr->data[i]);
        strVector = stringSplit(ostr.str(),',');
        exlongititude = strVector[6];
        exlatitude = strVector[7];
        exlongititude = strVector[6];
        exlatitude = strVector[7];
    }
}

/**************************
    更新前置磁力仪磁场数据
***************************/
void MagSave::Magneto_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    unsigned int temp_val = 0;
    int cal_val = 0;

    if(dataPtr->data[0] == 0x46 && dataPtr->data[1] == 0x4D)
    {
        if(dataPtr->data[22] == 0x0D && dataPtr->data[23] == 0x0A)
        {
            if(dataPtr->data[11] == 0x58)
            {
                temp_val = (dataPtr->data[10] << 16) + (dataPtr->data[9] << 8) + (dataPtr->data[8]);
                if(temp_val <= 0x7fffff)
                    cal_val = temp_val;
                else
                    cal_val = (temp_val - 0xffffff - 1);
                
                frontMagX = (float)(cal_val * (2.4 / 0x7fffff) * 1000 * 45);
            }
            if(dataPtr->data[15] == 0x59)
            {
                temp_val = (dataPtr->data[14] << 16) + (dataPtr->data[13] << 8) + (dataPtr->data[12]);
                if(temp_val <= 0x7fffff)
                    cal_val = temp_val;
                else
                    cal_val = (temp_val - 0xffffff - 1);
                
                frontMagY = (float)(cal_val * (2.4 / 0x7fffff) * 1000 * 45);
            }
            if(dataPtr->data[19] == 0x5A)
            {
                temp_val = (dataPtr->data[18] << 16) + (dataPtr->data[17] << 8) + (dataPtr->data[16]);
                if(temp_val <= 0x7fffff)
                    cal_val = temp_val;
                else
                    cal_val = (temp_val - 0xffffff - 1);
                
                frontMagZ = (float)(cal_val * (2.4 / 0x7fffff) * 1000 * 45);
            }
            frontMagS = sqrt(frontMagX * frontMagX + frontMagY * frontMagY + frontMagZ * frontMagZ);
        }
    }
}

/**************************
    更新BDGPS经纬度数据
***************************/
void MagSave::BDGPS_to_Pi_Callback(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    double fen;
    int fenfen;
    char buff[4];

    //获取经度
    buff[0] = dataPtr->data[4];//填充0.0001分
    buff[1] = dataPtr->data[3];
    buff[2] = 0x00;
    buff[3] = 0x00;
    memcpy(&fenfen, buff, sizeof(int));//将0.0001分化为整型
    fen = dataPtr->data[2]+fenfen * 0.0001;//分=分+0.0001分
    bdgpsLongititude = dataPtr->data[1] + fen/60;//度=度+分/60

    //处理纬度
    buff[0] = dataPtr->data[9];//填充0.0001分
    buff[1] = dataPtr->data[8];
    buff[2] = 0x00;
    buff[3] = 0x00;
    memcpy(&fenfen, buff, sizeof(int));//将0.0001分化为整型
    fen = dataPtr->data[7]+fenfen * 0.0001;//分=分+0.0001分
    bdgpsLatitude = dataPtr->data[6] + fen/60;//度=度+分/60
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<MagSave>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    exit(0);
}

std::vector<std::string> stringSplit(const std::string& str, char delim) 
{
    std::stringstream ss(str);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) 
        if (!item.empty())
            elems.push_back(item);
    return elems;
}