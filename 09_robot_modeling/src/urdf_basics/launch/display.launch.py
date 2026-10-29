from launch import LaunchDescription
from launch.substitutions import Command, FindExecutable, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    # 获取功能包的共享目录
    pkg_share = FindPackageShare('urdf_basics')

    # Xacro入口文件
    model_path = PathJoinSubstitution([
        pkg_share, 'urdf', 'mobile_robot.urdf.xacro'
    ])

    # RViz2配置文件
    rviz_config = PathJoinSubstitution([
        pkg_share, 'rviz', 'complete_robot.rviz'
    ])

    # 使用Xacro生成机器人描述
    robot_description = ParameterValue(
        Command([
            FindExecutable(name='xacro'),
            ' ',
            model_path
        ]),
        value_type=str
    )

    return LaunchDescription([
        # 发布机器人模型及TF
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            output='screen',
            parameters=[{
                'robot_description': robot_description
            }]
        ),

        # 发布关节状态
        Node(
            package='joint_state_publisher_gui',
            executable='joint_state_publisher_gui',
            output='screen'
        ),

        # 启动RViz2
        Node(
            package='rviz2',
            executable='rviz2',
            output='screen',
            arguments=['-d', rviz_config]
        )
    ])
