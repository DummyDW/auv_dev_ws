# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/zzf/auv_dev_ws/src/pi_daq

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/zzf/auv_dev_ws/build/pi_daq

# Include any dependencies generated for this target.
include CMakeFiles/STM32.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/STM32.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/STM32.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/STM32.dir/flags.make

CMakeFiles/STM32.dir/src/stm32.cpp.o: CMakeFiles/STM32.dir/flags.make
CMakeFiles/STM32.dir/src/stm32.cpp.o: /home/zzf/auv_dev_ws/src/pi_daq/src/stm32.cpp
CMakeFiles/STM32.dir/src/stm32.cpp.o: CMakeFiles/STM32.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zzf/auv_dev_ws/build/pi_daq/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/STM32.dir/src/stm32.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/STM32.dir/src/stm32.cpp.o -MF CMakeFiles/STM32.dir/src/stm32.cpp.o.d -o CMakeFiles/STM32.dir/src/stm32.cpp.o -c /home/zzf/auv_dev_ws/src/pi_daq/src/stm32.cpp

CMakeFiles/STM32.dir/src/stm32.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STM32.dir/src/stm32.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zzf/auv_dev_ws/src/pi_daq/src/stm32.cpp > CMakeFiles/STM32.dir/src/stm32.cpp.i

CMakeFiles/STM32.dir/src/stm32.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STM32.dir/src/stm32.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zzf/auv_dev_ws/src/pi_daq/src/stm32.cpp -o CMakeFiles/STM32.dir/src/stm32.cpp.s

# Object files for target STM32
STM32_OBJECTS = \
"CMakeFiles/STM32.dir/src/stm32.cpp.o"

# External object files for target STM32
STM32_EXTERNAL_OBJECTS =

STM32: CMakeFiles/STM32.dir/src/stm32.cpp.o
STM32: CMakeFiles/STM32.dir/build.make
STM32: /opt/ros/humble/lib/librclcpp.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_fastrtps_c.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_fastrtps_cpp.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_introspection_c.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_introspection_cpp.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_cpp.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/liblibstatistics_collector.so
STM32: /opt/ros/humble/lib/librcl.so
STM32: /opt/ros/humble/lib/librmw_implementation.so
STM32: /opt/ros/humble/lib/libament_index_cpp.so
STM32: /opt/ros/humble/lib/librcl_logging_spdlog.so
STM32: /opt/ros/humble/lib/librcl_logging_interface.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_cpp.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/librcl_interfaces__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/librcl_yaml_param_parser.so
STM32: /opt/ros/humble/lib/libyaml.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_cpp.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/librosgraph_msgs__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_cpp.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/libstatistics_msgs__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/libtracetools.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/librosidl_typesupport_fastrtps_c.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/librosidl_typesupport_fastrtps_cpp.so
STM32: /opt/ros/humble/lib/libfastcdr.so.1.0.24
STM32: /opt/ros/humble/lib/librmw.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/librosidl_typesupport_introspection_cpp.so
STM32: /opt/ros/humble/lib/librosidl_typesupport_introspection_c.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_cpp.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_cpp.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_cpp.so
STM32: /opt/ros/humble/lib/librosidl_typesupport_cpp.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_c.so
STM32: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/libgeometry_msgs__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_generator_py.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/libstd_msgs__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_generator_c.so
STM32: /opt/ros/humble/lib/librosidl_typesupport_c.so
STM32: /opt/ros/humble/lib/librcpputils.so
STM32: /opt/ros/humble/lib/librosidl_runtime_c.so
STM32: /opt/ros/humble/lib/librcutils.so
STM32: /usr/lib/aarch64-linux-gnu/libpython3.10.so
STM32: CMakeFiles/STM32.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zzf/auv_dev_ws/build/pi_daq/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable STM32"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/STM32.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/STM32.dir/build: STM32
.PHONY : CMakeFiles/STM32.dir/build

CMakeFiles/STM32.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/STM32.dir/cmake_clean.cmake
.PHONY : CMakeFiles/STM32.dir/clean

CMakeFiles/STM32.dir/depend:
	cd /home/zzf/auv_dev_ws/build/pi_daq && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zzf/auv_dev_ws/src/pi_daq /home/zzf/auv_dev_ws/src/pi_daq /home/zzf/auv_dev_ws/build/pi_daq /home/zzf/auv_dev_ws/build/pi_daq /home/zzf/auv_dev_ws/build/pi_daq/CMakeFiles/STM32.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/STM32.dir/depend

