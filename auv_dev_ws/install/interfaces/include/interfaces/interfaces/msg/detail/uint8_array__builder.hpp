// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from interfaces:msg/Uint8Array.idl
// generated code does not contain a copyright notice

#ifndef INTERFACES__MSG__DETAIL__UINT8_ARRAY__BUILDER_HPP_
#define INTERFACES__MSG__DETAIL__UINT8_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "interfaces/msg/detail/uint8_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace interfaces
{

namespace msg
{

namespace builder
{

class Init_Uint8Array_data
{
public:
  Init_Uint8Array_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::interfaces::msg::Uint8Array data(::interfaces::msg::Uint8Array::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::interfaces::msg::Uint8Array msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::interfaces::msg::Uint8Array>()
{
  return interfaces::msg::builder::Init_Uint8Array_data();
}

}  // namespace interfaces

#endif  // INTERFACES__MSG__DETAIL__UINT8_ARRAY__BUILDER_HPP_
