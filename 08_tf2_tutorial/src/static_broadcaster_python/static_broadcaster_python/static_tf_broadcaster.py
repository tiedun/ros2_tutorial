import rclpy
from rclpy.node import Node

from geometry_msgs.msg import TransformStamped
from tf2_ros.static_transform_broadcaster import StaticTransformBroadcaster


class StaticTFBroadcaster(Node):
    def __init__(self):
        super().__init__('static_tf_broadcaster_python')

        # 创建静态Transform广播器，并关联到当前节点
        self.tf_static_broadcaster = StaticTransformBroadcaster(self)

        # 创建一条带时间戳的Transform消息
        transform = TransformStamped()

        # 设置消息时间戳
        transform.header.stamp = self.get_clock().now().to_msg()

        # 设置父Frame和子Frame
        transform.header.frame_id = 'base_link'
        transform.child_frame_id = 'base_laser'

        # 设置平移分量，单位为m
        transform.transform.translation.x = 0.1
        transform.transform.translation.y = 0.0
        transform.transform.translation.z = 0.2

        # 设置旋转分量：单位四元数表示没有旋转
        transform.transform.rotation.x = 0.0
        transform.transform.rotation.y = 0.0
        transform.transform.rotation.z = 0.0
        transform.transform.rotation.w = 1.0

        # 发布静态Transform
        self.tf_static_broadcaster.sendTransform(transform)

        # 打印日志
        self.get_logger().info(
            '发布静态变换: base_link -> base_laser'
        )

def main(args=None):
    rclpy.init(args=args)

    node = StaticTFBroadcaster()

    # 保持节点运行
    rclpy.spin(node)

    node.destroy_node()
    rclpy.shutdown()
