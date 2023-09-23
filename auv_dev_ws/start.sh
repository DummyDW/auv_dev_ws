#!/bin/bash

sudo chmod 777 /dev/ttyAMA*
#sudo ifconfig eth0 192.168.1.108 netmask 255.255.255.0

source /opt/ros/humble/setup.bash
source /home/zzf/auv_dev_ws/install/setup.bash
source /usr/share/colcon_cd/function/colcon_cd.sh
export HOME=/home/zzf
export ROS_LOG_DIR=/home/zzf/auv_dev_ws/log
export _colcon_cd_root=/opt/ros/humble/
#export DISPLAY="192.168.1.200:0.0"

cd /home/zzf/auv_dev_ws/build/pi_daq
./Server &

exit 0