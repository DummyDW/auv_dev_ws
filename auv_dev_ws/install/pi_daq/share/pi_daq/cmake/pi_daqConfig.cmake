# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_pi_daq_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED pi_daq_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(pi_daq_FOUND FALSE)
  elseif(NOT pi_daq_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(pi_daq_FOUND FALSE)
  endif()
  return()
endif()
set(_pi_daq_CONFIG_INCLUDED TRUE)

# output package information
if(NOT pi_daq_FIND_QUIETLY)
  message(STATUS "Found pi_daq: 0.0.0 (${pi_daq_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'pi_daq' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${pi_daq_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(pi_daq_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${pi_daq_DIR}/${_extra}")
endforeach()
