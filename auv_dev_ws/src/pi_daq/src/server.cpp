/*
 * @Author       : zzhef
 * @Date         : 2023-06-12 10:25:54
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-06-12 11:05:40
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/server.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "server.hpp"
#include <fcntl.h> 
#include <unistd.h>
#include <signal.h>
 
void PubShoreToPiThread(std::shared_ptr<Server> &node);
void SubPiToShoreThread(std::shared_ptr<Server> &node);
void sigint_handler(int signum);

int listenfd;
vector<pid_t> childProcesses = {-1, -1, -1, -1, -1, -1, -1};
vector<bool> childrenAlive = {false, false, false, false, false, false, false};

int main(int argc, char * argv[])
{
    signal(SIGINT, sigint_handler);
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Server>();     //创建节点自动开始监听
    listenfd = node->listenfd;

    while(rclcpp::ok())
    {
        //如果没有客户端连接请求则阻塞。收到请求后，服务进程则从accept阻塞状态被唤醒，生成一个新的连接套接字
        //用连接套接字同客户端的套接字建立连接，之前那个套接字则继续用于监听网络上的连接请求。
        std::cout << "等待客户端连接..." << std::endl;
        if((node->connectfd = accept(listenfd, (struct sockaddr *)&(node->client),&(node->addrlen))) == -1)
        {
            printf("connect() error \n");
            return -1;
        }
        std::cout << "连接成功！" << std::endl;
        node->connectFlag = true;
        // fcntl(node->connectfd, F_SETFL, O_NONBLOCK); //非阻塞

        std::thread shoreToPiThreadObj(PubShoreToPiThread, std::ref(node));
        std::thread piToShoreThreadObj(SubPiToShoreThread, std::ref(node));

        shoreToPiThreadObj.join();
        piToShoreThreadObj.join();

        childProcesses = node->getChildProcessesVec();
        childrenAlive = node->getChildrenAliveVec();

        std::cout << "连接断开！" << std::endl;

        //如果客户端关闭连接则重新回到开头进入accept阻塞状态
    }
    rclcpp::shutdown();
    return 0;
}

void PubShoreToPiThread(std::shared_ptr<Server> &node)
{
    std::cout << "进入发布事件循环" << std::endl;
    while(node->connectFlag)
        node->ShoreToPi();
    std::cout << "退出发布事件循环" << std::endl;
}

void SubPiToShoreThread(std::shared_ptr<Server> &node)
{
    std::cout << "进入订阅事件循环" << std::endl;
    while(node->connectFlag)
        rclcpp::spin_some(node);
    std::cout << "退出订阅事件循环" << std::endl;
}

void sigint_handler(int signum)
{
    std::cout << "signum:" << signum << std::endl;
    // Ctrl C 后进行清理操作或其他需要的处理
    //关闭监听套接字
    close(listenfd);
    
    //如果还有子进程存活，则终止
    for(int i = 0; i < static_cast<int>(childProcesses.size()); i++)
    {
        if(childrenAlive[i])
        {
            std::cout << "kill " + std::to_string(i) + " PID: " << childProcesses[i] << std::endl;
            kill(childProcesses[i], SIGTERM);
            waitpid(childProcesses[i], NULL, 0);  // 等待子进程退出
        }
    }
    exit(0);
}