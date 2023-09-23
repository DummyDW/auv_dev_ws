/*
 * @Author       : zzhef
 * @Date         : 2023-03-23 18:22:18
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-06-07 19:12:29
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/pi.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/uint8_array.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::string;

class Pi : public rclcpp::Node
{
private:
    rclcpp::TimerBase::SharedPtr shoreTimer_;   //岸基5s定时器
    rclcpp::TimerBase::SharedPtr stm32Timer_;   //32 0.2s定时器
    rclcpp::TimerBase::SharedPtr bdgpsTimer_;   //BDGPS 1s定时器

    rclcpp::Publisher<interfaces::msg::Uint8Array>::SharedPtr publisher_Shore_;    //Pi_to_Shore
    rclcpp::Publisher<interfaces::msg::Uint8Array>::SharedPtr publisher_Stm32_;     //Pi_to_Stm32
    rclcpp::Publisher<interfaces::msg::Uint8Array>::SharedPtr publisher_BDGPS_;     //Pi_to_BDGPS
    rclcpp::Publisher<interfaces::msg::Uint8Array>::SharedPtr publisher_EX3040_;    //Pi_to_EX3040

    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_Shore_;    //Shore_to_Pi
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_Stm32_;     //Stm32_to_Pi
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_EX3040_;    //EX3040_to_Pi
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_BDGPS_;     //BDGPS_to_Pi
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_Camera_;    //Camera_to_Pi

    bool bdgpsSetTimeFlag = true;   //控制BDGPS是否可以设置系统时间
    bool ex3040InitFlag = true;     //控制EX3040是否可以初始化校准
    bool onWaterFlag = true;     //控制Pi传给Shore和Stm32的经纬度高程是BDGPS的还是EX3040的
    
    union int16_byte{
      int16_t int16;
      uint16_t uint16;
      u_char s[2];
    };
    union int32_byte{
      int32_t int32;
      uint32_t uint32;
      u_char s[4];
    };
    union float32_byte{
      float float32;
      u_char f[4];
    };
    
    string longitude{"117.1234567"};   //经度
    string latitude{"30.1234567"};     //纬度
    string altitude{"12.12"};       //高程
    string pitch{"1.12"};             //俯仰角
    string roll{"-0.12"};               //横滚角
    string yaw{"123.12"};                 //航向角
    string vn{"001.123"};                   //北向速度
    string ve{"001.123"};                   //东向速度
    string vd{"001.123"};                   //地向速度
    string wx{"0.1234"};                   //x轴角速度
    string wy{"0.1234"};                   //y轴角速度
    string wz{"1.1234"};                   //z轴角速度

    int32_byte objectStatus{0x00000000};    //部件状态
    u_char gateStatus{'0'};                 //舱门状态
    u_char waterWarning{'N'};               //漏水警告
    string cursorX{"2134.86"};         //光标信息X
    string cursorY{"965.989"};         //光标信息X

    int16_byte depthStatus{0x0000};         //深度
    int16_byte heightStatus{0x0000};        //高度
    int16_byte manipulatorDataLow{0x0000};  //机械手数据低位
    int16_byte manipulatorDataHigh{0x0000}; //机械手数据高位
    float32_byte lxMag{0x00000000};         //左梯度磁力仪数据X
    float32_byte lyMag{0x00000000};         //左梯度磁力仪数据Y
    float32_byte lzMag{0x00000000};         //左梯度磁力仪数据Z
    float32_byte rxMag{0x00000000};         //右梯度磁力仪数据X
    float32_byte ryMag{0x00000000};         //右梯度磁力仪数据Y
    float32_byte rzMag{0x00000000};         //右梯度磁力仪数据Z
    int16_byte lowVolBatteryWatch{0x0000};  //低压电池监测
    int16_byte highVolBatteryWatch{0x0000}; //高压电池监测
    int16_byte propellerStatus{0x0000};     //推进器状态
    int16_byte gravityBuoyancyGateWaterStatus{0x0000};     //重心浮心舱门漏水状态
    int16_byte relayStatus{0x0000};         //继电器工作状态
    u_char manipulatorWorkStatus{'0'};      //机械手工作状态
    u_char manipulatorTaskKind{'0'};        //机械手任务类型
    u_char manipulatorWrongStatus{'0'};     //机械手错误状态
    // int16_byte topMainVol{0x0000};          //上主推状态
    // int16_byte belowMainVol{0x0000};        //下主推状态
    // int16_byte leftMainVol{0x0000};         //左主推状态
    // int16_byte rightMainVol{0x0000};        //右主推状态
    // int16_byte frontHangPWM{0x0000};        //艏垂推状态
    // int16_byte leftTailHangPWM{0x0000};     //尾左垂推状态
    // int16_byte rightTailHangPWM{0x0000};    //尾右垂推状态
    // int16_byte frontSidePWM{0x0000};        //艏侧推状态
    // int16_byte backSidePWM{0x0000};         //尾侧推状态
    int16_byte topMainVol{0x1111};          //上主推状态
    int16_byte belowMainVol{0x2222};        //下主推状态
    int16_byte leftMainVol{0x3333};         //左主推状态
    int16_byte rightMainVol{0x4444};        //右主推状态
    int16_byte frontHangPWM{0x5555};        //艏垂推状态
    int16_byte leftTailHangPWM{0x6666};     //尾左垂推状态
    int16_byte rightTailHangPWM{0x7777};    //尾右垂推状态
    int16_byte frontSidePWM{0x1111};        //艏侧推状态
    int16_byte backSidePWM{0x1111};         //尾侧推状态

    void ShoreTimerCallback();
    void Stm32TimerCallback();
    void BDGPSTimerCallback();

    void onSetTime(interfaces::msg::Uint8Array::SharedPtr);
public:
    Pi();
    void ShoreCallback(interfaces::msg::Uint8Array::SharedPtr);
    void Stm32Callback(interfaces::msg::Uint8Array::SharedPtr);
    void EX3040Callback(interfaces::msg::Uint8Array::SharedPtr);
    void BDGPSCallback(interfaces::msg::Uint8Array::SharedPtr);
    void CameraCallback(interfaces::msg::Uint8Array::SharedPtr);
};

Pi::Pi() : Node("Pi")
{
    publisher_Stm32_ = this->create_publisher<interfaces::msg::Uint8Array>("Pi_to_STM32", 10);
    subscriber_Stm32_ = this->create_subscription<interfaces::msg::Uint8Array>("STM32_to_Pi", 10, std::bind(&Pi::Stm32Callback, this , _1));

    publisher_Shore_ = this->create_publisher<interfaces::msg::Uint8Array>("Pi_to_Shore", 10);
    subscriber_Shore_ = this->create_subscription<interfaces::msg::Uint8Array>("Shore_to_Pi", 10, std::bind(&Pi::ShoreCallback, this , _1));
    
    publisher_BDGPS_ =  this->create_publisher<interfaces::msg::Uint8Array>("Pi_to_BDGPS", 10);
    subscriber_BDGPS_ = this->create_subscription<interfaces::msg::Uint8Array>("BDGPS_to_Pi", 10, std::bind(&Pi::BDGPSCallback, this , _1));
    
    publisher_EX3040_ = this->create_publisher<interfaces::msg::Uint8Array>("Pi_to_EX3040", 10);
    subscriber_EX3040_ = this->create_subscription<interfaces::msg::Uint8Array>("EX3040_to_Pi", 10, std::bind(&Pi::EX3040Callback, this , _1));
    
    subscriber_Camera_ = this->create_subscription<interfaces::msg::Uint8Array>("Camera_to_Pi", 10, std::bind(&Pi::CameraCallback, this , _1));

    shoreTimer_ = this->create_wall_timer(5000ms, std::bind(&Pi::ShoreTimerCallback, this));
    stm32Timer_ = this->create_wall_timer(2000ms, std::bind(&Pi::Stm32TimerCallback, this));
    bdgpsTimer_ = this->create_wall_timer(1000ms, std::bind(&Pi::BDGPSTimerCallback, this));
}

/*
    Shore_to_Pi话题的回调函数，解析下行命令，将发给树莓派的命令单独处理，其余发布到Pi_to_Stm32话题
*/
void Pi::ShoreCallback(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    if(dataPtr->data[0] == '@')
        switch(dataPtr->data[1])
        {
            case 'T':
            {
                    if(dataPtr->data[2] == 'P')     //Shore cmd @TP
                        publisher_Stm32_->publish(*dataPtr);
                    else if(dataPtr->data[2] == 'S') 
                    {
                        interfaces::msg::Uint8Array ack;

                        dataPtr->data.erase(dataPtr->data.begin(), dataPtr->data.begin() + 3);  //移除@TS
                        onSetTime(dataPtr);       //Shore cmd @TS

                        ack.data = std::vector<unsigned char>{'@', 'A', 'C', 'K', '_', 'T', 'S', '$'};
                        publisher_Shore_->publish(ack);
                    }
                        
                    break;
            }   
            default:publisher_Stm32_->publish(*dataPtr);
        }
}

