/*
 * @Author       : zzhef
 * @Date         : 2023-03-23 18:22:25
 * @LastEditors  : zzhef
 * @LastEditTime : 2023-03-23 19:36:40
 * @FilePath     : /auv_dev_ws/src/pi_daq/src/pi.cpp
 * @Description  : 
 * 
 * Copyright (c) 2023 by sigma team, All Rights Reserved. 
 */
#include "pi.hpp"
 
int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Pi>());
  rclcpp::shutdown();
  return 0;
}