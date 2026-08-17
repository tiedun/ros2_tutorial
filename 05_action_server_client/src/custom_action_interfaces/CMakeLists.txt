cmake_minimum_required(VERSION 3.8)
project(custom_action_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

# 查找 ROS 2 接口代码生成工具
# 用于根据 .msg、.srv、.action 文件生成 C++、Python 等语言对应的接口代码
find_package(rosidl_default_generators REQUIRED)

# 根据自定义接口文件生成对应的 ROS 2 接口代码
rosidl_generate_interfaces(${PROJECT_NAME}
  # 指定需要处理的自定义消息文件
  # 可以同时指定多个 .msg、.srv 和 .action 文件，用空格或换行分隔
  "action/CustomAction.action"
)

# 向下游功能包导出 ROS 2 接口运行时依赖
ament_export_dependencies(rosidl_default_runtime)

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
