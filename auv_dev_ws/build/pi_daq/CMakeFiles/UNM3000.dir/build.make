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
include CMakeFiles/UNM3000.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/UNM3000.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/UNM3000.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UNM3000.dir/flags.make

CMakeFiles/UNM3000.dir/src/unm3000.cpp.o: CMakeFiles/UNM3000.dir/flags.make
CMakeFiles/UNM3000.dir/src/unm3000.cpp.o: /home/zzf/auv_dev_ws/src/pi_daq/src/unm3000.cpp
CMakeFiles/UNM3000.dir/src/unm3000.cpp.o: CMakeFiles/UNM3000.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/zzf/auv_dev_ws/build/pi_daq/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/UNM3000.dir/src/unm3000.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/UNM3000.dir/src/unm3000.cpp.o -MF CMakeFiles/UNM3000.dir/src/unm3000.cpp.o.d -o CMakeFiles/UNM3000.dir/src/unm3000.cpp.o -c /home/zzf/auv_dev_ws/src/pi_daq/src/unm3000.cpp

CMakeFiles/UNM3000.dir/src/unm3000.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UNM3000.dir/src/unm3000.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/zzf/auv_dev_ws/src/pi_daq/src/unm3000.cpp > CMakeFiles/UNM3000.dir/src/unm3000.cpp.i

CMakeFiles/UNM3000.dir/src/unm3000.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UNM3000.dir/src/unm3000.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/zzf/auv_dev_ws/src/pi_daq/src/unm3000.cpp -o CMakeFiles/UNM3000.dir/src/unm3000.cpp.s

# Object files for target UNM3000
UNM3000_OBJECTS = \
"CMakeFiles/UNM3000.dir/src/unm3000.cpp.o"

# External object files for target UNM3000
UNM3000_EXTERNAL_OBJECTS =

UNM3000: CMakeFiles/UNM3000.dir/src/unm3000.cpp.o
UNM3000: CMakeFiles/UNM3000.dir/build.make
UNM3000: /opt/ros/humble/lib/librclcpp.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_fastrtps_c.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_introspection_c.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_introspection_cpp.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_cpp.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/liblibstatistics_collector.so
UNM3000: /opt/ros/humble/lib/librcl.so
UNM3000: /opt/ros/humble/lib/librmw_implementation.so
UNM3000: /opt/ros/humble/lib/libament_index_cpp.so
UNM3000: /opt/ros/humble/lib/librcl_logging_spdlog.so
UNM3000: /opt/ros/humble/lib/librcl_logging_interface.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_cpp.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/librcl_interfaces__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/librcl_yaml_param_parser.so
UNM3000: /opt/ros/humble/lib/libyaml.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_cpp.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/librosgraph_msgs__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_cpp.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/libstatistics_msgs__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/libtracetools.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/librosidl_typesupport_fastrtps_c.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/librosidl_typesupport_fastrtps_cpp.so
UNM3000: /opt/ros/humble/lib/libfastcdr.so.1.0.24
UNM3000: /opt/ros/humble/lib/librmw.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/librosidl_typesupport_introspection_cpp.so
UNM3000: /opt/ros/humble/lib/librosidl_typesupport_introspection_c.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_cpp.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_cpp.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_cpp.so
UNM3000: /opt/ros/humble/lib/librosidl_typesupport_cpp.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_typesupport_c.so
UNM3000: /home/zzf/auv_dev_ws/install/interfaces/lib/libinterfaces__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/libgeometry_msgs__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_generator_py.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/libstd_msgs__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/libbuiltin_interfaces__rosidl_generator_c.so
UNM3000: /opt/ros/humble/lib/librosidl_typesupport_c.so
UNM3000: /opt/ros/humble/lib/librcpputils.so
UNM3000: /opt/ros/humble/lib/librosidl_runtime_c.so
UNM3000: /opt/ros/humble/lib/librcutils.so
UNM3000: /usr/lib/aarch64-linux-gnu/libpython3.10.so
UNM3000: CMakeFiles/UNM3000.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/zzf/auv_dev_ws/build/pi_daq/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable UNM3000"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UNM3000.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/UNM3000.dir/build: UNM3000
.PHONY : CMakeFiles/UNM3000.dir/build

CMakeFiles/UNM3000.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UNM3000.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UNM3000.dir/clean

CMakeFiles/UNM3000.dir/depend:
	cd /home/zzf/auv_dev_ws/build/pi_daq && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/zzf/auv_dev_ws/src/pi_daq /home/zzf/auv_dev_ws/src/pi_daq /home/zzf/auv_dev_ws/build/pi_daq /home/zzf/auv_dev_ws/build/pi_daq /home/zzf/auv_dev_ws/build/pi_daq/CMakeFiles/UNM3000.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/UNM3000.dir/depend
