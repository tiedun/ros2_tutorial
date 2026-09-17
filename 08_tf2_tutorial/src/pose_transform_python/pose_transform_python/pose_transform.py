import math

import rclpy
from rclpy.node import Node

from geometry_msgs.msg import PoseStamped

from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener

# 为PoseStamped等geometry_msgs消息注册tf2转换函数
import tf2_geometry_msgs

class PoseTransform(Node):
    def __init__(self):
        # 初始化节点
        super().__init__('pose_transform_python')

        # 创建Buffer
        self.tf_buffer = Buffer(node=self)

        # 创建TransformListener
        self.tf_listener = TransformListener(
            self.tf_buffer,
            self,
            spin_thread=False
        )

        # 创建定时器，每1秒尝试执行一次位姿转换
        # 转换成功后取消定时器
        self.timer = self.create_timer(
            1.0,
            self.transform_pose
        )

    def transform_pose(self):
        # 创建camera_link中的目标位姿
        pose = PoseStamped()

        # 设置数据所属的Frame
        pose.header.frame_id = 'camera_link'

        # 设置数据对应的时间
        pose.header.stamp = self.get_clock().now().to_msg()

        # 设置目标位置：[1.0, 0.0, 0.0]
        pose.pose.position.x = 1.0
        pose.pose.position.y = 0.0
        pose.pose.position.z = 0.0

        # 设置目标姿态：绕Z轴旋转90°
        pose.pose.orientation.x = 0.0
        pose.pose.orientation.y = 0.0
        pose.pose.orientation.z = math.sin(math.pi / 4)
        pose.pose.orientation.w = math.cos(math.pi / 4)

        try:
            # 根据pose中的Frame和时间查询Transform
            # 并将目标位姿从camera_link转换到base_link
            result = self.tf_buffer.transform(
                pose,
                'base_link'
            )

            # 输出目标在camera_link中的位置
            self.get_logger().info(
                '目标在 camera_link 中的 position: '
                f'[{pose.pose.position.x:.2f}, '
                f'{pose.pose.position.y:.2f}, '
                f'{pose.pose.position.z:.2f}]'
            )

            # 输出目标在camera_link中的姿态
            self.get_logger().info(
                '目标在 camera_link 中的 orientation: '
                f'[{pose.pose.orientation.x:.3f}, '
                f'{pose.pose.orientation.y:.3f}, '
                f'{pose.pose.orientation.z:.3f}, '
                f'{pose.pose.orientation.w:.3f}]'
            )

            # 输出目标在base_link中的位置
            self.get_logger().info(
                '目标在 base_link 中的 position: '
                f'[{result.pose.position.x:.2f}, '
                f'{result.pose.position.y:.2f}, '
                f'{result.pose.position.z:.2f}]'
            )

            # 输出目标在base_link中的姿态
            self.get_logger().info(
                '目标在 base_link 中的 orientation: '
                f'[{result.pose.orientation.x:.3f}, '
                f'{result.pose.orientation.y:.3f}, '
                f'{result.pose.orientation.z:.3f}, '
                f'{result.pose.orientation.w:.3f}]'
            )

            # 转换成功后取消定时器
            self.timer.cancel()

        except TransformException as ex:
            # Buffer中暂时没有可用于转换的Transform时输出警告
            # 定时器下一次触发时会继续尝试
            self.get_logger().warn(
                f'位姿转换失败：{ex}'
            )

def main(args=None):
    # 初始化ROS 2
    rclpy.init(args=args)

    # 创建节点
    node = PoseTransform()

    # 进入事件循环，处理订阅和定时器等回调
    rclpy.spin(node)

    # 销毁节点并关闭ROS 2
    node.destroy_node()
    rclpy.shutdown()
