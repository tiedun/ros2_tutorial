import rclpy
from rclpy.node import Node

from std_msgs.msg import String

# 定义订阅者节点类
class FirstSubscriber(Node):
    def __init__(self):
        # 创建名为 subscriber_python 的节点
        super().__init__('subscriber_python')

        # 创建订阅者：
        # 消息类型为 String，订阅 python_topic 话题
        # 接收到消息后调用 subscriber_callback()
        # 队列深度为 10
        self.subscriber = self.create_subscription(
            String,
            'python_topic',
            self.subscriber_callback,
            10
        )

    # 订阅回调函数，每接收到一条消息就会被调用一次
    def subscriber_callback(self, msg):
        self.get_logger().info(f'接收到消息，内容为：{msg.data}')

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建订阅者节点
    subscriber_node = FirstSubscriber()

    # 输出节点启动日志
    subscriber_node.get_logger().info(
        '已创建 subscriber_python，准备接收消息...'
    )

    # 保持节点运行，等待并处理接收到的消息
    rclpy.spin(subscriber_node)

    # 关闭 ROS 2 Python 客户端库
    rclpy.shutdown()

if __name__ == '__main__':
    main()
