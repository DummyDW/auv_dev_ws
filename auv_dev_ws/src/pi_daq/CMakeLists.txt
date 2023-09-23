cmake_minimum_required(VERSION 3.5)
project(pi_daq)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

#前置磁力仪
add_executable(Magneto src/magneto.cpp)
ament_target_dependencies(Magneto rclcpp interfaces)
target_link_libraries(Magneto CppLinuxSerial bcm2835)
target_include_directories(Magneto PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#BDGPS
add_executable(BDGPS src/bdgps.cpp)
ament_target_dependencies(BDGPS rclcpp interfaces)
target_link_libraries(BDGPS CppLinuxSerial)
target_include_directories(BDGPS PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#EX3040
add_executable(EX3040 src/ex3040.cpp)
ament_target_dependencies(EX3040 rclcpp interfaces)
target_link_libraries(EX3040 CppLinuxSerial)
target_include_directories(EX3040 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#P360
add_executable(P360 src/p360.cpp)
ament_target_dependencies(P360 rclcpp interfaces)
target_link_libraries(P360 CppLinuxSerial)
target_include_directories(P360 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#STM32
add_executable(STM32 src/stm32.cpp)
ament_target_dependencies(STM32 rclcpp interfaces)
target_link_libraries(STM32 CppLinuxSerial)
target_include_directories(STM32 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#UHF
add_executable(UHF src/uhf.cpp)
ament_target_dependencies(UHF rclcpp interfaces)
target_link_libraries(UHF CppLinuxSerial)
target_include_directories(UHF PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#UNM3000
add_executable(UNM3000 src/unm3000.cpp)
ament_target_dependencies(UNM3000 rclcpp interfaces)
target_link_libraries(UNM3000 CppLinuxSerial)
target_include_directories(UNM3000 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Camera
add_executable(Camera src/camera.cpp)
ament_target_dependencies(Camera rclcpp interfaces)
target_link_libraries(Camera CppLinuxSerial bcm2835)
target_include_directories(Camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Pi
add_executable(Pi src/pi.cpp)
ament_target_dependencies(Pi rclcpp interfaces)
target_link_libraries(Pi CppLinuxSerial)
target_include_directories(Pi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Server
add_executable(Server src/server.cpp)
ament_target_dependencies(Server rclcpp interfaces)
target_include_directories(Server PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS Magneto BDGPS EX3040 P360 STM32 UHF UNM3000 Camera Pi Server DESTINATION lib/${PROJECT_NAME})

ament_package()
