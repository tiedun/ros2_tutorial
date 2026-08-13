cmake_minimum_required(VERSION 3.8)
project(server_client_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)

# 创建服务端可执行文件
# 可执行文件名：server
# 源文件：src/server.cpp
add_executable(server src/server.cpp)

# 为 server 添加依赖
# example_interfaces：提供 AddTwoInts 等示例接口
ament_target_dependencies(server rclcpp example_interfaces)

# 创建客户端可执行文件
# 可执行文件名：client
# 源文件：src/client.cpp
add_executable(client src/client.cpp)

# 为 client 添加依赖
ament_target_dependencies(client rclcpp example_interfaces)

# 安装可执行文件
install(TARGETS
  server
  client
  # 安装到 install/<功能包名>/lib/<功能包名>/ 目录
  # 这样可以通过 ros2 run <功能包名> server/client 启动
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
