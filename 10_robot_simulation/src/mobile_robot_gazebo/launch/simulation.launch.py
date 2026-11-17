from launch import LaunchDescription
from launch.actions import (
    ExecuteProcess,
    TimerAction,
    RegisterEventHandler
)
from launch.event_handlers import OnProcessStart
from launch.substitutions import (
    Command,
    FindExecutable,
    PathJoinSubstitution
)

from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():
    # 获取功能包的共享目录
    description_share = FindPackageShare(
        'mobile_robot_description'
    )

    gazebo_share = FindPackageShare(
        'mobile_robot_gazebo'
    )

    # Xacro入口文件
    model_path = PathJoinSubstitution([
        description_share,
        'urdf',
        'mobile_robot.urdf.xacro'
    ])

    # Gazebo世界文件
    world_file = PathJoinSubstitution([
        gazebo_share,
        'worlds',
        'mobile_robot_world.sdf'
    ])

    # RViz2配置文件
    rviz_config = PathJoinSubstitution([
        gazebo_share,
        'rviz',
        'mobile_robot_simulation.rviz'
    ])

    # ROS 2与Gazebo通信配置
    bridge_config = PathJoinSubstitution([
        gazebo_share,
        'config',
        'bridge.yaml'
    ])

    # 动态展开Xacro，生成机器人描述
    robot_description = ParameterValue(
        Command([
            FindExecutable(name='xacro'),
            ' ',
            model_path
        ]),
        value_type=str
    )

    # 启动Gazebo
    gazebo = ExecuteProcess(
        cmd=[
            'gz',
            'sim',
            '-r',
            '-v',
            '4',
            world_file,
            '--render-engine-gui',
            'ogre',
            '--render-engine-server',
            'ogre2'
        ],
        additional_env={
            'MESA_EXTENSION_OVERRIDE':
                '-GL_ARB_multi_draw_indirect'
        },
        output='screen'
    )

    # 发布机器人模型及TF
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[{
            'robot_description': robot_description,
            'use_sim_time': True
        }]
    )

    # 建立Gazebo与ROS 2通信桥接
    bridge = Node(
        package='ros_gz_bridge',
        executable='parameter_bridge',
        name='ros_gz_bridge',
        parameters=[{
            'config_file': bridge_config
        }],
        output='screen'
    )

    # 从robot_description话题创建Gazebo机器人
    create_robot = Node(
        package='ros_gz_sim',
        executable='create',
        arguments=[
            '--world',
            'mobile_robot_world',
            '--topic',
            '/robot_description',
            '--name',
            'mobile_robot_complete',
            '--x',
            '0',
            '--y',
            '0',
            '--z',
            '0.05'
        ],
        output='screen'
    )

    # Gazebo启动后，等待5秒再创建机器人
    spawn_robot_after_gazebo_start = RegisterEventHandler(
        OnProcessStart(
            target_action=gazebo,
            on_start=[
                TimerAction(
                    period=5.0,
                    actions=[create_robot]
                )
            ]
        )
    )

    # 启动RViz2
    rviz = Node(
        package='rviz2',
        executable='rviz2',
        output='screen',
        arguments=[
            '-d',
            rviz_config
        ],
        parameters=[{
            'use_sim_time': True
        }]
    )

    return LaunchDescription([
        spawn_robot_after_gazebo_start,
        gazebo,
        robot_state_publisher,
        bridge,
        rviz
    ])
