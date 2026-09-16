cmake_minimum_required(VERSION 3.8)
project(static_broadcaster_cpp)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2_ros REQUIRED)

# 创建可执行文件 static_tf_broadcaster
# 源文件为 src/static_tf_broadcaster.cpp
add_executable(static_tf_broadcaster src/static_tf_broadcaster.cpp)

# 为 static_tf_broadcaster 声明依赖
# rclcpp：ROS 2 C++客户端库
# geometry_msgs：提供 TransformStamped 等消息接口
# tf2_ros：提供 StaticTransformBroadcaster 等 tf2 ROS 2 接口
ament_target_dependencies(
  static_tf_broadcaster
  rclcpp
  geometry_msgs
  tf2_ros
)

# 安装可执行文件
# 安装到 install/<功能包名>/lib/<功能包名>/ 目录下，
# 使其可以通过 ros2 run <功能包名> static_tf_broadcaster 运行
install(TARGETS
  static_tf_broadcaster
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
