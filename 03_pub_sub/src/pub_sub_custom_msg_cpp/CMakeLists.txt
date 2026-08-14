cmake_minimum_required(VERSION 3.8)
project(pub_sub_custom_msg_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(custom_msg_interfaces REQUIRED)

# 将 publisher.cpp 编译为可执行程序 publisher
add_executable(publisher src/publisher.cpp)
# 为 publisher 添加 rclcpp 和 custom_msg_interfaces 依赖
ament_target_dependencies(publisher rclcpp custom_msg_interfaces)

# 将 subscriber.cpp 编译为可执行程序 subscriber
add_executable(subscriber src/subscriber.cpp)
# 为 subscriber 添加 rclcpp 和 custom_msg_interfaces 依赖
ament_target_dependencies(subscriber rclcpp custom_msg_interfaces)

# 安装 publisher 和 subscriber 可执行程序
install(TARGETS 
  publisher
  subscriber

  # 安装到 install/<功能包名>/lib/<功能包名>/ 目录
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
