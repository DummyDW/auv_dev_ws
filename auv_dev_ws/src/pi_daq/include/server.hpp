/*
 * @Author       : zzhef
 * @Date         : 2023-06-12 10:25:46
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-06-12 11:58:06
 * @FilePath     : /auv_dev_ws/src/pi_daq/include/server.hpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <cstdlib>
#include <sys/wait.h>
#include <ctime>
#include <cstdio>
#include <unistd.h>
#include <mutex>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "rclcpp/rclcpp.hpp"
#include "interfaces/msg/uint8_array.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;
using std::vector;
using std::string;
using std::to_string;

#define MAXSERVERTOPIDATASIZE 50
#define MAXPITOSERVERDATASIZE 300

class Server : public rclcpp::Node
{
private:
    rclcpp::Publisher<interfaces::msg::Uint8Array>::SharedPtr publisher_Pi_;
    rclcpp::Subscription<interfaces::msg::Uint8Array>::SharedPtr subscriber_PC_;

    int val = 1;
    char shoreToPiBuf[MAXSERVERTOPIDATASIZE];
    char piToShoreBuf[MAXPITOSERVERDATASIZE];

    vector<pid_t> childProcesses = {-1, -1, -1, -1, -1, -1, -1};
    vector<bool> childrenAlive = {false, false, false, false, false, false, false};
    enum childrenName{PI, STM32, MAGNETO, EX3040, CAMERA, BDGPS, P360, SAVE, MAGSAVE};

    void CreateChildrenProcesses(char createChar);
    void CreateChildProcess(const string &str, childrenName childName);
    void TerminateChildrenProcesses(char terminateChar);
    void TerminateChildProcess(const string &str, childrenName childName, int &status);
    
public:
    bool connectFlag = true;
    int listenfd, connectfd, recvbytes;
    socklen_t addrlen;
    struct sockaddr_in client;
    struct sockaddr_in server;
    Server();
    void PiToShore(interfaces::msg::Uint8Array::SharedPtr);
    void ShoreToPi();
    vector<pid_t> getChildProcessesVec() {return childProcesses;}
    vector<bool> getChildrenAliveVec() {return childrenAlive;}
};

Server::Server() : Node("Server")
{
    publisher_Pi_ = this->create_publisher<interfaces::msg::Uint8Array>("Shore_to_Pi", 10);
    subscriber_PC_ = this->create_subscription<interfaces::msg::Uint8Array>("Pi_to_Shore", 10, std::bind(&Server::PiToShore, this , _1));

	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)  //创建一个流式套接字
	{
		perror("socket");
		exit(1);
	}

    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (char *)&val, sizeof(val));  //设置SO_REUSEADDR选项值为1，即地址可重用
    //避免在终止后的一段时间内等待操作系统释放之前绑定的地址和端口。

	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("0.0.0.0");
    bzero(&(server.sin_zero), 8);   //用于填充，以保证struct sockaddr_in的大小与较早版本的struct sockaddr相同。

	if(bind(listenfd, (struct sockaddr *)&server, sizeof(server)) == -1)    //将服务器端所在主机地址和端口号绑定在该套接字上。
	{
		perror("bind");
		exit(1);
	}
	
    if(listen(listenfd, 1) == -1)   //将套接字转换为被动监听套接字，并在对应端口开始监听。
    {
        perror("listen");
        exit(1);
    }
    
    addrlen = sizeof(client);
}

/*
    订阅Pi_to_Shore话题的回调函数 @VS...$
*/
void Server::PiToShore(interfaces::msg::Uint8Array::SharedPtr dataPtr)
{
    std::lock_guard<std::mutex> lock(std::mutex);
    int dataSize = static_cast<int>(dataPtr->data.size());
    for(int i = 0; i < dataSize; i++)
        piToShoreBuf[i] = char(dataPtr->data[i]);

    if(send(connectfd, piToShoreBuf, dataSize, 0) == -1)
        perror("send");

    //RCLCPP_INFO(this->get_logger(), piToShoreBuf);
}

