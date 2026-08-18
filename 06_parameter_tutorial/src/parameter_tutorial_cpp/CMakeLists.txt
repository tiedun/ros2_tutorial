cmake_minimum_required(VERSION 3.8)
project(parameter_tutorial_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)

# 创建参数节点可执行文件
# 可执行文件名：parameter_node
# 源文件：src/parameter_node.cpp
add_executable(parameter_node src/parameter_node.cpp)

# 为 parameter_node 添加依赖
# rclcpp：提供 ROS 2 C++ 客户端库及参数相关基础 API
# rcl_interfaces：提供 ParameterDescriptor 等参数接口类型
ament_target_dependencies(parameter_node rclcpp rcl_interfaces)

# 安装可执行文件
install(TARGETS
  parameter_node

  # 安装到 install/<功能包名>/lib/<功能包名>/ 目录
  # 这样可以通过 ros2 run <功能包名> parameter_node 启动
  DESTINATION lib/${PROJECT_NAME}
)

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

ament_package()
