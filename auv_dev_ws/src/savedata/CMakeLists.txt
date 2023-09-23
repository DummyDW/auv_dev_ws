cmake_minimum_required(VERSION 3.8)
project(savedata)

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

#Save
add_executable(Save src/save.cpp)
ament_target_dependencies(Save rclcpp interfaces)
#target_include_directories(Save PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/savedata)

#Save
add_executable(MagSave src/magsave.cpp)
ament_target_dependencies(MagSave rclcpp interfaces)
#target_include_directories(MagSave PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/savedata)

install(TARGETS Save MagSave DESTINATION lib/${PROJECT_NAME})

ament_package()