void Pi::onSetTime(interfaces::msg::Uint8Array::SharedPtr dataPtr) 
{
    long nsec;
    int sec, min, hour, mday, mon, year;
    year = 2000 + 10 * (dataPtr->data[0] - '0') + (dataPtr->data[1] - '0');
    mon = 10 * (dataPtr->data[2] - '0') + (dataPtr->data[3] - '0') - 1;
    mday = 10 * (dataPtr->data[4] - '0') + (dataPtr->data[5] - '0');
    hour = 10 * (dataPtr->data[6] - '0') + (dataPtr->data[7] - '0');
    min = 10 * (dataPtr->data[8] - '0') + (dataPtr->data[9] - '0');
    sec = 10 * (dataPtr->data[10] - '0') + (dataPtr->data[11] - '0');
    nsec =  100000000 * (dataPtr->data[13] - '0');

    struct tm setTime;
    setTime.tm_sec = sec; 
    setTime.tm_min = min;
    setTime.tm_hour = hour;
    setTime.tm_mday = mday;
    setTime.tm_mon = mon;
    setTime.tm_year = year - 1900;
    
    struct timespec tv_set;
    tv_set.tv_sec  = mktime(&setTime);    //将tm结构转成秒数
    tv_set.tv_nsec = nsec;
    if (clock_settime(CLOCK_REALTIME, &tv_set)<0) 
    {
        perror("clock_settime");
    }
}

