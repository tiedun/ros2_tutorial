import rclpy
from rclpy.duration import Duration
from rclpy.node import Node
from rclpy.time import Time

from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener

class TimeQuery(Node):
    def __init__(self):
        # 初始化节点
        super().__init__('time_query_python')

        # 创建Buffer
        self.tf_buffer = Buffer(node=self)

        # 创建TransformListener
        self.tf_listener = TransformListener(
            self.tf_buffer,
            self,
            spin_thread=False
        )

        # 创建定时器，按照1 Hz的频率查询Transform
        self.timer = self.create_timer(
            1.0,
            self.query_timer
        )

    def query_transform(self, time_obj, desc):
        """查询指定时间的Transform"""

        try:
            # 查询base_link在odom中的Transform
            transform = self.tf_buffer.lookup_transform(
                'odom',
                'base_link',
                time_obj
            )

            # 输出X方向的平移量
            self.get_logger().info(
                f'{desc}Transform：'
                f'x={transform.transform.translation.x:.2f}'
            )

        except TransformException as ex:
            # Transform查询失败时输出警告
            self.get_logger().warn(
                f'查询{desc}Transform失败：{ex}'
            )

    def query_timer(self):
        """定时执行不同时间的Transform查询"""

        self.get_logger().info('-' * 20)

        # 查询Buffer当前能够提供的最新Transform
        self.query_transform(
            Time(),
            '最新'
        )

        # 获取当前时间
        current_time = self.get_clock().now()

        # 查询当前时间3秒前的Transform
        self.query_transform(
            current_time - Duration(seconds=3.0),
            '3秒前'
        )

        # 查询当前时间3秒后的Transform
        self.query_transform(
            current_time + Duration(seconds=3.0),
            '3秒后'
        )

        # 查询当前时间20秒前的Transform
        self.query_transform(
            current_time - Duration(seconds=20.0),
            '20秒前'
        )

        self.get_logger().info('-' * 20)

def main(args=None):
    # 初始化ROS 2
    rclpy.init(args=args)

    # 创建节点
    node = TimeQuery()

    # 运行节点
    rclpy.spin(node)

    # 销毁节点并关闭ROS 2
    node.destroy_node()
    rclpy.shutdown()
