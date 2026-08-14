import rclpy
from rclpy.node import Node

from custom_msg_interfaces.msg import CustomMessage

# 定义订阅者节点类
class FirstSubscriber(Node):
    def __init__(self):
        # 创建名为 custom_msg_subscriber_python 的节点
        super().__init__('custom_msg_subscriber_python')

        # 创建订阅者：
        # 消息类型：CustomMessage
        # 话题名称：custom_msg_python_topic
        # 接收到消息后调用 subscriber_callback()
        # QoS 队列深度为 10
        self.subscriber_ = self.create_subscription(
            CustomMessage,
            'custom_msg_python_topic',
            self.subscriber_callback,
            10
        )

    # 订阅回调函数，每接收到一条消息就会被调用一次
    def subscriber_callback(self, msg):
        self.get_logger().info(
            '接收到消息，内容为：'
            f'log="{msg.log.data}", '
            f'timestamp={msg.timestamp.sec}.{msg.timestamp.nanosec:09d}, '
            f'number={msg.number}'
        )

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建订阅者节点
    subscriber_node = FirstSubscriber()

    # 输出节点启动日志
    subscriber_node.get_logger().info(
        '已创建 custom_msg_subscriber_python，准备接收消息...'
    )

    # 保持节点运行，等待并处理接收到的消息
    rclpy.spin(subscriber_node)

    # 销毁节点
    subscriber_node.destroy_node()

    # 关闭 ROS 2 Python 客户端库
    rclpy.shutdown()

if __name__ == '__main__':
    main()
