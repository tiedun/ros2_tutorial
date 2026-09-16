import rclpy
from rclpy.node import Node

from geometry_msgs.msg import TransformStamped
from tf2_ros import TransformBroadcaster

class DynamicTFBroadcaster(Node):
    def __init__(self):
        # 初始化节点，节点名为dynamic_tf_broadcaster_python
        super().__init__('dynamic_tf_broadcaster_python')

        # 创建动态Transform广播器，并关联到当前节点
        # TransformBroadcaster发布的Transform会进入/tf话题
        self.tf_broadcaster = TransformBroadcaster(self)

        # 记录base_link在odom坐标系X轴方向上的当前位置，单位为m
        self.x = 0.0

        # 创建定时器
        # 每0.1 s调用一次timer_callback()，即发布频率约为10 Hz
        self.timer = self.create_timer(
            0.1,
            self.timer_callback
        )

        # 打印启动日志
        self.get_logger().info(
            '开始发布动态变换: odom -> base_link'
        )

    def timer_callback(self):
        # 获取当前节点时间
        now = self.get_clock().now()

        # 创建一条带时间戳的Transform消息
        transform = TransformStamped()

        # 设置Transform对应的时间
        # 动态Transform每次发布时都需要使用当前状态对应的时间戳
        transform.header.stamp = now.to_msg()

        # 设置父Frame和子Frame
        # 表示发布base_link相对于odom的位姿
        transform.header.frame_id = 'odom'
        transform.child_frame_id = 'base_link'

        # 每次定时器触发，X方向位置增加0.01 m
        # 发布周期为0.1 s，因此相当于以0.1 m/s沿X轴正方向运动
        self.x += 0.01

        # 设置平移分量，单位为m
        transform.transform.translation.x = self.x
        transform.transform.translation.y = 0.0
        transform.transform.translation.z = 0.0

        # 设置旋转分量
        # 单位四元数(0, 0, 0, 1)表示没有旋转
        transform.transform.rotation.x = 0.0
        transform.transform.rotation.y = 0.0
        transform.transform.rotation.z = 0.0
        transform.transform.rotation.w = 1.0

        # 发布动态Transform
        # 每次定时器回调都会发布一条新的odom -> base_link Transform
        self.tf_broadcaster.sendTransform(transform)

        # 打印发布日志
        self.get_logger().info(
            f'发布动态变换，此时 x={self.x}'
        )

def main(args=None):
    # 初始化ROS 2
    rclpy.init(args=args)

    # 创建节点
    node = DynamicTFBroadcaster()

    # 进入事件循环，处理定时器等回调
    rclpy.spin(node)

    # 销毁节点
    node.destroy_node()

    # 关闭ROS 2
    rclpy.shutdown()
