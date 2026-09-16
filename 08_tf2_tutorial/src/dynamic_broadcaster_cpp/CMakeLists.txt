cmake_minimum_required(VERSION 3.8)
project(dynamic_broadcaster_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

add_executable(
  dynamic_tf_broadcaster
  src/dynamic_tf_broadcaster.cpp
)

# 声明 dynamic_tf_broadcaster 目标依赖的 ROS 2 功能包
ament_target_dependencies(
  dynamic_tf_broadcaster
  rclcpp
  geometry_msgs
  tf2_ros
)

# 安装可执行文件
# 安装后可通过 ros2 run dynamic_broadcaster_cpp dynamic_tf_broadcaster 运行
install(
  TARGETS
  dynamic_tf_broadcaster
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