/*
    将从客户端传来的消息帧发布到Shore_to_Pi
*/
void Server::ShoreToPi()
{
    interfaces::msg::Uint8Array sendData;

    if((recvbytes = recv(connectfd, shoreToPiBuf, MAXSERVERTOPIDATASIZE, 0)) >= 6)   //@STA1$
    {
        if(shoreToPiBuf[1] == 'S' && shoreToPiBuf[2] == 'T')    //开启或关闭节点指令
        {
            if(shoreToPiBuf[4] == '1')
                CreateChildrenProcesses(shoreToPiBuf[3]);
            else if(shoreToPiBuf[4] == '0')
                TerminateChildrenProcesses(shoreToPiBuf[3]);
            {
                std::lock_guard<std::mutex> lock(std::mutex);
                char ack[8]{'@', 'A', 'C', 'K', '_', 'S', 'T', '$'};
                if(send(connectfd, ack, sizeof(ack), 0) == -1)
                    perror("send");
            }
        }
        shoreToPiBuf[recvbytes] = '\0';
        for(int i = 0; i < recvbytes; i++)
            sendData.data.push_back(shoreToPiBuf[i]);
        publisher_Pi_->publish(sendData);
        //RCLCPP_INFO(this->get_logger(), shoreToPiBuf);
    }
    else if(recvbytes == 0)     //recv返回0，则表明连接已关闭
        connectFlag = false;

    //recv返回-1，说明非阻塞模式下没有接收到数据
}

void Server::CreateChildrenProcesses(char createChar) 
{
    switch(createChar)
    {
        case 'A':   CreateChildProcess("Pi", PI);    break;
        case 'B':   CreateChildProcess("STM32", STM32);    break;
        case 'C':   CreateChildProcess("Magneto", MAGNETO);    break;
        case 'D':   CreateChildProcess("EX3040", EX3040);    break;
        case 'E':   CreateChildProcess("Camera", CAMERA);    break;
        case 'F':   CreateChildProcess("BDGPS", BDGPS);    break;
        case 'G':   CreateChildProcess("P360", P360);    break;
        case 'H':   CreateChildProcess("Save", SAVE);    break;
        case 'I':   CreateChildProcess("MagSave", MAGSAVE);    break;
    }
}

void Server::CreateChildProcess(const string &str, childrenName childName)
{
    if(!childrenAlive[childName])
    {
        pid_t pid = fork();
        if (pid == -1)
            std::cerr << "Failed to create child process." << std::endl;
        else if (pid == 0)
        {
            if(str == "Save" || str == "MagSave")
                execl(string("/home/zzf/auv_dev_ws/build/savedata/" + str).c_str(), str.c_str(), NULL);
            else
                execl(string("/home/zzf/auv_dev_ws/build/pi_daq/" + str).c_str(), str.c_str(), NULL);
            perror(string("execl " + str).c_str());
        }
        else
        {
            std::cout << "create " + str + " PID: " << pid << std::endl;
            childProcesses[childName] = pid;    
            childrenAlive[childName] = true;
        }
    }
    else
        std::cout << str + "节点已创建，不要重复创建！" << std::endl;
}

void Server::TerminateChildrenProcesses(char terminateChar) 
{
    int status;

    switch(terminateChar)
    {
        case 'A':    TerminateChildProcess("Pi", PI, status);    break;
        case 'B':    TerminateChildProcess("STM32", STM32, status);    break;
        case 'C':    TerminateChildProcess("Magneto", MAGNETO, status);    break;
        case 'D':    TerminateChildProcess("EX3040", EX3040, status);    break;
        case 'E':    TerminateChildProcess("Camera", CAMERA, status);    break;
        case 'F':    TerminateChildProcess("BDGPS", BDGPS, status);    break;
        case 'G':    TerminateChildProcess("P360", P360, status);    break;
        case 'H':    TerminateChildProcess("Save", SAVE, status);    break;
        case 'I':    TerminateChildProcess("MagSave", MAGSAVE, status);    break;
    }
}

void Server::TerminateChildProcess(const string &str, childrenName childName, int &status)
{
    if(childrenAlive[childName]) 
    {
        std::cout << "kill " + str + " PID: " << childProcesses[childName] << std::endl;
        kill(childProcesses[childName], SIGTERM);
        waitpid(childProcesses[childName], &status, 0);  // 等待子进程退出
        childrenAlive[childName] = false;
    }
    else
        std::cout << str + "节点已删除，不要重复删除！" << std::endl;
}