/*
    STM32_to_Pi话题的回调函数，一旦该话题有DUP消息，则更新深度高度等状态信息。
*/
void Pi::Stm32Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    if(data->data.size()==68)
    {
        depthStatus.s[0] = data->data[4];
        depthStatus.s[1] = data->data[5];

        heightStatus.s[0] = data->data[6];
        heightStatus.s[1] = data->data[7];

        manipulatorDataLow.s[0] = data->data[8];
        manipulatorDataLow.s[1] = data->data[9];

        manipulatorDataHigh.s[0] = data->data[10];
        manipulatorDataHigh.s[1] = data->data[11];

        lxMag.f[0] = data->data[12];
        lxMag.f[1] = data->data[13];
        lxMag.f[2] = data->data[14];
        lxMag.f[3] = data->data[15];

        lyMag.f[0] = data->data[16];
        lyMag.f[1] = data->data[17];
        lyMag.f[2] = data->data[18];
        lyMag.f[3] = data->data[19];

        lzMag.f[0] = data->data[20];
        lzMag.f[1] = data->data[21];
        lzMag.f[2] = data->data[22];
        lzMag.f[3] = data->data[23];

        rxMag.f[0] = data->data[24];
        rxMag.f[1] = data->data[25];
        rxMag.f[2] = data->data[26];
        rxMag.f[3] = data->data[27];

        ryMag.f[0] = data->data[28];
        ryMag.f[1] = data->data[29];
        ryMag.f[2] = data->data[30];
        ryMag.f[3] = data->data[31];

        rzMag.f[0] = data->data[32];
        rzMag.f[1] = data->data[33];
        rzMag.f[2] = data->data[34];
        rzMag.f[3] = data->data[35];

        lowVolBatteryWatch.s[0] = data->data[36];
        lowVolBatteryWatch.s[1] = data->data[37];

        highVolBatteryWatch.s[0] = data->data[38];
        highVolBatteryWatch.s[1] = data->data[39];

        propellerStatus.s[0] = data->data[40];
        propellerStatus.s[1] = data->data[41];

        gravityBuoyancyGateWaterStatus.s[0] = data->data[42];
        gravityBuoyancyGateWaterStatus.s[1] = data->data[43];

        relayStatus.s[0] = data->data[44];
        relayStatus.s[1] = data->data[45];

        manipulatorWorkStatus = data->data[46];
        manipulatorTaskKind = data->data[47];
        manipulatorWrongStatus = data->data[48];

        topMainVol.s[0] = data->data[50];
        topMainVol.s[1] = data->data[49];

        belowMainVol.s[0] = data->data[52];
        belowMainVol.s[1] = data->data[51];

        leftMainVol.s[0] = data->data[54];
        leftMainVol.s[1] = data->data[53];

        rightMainVol.s[0] = data->data[56];
        rightMainVol.s[1] = data->data[55];

        frontHangPWM.s[0] = data->data[58];
        frontHangPWM.s[1] = data->data[57];

        leftTailHangPWM.s[0] = data->data[60];
        leftTailHangPWM.s[1] = data->data[59];

        rightTailHangPWM.s[0] = data->data[62];
        rightTailHangPWM.s[1] = data->data[61];

        frontSidePWM.s[0] = data->data[64];
        frontSidePWM.s[1] = data->data[63];

        backSidePWM.s[0] = data->data[66];
        backSidePWM.s[1] = data->data[65];
    }
    else if(data->data.size() == 8)
        publisher_Shore_->publish(*data);  //32发送ACK给树莓派，树莓派将该ACK发给PC
}

