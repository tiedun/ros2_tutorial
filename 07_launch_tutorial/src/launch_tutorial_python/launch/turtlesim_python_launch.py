from launch import LaunchDescription

# 声明 Launch 参数
from launch.actions import DeclareLaunchArgument

# 获取 Launch 参数对应的配置值
from launch.substitutions import LaunchConfiguration

from launch_ros.actions import Node

def generate_launch_description():
    # 声明 Launch 参数 log_level
    declare_turtlesim1_log_level = DeclareLaunchArgument(
        'turtlesim1_log_level',
        default_value='info',
        description='设置 turtlesim1 的日志级别'
    )

    declare_turtlesim2_log_level = DeclareLaunchArgument(
        'turtlesim2_log_level',
        default_value='warn',
        description='设置 turtlesim2 的日志级别'
    )

    # 引用 *log_level 对应的 Launch Configuration
    turtlesim1_log_level = LaunchConfiguration('turtlesim1_log_level')
    turtlesim2_log_level = LaunchConfiguration('turtlesim2_log_level')

    # 创建并返回 LaunchDescription
    # 其中包含需要由 Launch 系统启动的各个 ROS 2 节点
    return LaunchDescription([
        declare_turtlesim1_log_level,
        declare_turtlesim2_log_level,

        # 启动第一个 turtlesim 节点
        Node(
            # 功能包名称
            package='turtlesim',

            # 设置节点命名空间
            # 节点最终完整名称为：/turtlesim1/sim
            namespace='turtlesim1',

            # 要启动的可执行程序
            executable='turtlesim_node',

            # 设置节点名称
            name='sim',

            # 向可执行程序传递普通命令行参数
            # --ros-args 表示后续参数交给 ROS 2 处理
            # --log-level info 将日志级别设置为 info
            arguments=[
                '--ros-args',
                '--log-level',
                turtlesim1_log_level
            ]
        ),

        # 启动第二个 turtlesim 节点
        Node(
            # 功能包名称
            package='turtlesim',

            # 设置节点命名空间
            # 节点最终完整名称为：/turtlesim2/sim
            namespace='turtlesim2',

            # 要启动的可执行程序
            executable='turtlesim_node',

            # 设置节点名称
            name='sim',

            # 专门向 ROS 2 传递 ROS 参数
            # 不需要额外写 --ros-args
            # 将日志级别设置为 warn
            ros_arguments=[
                '--log-level',
                turtlesim2_log_level
            ],

            # 设置节点启动参数
            # 修改 turtlesim 窗口的背景色为豆沙绿
            parameters=[
                {'background_r': 199},
                {'background_g': 237},
                {'background_b': 204}
            ]
        ),

        # 启动 mimic 节点
        # mimic 会读取一只乌龟的运动状态，
        # 并将其中的线速度、角速度转换为 Twist 消息发布出去
        Node(
            # 功能包名称
            package='turtlesim',

            # 启动 turtlesim 包中的 mimic 可执行程序
            executable='mimic',

            # 设置节点名称
            name='mimic',

            # 对 mimic 使用的话题名称进行重映射
            remappings=[
                # mimic 默认订阅 /input/pose
                # 重映射后实际订阅第一只乌龟的 Pose
                (
                    '/input/pose',
                    '/turtlesim1/turtle1/pose'
                ),

                # mimic 默认发布 /output/cmd_vel
                # 重映射后实际向第二只乌龟发送速度指令
                (
                    '/output/cmd_vel',
                    '/turtlesim2/turtle1/cmd_vel'
                ),
            ]
        )
    ])
