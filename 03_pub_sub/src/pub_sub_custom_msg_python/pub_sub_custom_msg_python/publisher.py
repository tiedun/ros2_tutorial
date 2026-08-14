import rclpy
from rclpy.node import Node

from custom_msg_interfaces.msg import CustomMessage

# 自定义发布者节点
class FirstPublisher(Node):
    def __init__(self):
        # 初始化节点，节点名称为 custom_msg_publisher_python
        super().__init__('custom_msg_publisher_python')

        # 创建发布者：
        # 消息类型：CustomMessage
        # 话题名称：custom_msg_python_topic
        # QoS 队列深度：10
        self.publisher_ = self.create_publisher(
            CustomMessage,
            'custom_msg_python_topic',
            10
        )

        # 创建定时器：
        # 每 0.5 秒调用一次 publisher_callback()
        period = 0.5
        self.timer_ = self.create_timer(
            period,
            self.publisher_callback
        )

        # 消息计数器
        self.i = 0

    # 定时器回调函数
    def publisher_callback(self):
        # 创建自定义消息对象
        msg = CustomMessage()

        # 设置 log 字段
        # log 的类型为 std_msgs/msg/String，
        # 因此需要通过其 data 字段保存字符串
        msg.log.data = 'Hello world!'

        # 设置 timestamp 字段
        # 获取节点当前时间，并转换为 builtin_interfaces/msg/Time 消息
        msg.timestamp = self.get_clock().now().to_msg()

        # 设置 number 字段
        msg.number = self.i

        # 消息计数器递增
        self.i += 1

        # 发布消息
        self.publisher_.publish(msg)

        # 输出发布日志
        self.get_logger().info(
            '发布消息：'
            f'log="{msg.log.data}", '
            f'timestamp={msg.timestamp.sec}.{msg.timestamp.nanosec:09d}, '
            f'number={msg.number}'
        )

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建发布者节点
    publisher_node = FirstPublisher()

    publisher_node.get_logger().info(
        '创建 custom_msg_publisher_python，'
        '即将向 custom_msg_python_topic 发布 CustomMessage 消息...'
    )

    # 保持节点运行，处理定时器等 ROS 2 事件
    rclpy.spin(publisher_node)

    # 销毁节点
    publisher_node.destroy_node()

    # 关闭 ROS 2 Python 客户端库
    rclpy.shutdown()

if __name__ == '__main__':
    main()