/*
    EX3040_to_Pi话题的回调函数，一旦该话题有data，则更新经纬度，俯仰角，横滚角，艏向角，
    北向速度，东向速度，地向速度，x轴角速率，y轴角速率，z轴角速率
*/
void Pi::EX3040Callback(interfaces::msg::Uint8Array::SharedPtr data)
{
    if(data->data[3] == 'F')
    {
        data->data.erase(data->data.begin(),data->data.begin()+7);  //抹去协议头

        auto iter = find(data->data.begin(),data->data.end(),',');
        data->data.erase(data->data.begin(),iter+1);    //抹去GPSWeek
        iter = find(data->data.begin(),data->data.end(),',');
        data->data.erase(data->data.begin(),iter+1);    //抹去GPSTime

        iter = find(data->data.begin(),data->data.end(),',');
        yaw.assign(data->data.begin(),iter);    //提取艏向角
        data->data.erase(data->data.begin(),iter+1);
        
        iter = find(data->data.begin(),data->data.end(),',');
        pitch.assign(data->data.begin(),iter);    //提取俯仰角
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        roll.assign(data->data.begin(),iter);    //提取横滚角
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        if(!onWaterFlag)    longitude.assign(data->data.begin(),iter);    //如果BDGPS定位失败，则提取惯导经度
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        if(!onWaterFlag)    latitude.assign(data->data.begin(),iter);    //提取纬度
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        if(!onWaterFlag)    altitude.assign(data->data.begin(),iter);    //提取高度
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        vn.assign(data->data.begin(),iter);    //北向速度
        data->data.erase(data->data.begin(),iter+1);        

        iter = find(data->data.begin(),data->data.end(),',');
        ve.assign(data->data.begin(),iter);    //东向速度
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        vd.assign(data->data.begin(),iter);    //地向速度
        data->data.erase(data->data.begin(),iter+1);
    }
    else if(data->data[3]=='I')
    {
        data->data.erase(data->data.begin(),data->data.begin()+7);  //抹去协议头

        auto iter = find(data->data.begin(),data->data.end(),',');
        data->data.erase(data->data.begin(),iter+1);    //抹去GPSWeek
        iter = find(data->data.begin(),data->data.end(),',');
        data->data.erase(data->data.begin(),iter+1);    //抹去GPSTime

        iter = find(data->data.begin(),data->data.end(),',');
        wx.assign(data->data.begin(),iter);    //wx
        data->data.erase(data->data.begin(),iter+1);
        
        iter = find(data->data.begin(),data->data.end(),',');
        wy.assign(data->data.begin(),iter);    //wy
        data->data.erase(data->data.begin(),iter+1);

        iter = find(data->data.begin(),data->data.end(),',');
        wz.assign(data->data.begin(),iter);    //wz
        data->data.erase(data->data.begin(),iter+1);
    }
}

