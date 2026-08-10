# 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.8)

# 定义当前 CMake 工程名称
project(first_cpp_node)

# 查找 ROS 2 的 rclcpp 库及其 CMake 配置
find_package(rclcpp REQUIRED)

# 将 first_node.cpp 编译为可执行文件 first_node
add_executable(first_node first_node.cpp)

# 为 first_node 链接 rclcpp
target_link_libraries(
  first_node
  rclcpp::rclcpp
)
