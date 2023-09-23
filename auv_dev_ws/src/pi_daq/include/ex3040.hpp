/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 07:34:32
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-05-09 12:29:16
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/ex3040.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "daqabc.hpp"

using namespace SIGMA::AUV100;

class EX3040 : public DaqABC
{
private:
    virtual void CallBack(interfaces::msg::Uint8Array::SharedPtr);
    virtual void Init(const std::string, BaudRate);
public:
    EX3040(const std::string, BaudRate, const char[], const char[], const char[]);
    virtual ~EX3040();
    virtual void PubEvent(char, char);
};

EX3040::EX3040(const std::string deviceName, BaudRate baudRate, const char pubTopicName[],
const char subTopicName[], const char nodeName[]) : DaqABC(pubTopicName, subTopicName, nodeName)
{
    Init(deviceName, baudRate);
}

void EX3040::Init(const std::string deviceName, BaudRate baudRate)
{
    DaqABC::Init(deviceName, baudRate);
    SerialOpen();
}

//订阅"Pi_to_EX3040"
void EX3040::CallBack(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    std::vector<uint8_t> senddata = {0xaa, 0x55, 0xb1};
    float tempFloat;
    float fen;
    int fenfen; //fenfen = 0.0001fen
    int altitude;
    char buff[4];
    
    //处理经度
    buff[0] = dataPtr->data[4];     //填充0.0001分
    buff[1] = dataPtr->data[3];     //该协议中0.0001分是高位在前，树莓派小端，所以将高位放在高地址
    buff[2] = 0x00;
    buff[3] = 0x00;
    memcpy(&fenfen, buff, sizeof(int));     //将0.0001分化为整型
    fen = dataPtr->data[2]+fenfen * 0.0001;     //分=分+0.0001分
    tempFloat = dataPtr->data[1] + fen/60;     //度=度+分/60
    memcpy(buff, &tempFloat, sizeof(float));   //把度各字节放入buff中
    for(int i = 0;i<4;i++){
        senddata.push_back(buff[i]);    //把经度信息填充入待发送字段
    }

    //处理纬度
    buff[0] = dataPtr->data[9];
    buff[1] = dataPtr->data[8];
    buff[2] = 0x00;
    buff[3] = 0x00;
    memcpy(&fenfen, buff, sizeof(int));
    fen = dataPtr->data[7]+fenfen * 0.0001;
    tempFloat = dataPtr->data[6] + fen/60;
    memcpy(buff, &tempFloat, sizeof(float));
    for(int i = 0;i<4;i++){
        senddata.push_back(buff[i]);//把纬度信息填充入待发送字段
    }

    //处理海拔
    buff[0] = dataPtr->data[11];
    buff[1] = dataPtr->data[10];
    buff[2] = 0x00;
    buff[3] = 0x00;
    memcpy(&altitude, buff, sizeof(int));
    tempFloat = altitude;//整型海拔转换为float型
    memcpy(buff, &tempFloat, sizeof(float));
    for(int i = 0;i<4;i++){
        senddata.push_back(buff[i]);
    }

    //加入校验和
    u_int8_t sum = 0;
    for(long unsigned int i = 0;i<senddata.size();i++){
        sum = sum+senddata[i];
    }
    senddata.push_back(sum);

    //打印格式化后发往惯导的初始化数据
    std::ostringstream ostr;
    ostr.fill('0');
    ostr << "EX3040 after format location:";
    for(int i = 0; i < static_cast<int>(senddata.size()); i++){
        if(i != 0) ostr << " ";
        ostr << "0X" << std::hex << std::uppercase << std::setw(2) << int(senddata[i]);
    }
    RCLCPP_INFO(this->get_logger(), ostr.str().c_str());

    SerialWriteBinary(senddata);
}

void EX3040::PubEvent(char frameHeader, char frameTail)
{
    using namespace std;
    if(SerialAvailable())
    {
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
                    data.data.erase(iter+1, data.data.end());//&GPHFD...\r\nFGPHFD...\r\nFGPHFD...这种情况没考虑
            }

            //@@@GPHFD...$$$ -> @GPHFD...$
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
// std::ostringstream ostr;
// ostr << "EX3040 to Pi:";
// for(int i = 0; i < static_cast<int>(partdata.data.size()); i++){
//     ostr << char(partdata.data[i]);
// }
// RCLCPP_INFO(this->get_logger(), ostr.str().c_str()); 
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

EX3040::~EX3040()
{
    RCLCPP_INFO(this->get_logger(), "EX3040 serial port is over!");
}