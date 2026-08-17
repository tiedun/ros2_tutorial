cmake_minimum_required(VERSION 3.8)
project(action_server_client_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(custom_action_interfaces REQUIRED)

# 创建动作服务端可执行文件
# 可执行文件名：action_server
# 源文件：src/action_server.cpp
add_executable(action_server src/action_server.cpp)

# 为 action_server 添加依赖
# rclcpp_action：提供 ROS 2 C++ Action 相关 API
# custom_action_interfaces：提供 CustomAction 接口
ament_target_dependencies(
  action_server
  rclcpp
  rclcpp_action
  custom_action_interfaces
)

# 创建动作客户端可执行文件
# 可执行文件名：action_client
# 源文件：src/action_client.cpp
add_executable(action_client src/action_client.cpp)

# 为 action_client 添加依赖
ament_target_dependencies(
  action_client
  rclcpp
  rclcpp_action
  custom_action_interfaces
)

# 安装可执行文件
install(TARGETS
  action_server
  action_client

  # 安装到 install/<功能包名>/lib/<功能包名>/ 目录
  # 这样可以通过
  # ros2 run <功能包名> action_server/action_client
  # 启动对应的可执行程序
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