/*
    BDGPS_to_Pi话题的回调函数，一旦该话题有data，则给树莓派更新年月日时分秒
*/
void Pi::BDGPSCallback(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    if(dataPtr->data[34] == 0)  //定位成功
    {
        onWaterFlag = true;
        if(bdgpsSetTimeFlag)    //初始化树莓派系统时间
        {
            struct tm BDGPStime;
            BDGPStime.tm_sec = dataPtr->data[15]; 
            BDGPStime.tm_min = dataPtr->data[14];
            BDGPStime.tm_hour = dataPtr->data[13];
            BDGPStime.tm_mday = dataPtr->data[12];
            BDGPStime.tm_mon = dataPtr->data[11]-1;
            BDGPStime.tm_year = 2000 + dataPtr->data[10] - 1900;
            
            struct timespec tv_set;
            tv_set.tv_sec  = mktime(&BDGPStime);    //将tm结构转成秒数
            tv_set.tv_nsec = 0;
            if (clock_settime(CLOCK_REALTIME, &tv_set)<0) 
            {
                perror("clock_settime");
            }
            
            bdgpsSetTimeFlag = false;
        }

        if(ex3040InitFlag)  //初始化惯导位置
        {
            interfaces::msg::Uint8Array senddata;
            senddata.data.assign(dataPtr->data.begin()+16, dataPtr->data.begin()+28);
            publisher_EX3040_ -> publish(senddata);    //把经纬度信息发布到Pi_to_EX3040
            ex3040InitFlag = false;
        }

        //同步BDGPS获取的经纬度高程数据
        {
            float fen;
            int fenfen; //fenfen = 0.0001fen
            int alt;
            char buff[4];
            using std::to_string;
            
            //处理经度
            buff[0] = dataPtr->data[20];
            buff[1] = dataPtr->data[19];
            buff[2] = 0x00;
            buff[3] = 0x00;
            memcpy(&fenfen, buff, sizeof(int));
            fen = dataPtr->data[18]+fenfen * 0.0001;
            longitude = to_string(dataPtr->data[17] + fen/60);   //水上定位成功时，选择BDGPS的经纬度高程。失败则选择惯导的。

            //处理纬度
            buff[0] = dataPtr->data[25];
            buff[1] = dataPtr->data[24];
            buff[2] = 0x00;
            buff[3] = 0x00;
            memcpy(&fenfen, buff, sizeof(int));
            fen = dataPtr->data[23]+fenfen * 0.0001;
            latitude = to_string(dataPtr->data[22] + fen/60);

            //处理高程
            buff[0] = dataPtr->data[27];
            buff[1] = dataPtr->data[26];
            buff[2] = 0x00;
            buff[3] = 0x00;
            memcpy(&alt, buff, sizeof(int));
            altitude = to_string(alt);
        }
    }
    else //定位失败，可能是位于水下了，此时改用惯导的经纬度
        onWaterFlag = false;
}

/*
    Camera_to_Pi话题的回调函数
*/
void Pi::CameraCallback(interfaces::msg::Uint8Array::SharedPtr data)
{
    auto iter = find(data->data.begin(),data->data.end(),',');
    data->data.erase(data->data.begin(),iter+1);    //抹去$斜率
    iter = find(data->data.begin(),data->data.end(),',');
    data->data.erase(data->data.begin(),iter+1);    //抹去,截距

    iter = find(data->data.begin(),data->data.end(),',');
    data->data.erase(data->data.begin(),iter+1);    //抹去,深度
    
    iter = find(data->data.begin(),data->data.end(),',');
    cursorX.assign(data->data.begin(),iter);    //光标信息X
    data->data.erase(data->data.begin(),iter+1);

    iter = find(data->data.begin(),data->data.end(),',');
    cursorY.assign(data->data.begin(),iter);    //光标信息XY

   /*---------------------------2023-09-15-s----------------------------------*/
    data->data.erase(data->data.begin(),iter+1);//抹去Y,
    if(*(data->data.begin()) == '0'){
        cursorX.assign("0");
        cursorY.assign("0");
    }


    //调试：
    std::ostringstream ostr1;
    ostr1 << "Camera to Pi2:";
    for(int i = 0; i < static_cast<int>(data->data.size()); i++){
       ostr1 << char(data->data[i]);
     }
    RCLCPP_INFO(this->get_logger(),"%s\r\n", ostr1.str().c_str());  
    RCLCPP_INFO(this->get_logger(),"cursorX=%s,cursorY=%s \r\n",cursorX.c_str(),cursorY.c_str());

    /*---------------------------2023-09-15-s----------------------------------*/

}

