/*
 * @Author       : zzhef
 * @Date         : 2023-03-22 00:22:33
 * @LastEditors: zzhef
 * @LastEditTime: 2023-08-29 14:45:29
 * @FilePath: \pi_daq\include\daqabc.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#ifndef DAQABC_H_
#define DAQABC_H_
#include <vector>
#include <algorithm>
#include <cstring>
#include "SerialPort.hpp"
#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/uint8_array.hpp"

using namespace mn::CppLinuxSerial;
using namespace std;
using std::placeholders::_1;
using namespace std::chrono_literals;

namespace SIGMA 
{
    namespace AUV100
    {
        class DaqABC : public rclcpp::Node
        {
        private:
            interfaces::msg::Uint8Array data;
            rclcpp::Duration durTime_;
            rclcpp::Duration nowDur_;
            SerialPort serial;
            rclcpp::Publisher<interfaces::msg::Uint8Array>::SharedPtr publisher_;
            rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_;
        protected:
            interfaces::msg::Uint8Array & GetData() {return data;}
            rclcpp::Duration & GetDurTime() {return durTime_;}
            rclcpp::Duration & GetNowDur() {return nowDur_;}
            void ClearData(){data.data.clear();}
            bool SerialAvailable(){return (serial.Available() != 0) ? true : false;}
            void SerialWriteBinary(vector<uint8_t> &data) {serial.WriteBinary(data);}
            void SerialReadBinary(vector<uint8_t> &data) {serial.ReadBinary(data);}
            void SerialSetTimeout(int32_t num) {serial.SetTimeout(num);}
            void SerialOpen() {serial.Open();}
            void Publish(interfaces::msg::Uint8Array &data) {publisher_ -> publish(data);}
            virtual void Init(const string, BaudRate) = 0;
            virtual void CallBack(interfaces::msg::Uint8Array::SharedPtr);
            void PrintHexAndAsciiData(interfaces::msg::Uint8Array& data, const char * str = "");
        public:
            DaqABC(const char[], const char[],const char[], rclcpp::Duration durTime = 20ms);
            virtual void PubEvent(char, char, int byteSize = 6);
            virtual ~DaqABC();
        };

        DaqABC::DaqABC(const char pubTopicName[], const char subTopicName[], const char nodeName[], rclcpp::Duration durTime) 
            : Node(nodeName), durTime_(durTime), nowDur_(1ms)
        {
            publisher_ = this->create_publisher<interfaces::msg::Uint8Array>(pubTopicName, 10);
            subscriber_ = this->create_subscription<interfaces::msg::Uint8Array>(subTopicName, 10, bind(&DaqABC::CallBack, this , _1));
        }

        void DaqABC::Init(const string deviceName, BaudRate baudRate)
        {
            serial.SetDevice(deviceName);
            serial.SetBaudRate(baudRate);
        }

        void DaqABC::CallBack(interfaces::msg::Uint8Array::SharedPtr dataPtr)
        {
            serial.WriteBinary(dataPtr->data);
            
            /********打印发送的数据********/
            // ostringstream ostr;
            // ostr << "Write data size = " << dec << int(dataPtr->data.size()) << endl;
            // ostr.fill('0');        
            // ostr << "Hex:";
            // for(int i = 0; i < static_cast<int>(dataPtr->data.size()); i++){
            //     if(i != 0) ostr << " ";
            //     ostr << "0X" << hex << uppercase << setw(2) << int(dataPtr->data[i]);
            // }
            // ostr << endl;
            // ostr << "Ascii:";
            // for(int i = 0; i < static_cast<int>(dataPtr->data.size()); i++){
            //     ostr << char(dataPtr->data[i]);
            // }
            // RCLCPP_INFO(this->get_logger(), ostr.str().c_str());
            /********打印发送的数据********/

        }

        void DaqABC::PubEvent(char frameHeader, char frameTail, int byteSize)
        {
            //data.data.clear();
            data.data.erase(data.data.begin(), data.data.end());
            
            int recvByteSize;

            serial.ReadBinary(data.data);   //默认阻塞
//调试用
// PrintHexAndAsciiData(data, "First read";
//调试用
            //帧头有干扰:123@12@45$...
            if(data.data.front() != frameHeader)
            {
                auto iter = find(data.data.begin(),data.data.end(), frameHeader);
//调试用
PrintHexAndAsciiData(data, "Have noise befor frameheader");
//调试用
                data.data.erase(data.data.begin(),iter);
            }

            //@12@4$67$8...
            if(data.data.front() == frameHeader)
            {
                rclcpp::Time now = this->now();
                nowDur_ = this->now()-now;

                //find frametail until time out
                while((data.data.back() != frameTail) && (nowDur_ < durTime_))
                {
                    vector<uint8_t> tempdata;
                    serial.ReadBinary(tempdata);
                    if(!tempdata.empty())
                        now = this->now();
                    data.data.insert(data.data.end(),tempdata.begin(),tempdata.end());
                    nowDur_ = this->now()-now;
                }//  -> generate  1.have tail: @12$  @123@56$8...$  2.no tail: @12@456  @123...$..456 
//调试用
// PrintHexAndAsciiData(data, "Read head");
//调试用
                //handle no tail situation
                if(data.data.back() != frameTail)
                {
                    auto iter = data.data.rbegin();
                    for(; iter != data.data.rend(); ++iter)
                        if(*iter == frameTail)
                            break;
                    
                    //@123456789$1234
                    if(iter != data.data.rend())    //have tail but the tail not at dataend
                    {
                        string str;
                        str.assign(data.data.begin(),data.data.end());
                        data.data.erase(data.data.begin()+(data.data.rend()-iter),data.data.end());
                        RCLCPP_INFO(this->get_logger(), "erase no $ part");
                        RCLCPP_INFO(this->get_logger(), str.c_str()); 
                    }   // -> @123456789$
                }

                //@12$  @123@56$8...$   must have $ but may not frametail
                while(!data.data.empty() && (nowDur_ < durTime_))
                {
                    if(data.data.front() != frameHeader)
                    {
                        auto iter = find(data.data.begin(),data.data.end(), frameHeader);
                        data.data.erase(data.data.begin(),iter);
                        if(data.data.empty())
                            break;
                    }
                    if(byteSize > 6)
                        recvByteSize = byteSize;
                    else
                        switch(data.data[1])
                        {
                            case 'A':recvByteSize = 8;break;    //32 @ACK
                            case 'D':
                            {
                                    if(data.data[2] == 'U')     //32 @DUP
                                        recvByteSize = 68;
                                    else
                                        recvByteSize = 6;       //Shore cmd @DT @DA
                                    break;
                            }
                            case 'T':
                            {
                                    if(data.data[2] == 'P')     //Shore cmd @TP
                                        recvByteSize = 11;
                                    else if(data.data[2] == 'S') 
                                        recvByteSize = 18;       //Shore cmd @DT @DA
                                    break;
                            }   
                            default:recvByteSize = byteSize;
                        }

// ostringstream ostr;
// ostr << "data size = " << dec << int(data.data.size()) << " recvByteSize = " << recvByteSize;
// RCLCPP_INFO(this->get_logger(), ostr.str().c_str());
                    
                    if(static_cast<int>(data.data.size()) < recvByteSize) //@1234$ shorter than recvByteSize
                    {
                        vector<uint8_t> tempdata;
                        serial.ReadBinary(tempdata);
                        if(!tempdata.empty())
                        {
                            now = this->now();
                            data.data.insert(data.data.end(),tempdata.begin(),tempdata.end());
                        }
//调试用
// PrintHexAndAsciiData(data, "data size < recvByteSize");
//调试用
                    }
                    else if(static_cast<int>(data.data.size()) > recvByteSize)  //粘包 @ACK_$ -> @ACK_$1$@ACK_01$ >recvByteSize
                    {
                        if(data.data.front() == frameHeader && data.data[recvByteSize-1]==frameTail)
                        {
                            interfaces::msg::Uint8Array sendData;
                            sendData.data.assign(data.data.begin(), data.data.begin()+recvByteSize);
                            data.data.erase(data.data.begin(), data.data.begin()+recvByteSize);
                            publisher_ -> publish(sendData);
                            
//调试用
// PrintHexAndAsciiData(data, ">");
// PrintHexAndAsciiData(sendData, ">");
//调试用
                            now = this->now();
                        }
                        else    //删除begin到下一个帧头
                        {
                            auto iter = find(data.data.begin() + 1, data.data.end(), frameHeader);
                            data.data.erase(data.data.begin(), iter);
                        }
                    }
                    else if((static_cast<int>(data.data.size()) == recvByteSize) && (data.data.back() == frameTail))
                    {
                        publisher_ -> publish(data);
//调试用
// PrintHexAndAsciiData(data, "data size == recvByteSize");
// PrintHexAndAsciiData(data, "==");
//调试用
                        return;
                    }
                    
                    nowDur_ = this->now()-now;
                }

                //@1234$ short    @ACK_$ -> @ACK_$1$@ACK_01$ >recvByteSize timeout
                if(!data.data.empty() && (nowDur_ >= durTime_))
                {
/*查看超时丢弃的数据*/
PrintHexAndAsciiData(data, "Time out");
/*查看超时丢弃的数据*/             
                }
            }
        }

        void DaqABC::PrintHexAndAsciiData(interfaces::msg::Uint8Array& data, const char* str)
        {
            ostringstream ostr;
            ostr << str;
            ostr << " data size = " << dec << int(data.data.size()) << endl;
            ostr.fill('0');        
            ostr << "Hex:";
            for(int i = 0; i < static_cast<int>(data.data.size()); i++){
                if(i != 0) ostr << " ";
                ostr << "0X" << hex << uppercase << setw(2) << int(data.data[i]);
            }
            ostr << endl;
            ostr << "Ascii:";
            for(int i = 0; i < static_cast<int>(data.data.size()); i++){
                ostr << char(data.data[i]);
            }
            ostr << endl;
            RCLCPP_INFO(this->get_logger(), ostr.str().c_str());
        }

        DaqABC::~DaqABC()
        {
            serial.Close();
        }
    }
}

#endif  //DAQABC_H_