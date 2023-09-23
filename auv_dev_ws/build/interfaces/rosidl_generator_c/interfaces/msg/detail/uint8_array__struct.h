// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from interfaces:msg/Uint8Array.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__UINT8_ARRAY__STRUCT_H_
#define INTERFACES__MSG__DETAIL__UINT8_ARRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/Uint8Array in the package interfaces.
typedef struct interfaces__msg__Uint8Array
{
  rosidl_runtime_c__uint8__Sequence data;
} interfaces__msg__Uint8Array;

// Struct for a sequence of interfaces__msg__Uint8Array.
typedef struct interfaces__msg__Uint8Array__Sequence
{
  interfaces__msg__Uint8Array * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} interfaces__msg__Uint8Array__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // INTERFACES__MSG__DETAIL__UINT8_ARRAY__STRUCT_H_