/*
    五秒定时器的回调函数，树莓派定时向岸基发送消息。
*/
void Pi::ShoreTimerCallback()
{
    std::ostringstream ostr;
    interfaces::msg::Uint8Array data;
    using std::ios_base;
    
    ostr.fill('0');//填充0
    ostr.setf(ios_base::fixed, ios_base::floatfield);//用定点格式显示浮点数,而非科学计数法表示
    ostr.setf((ios_base::showpoint));	//显示浮点数小数点后面的零
    //ostr.precision(4);//设置精度为4，并返回上一次的设置。

    time_t now;//采集当前时间，填入待发送字符串
    struct tm *p_tm;
    char buf[30]={0};
    now = time(NULL); 
    p_tm = localtime(&now);
    strftime(buf, sizeof(buf)-1, "%g-%m-%d %X", p_tm);

    ostr << "@VS," << buf << "," << longitude << ","  << latitude << ","  << altitude 
    << "," << pitch << ","  << roll << ","  << yaw << ","  << vn << ","  << ve 
    << ","  << vd << ","  << wx << ","  << wy << ","  << wz << "," ;
    
    ostr << depthStatus.uint16 << "," << heightStatus.uint16 << ",";

    ostr << std::hex << std::uppercase << objectStatus.int32 << ",";

    ostr << manipulatorWorkStatus << "," << gateStatus << "," << lowVolBatteryWatch.int16 
    << "," << highVolBatteryWatch.int16  << "," << waterWarning << "," << cursorX << "," 
    << cursorY << "," ;
    
    ostr.precision(2);
    ostr << float(topMainVol.int16) / 100 << "," << float(belowMainVol.int16) / 100
    << "," << float(leftMainVol.int16) / 100 << "," << float(rightMainVol.int16) / 100 << ",";
    
    ostr << std::dec << frontHangPWM.uint16 << "," << leftTailHangPWM.uint16 << "," << rightTailHangPWM.uint16 
    << "," << frontSidePWM.uint16 << "," << backSidePWM.uint16 << ",$";

    auto str = ostr.str();
    
    data.data.assign(str.begin(),str.end());
    publisher_Shore_->publish(data);
}

/*
    0.2秒定时器的回调函数，树莓派定时向stm32发送消息。
*/
void Pi::Stm32TimerCallback()
{
    interfaces::msg::Uint8Array data;
    std::ostringstream ostr;
    using std::ios_base;

    int32_byte Lx = {int32_t(std::stof(longitude) * 10000000)};
    int32_byte Ly = {int32_t(std::stof(latitude) * 10000000)};
    int32_byte Lz = {int32_t(std::stof(altitude) * 100)};
    int16_byte Ax = {int16_t(std::stof(pitch) * 100)};
    int16_byte Ay = {int16_t(std::stof(roll) * 100)};
    int16_byte Az = {int16_t(std::stof(yaw) * 100)};
    int16_byte Vx = {int16_t(std::stof(vn) * 1000)};
    int16_byte Vy = {int16_t(std::stof(ve) * 1000)};
    int16_byte Vz = {int16_t(std::stof(vd) * 1000)};
    int16_byte Ox = {int16_t(std::stof(wx) * 10000)};
    int16_byte Oy = {int16_t(std::stof(wy) * 10000)};
    int16_byte Oz = {int16_t(std::stof(wz) * 10000)};
    int16_byte targetx = {int16_t(std::stof(cursorX))};
    int16_byte targety = {int16_t(std::stof(cursorY))};

    //33个字节，实际上协议已经改为39个字节。
    data.data = {'@','W','D','D', Lx.s[0], Lx.s[1], Ly.s[0], Ly.s[1], Lz.s[0], Lz.s[1], Ax.s[0], Ax.s[1]
    , Ay.s[0], Ay.s[1], Az.s[0], Az.s[1], Vx.s[0], Vx.s[1], Vy.s[0], Vy.s[1], Vz.s[0], Vz.s[1]
    , Ox.s[0], Ox.s[1], Oy.s[0], Oy.s[1], Oz.s[0], Oz.s[1], targetx.s[0], targetx.s[1], targety.s[0], targety.s[1], '$'};
    
    publisher_Stm32_ -> publish(data);

    ostr << "@WDD," << Lx.int32 << "," << Ly.int32 << ","  << Lz.int32 << ","  << Ax.int16 
    << "," << Ay.int16  << ","  << Az.int16  << ","  << Vx.int16  << ","  << Vy.int16  << ","  << Vz.int16  
    << ","  << Ox.int16  << ","  << Oy.int16  << ","  << Oz.int16  << ","  << targetx.int16  << "," << targety.int16;
}

void Pi::BDGPSTimerCallback()
{
    interfaces::msg::Uint8Array data;
    data.data = {0x24, 0x47, 0x50, 0x53, 0x5A, 0x00, 0x0F, 0x02, 0xE4, 0xEC, 0x00, 0x00, 0x00, 0x00, 0x3F};
    publisher_BDGPS_ -> publish(data);
}