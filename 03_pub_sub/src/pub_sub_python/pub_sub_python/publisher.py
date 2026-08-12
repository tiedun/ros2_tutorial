import rclpy
from rclpy.node import Node

from std_msgs.msg import String

# 定义发布者节点类
class FirstPublisher(Node):
    def __init__(self):
        # 初始化节点，节点名称为 publisher_python
        super().__init__('publisher_python')

        # 创建发布者
        # 消息类型为 String，发布到 python_topic 话题
        # 队列深度为 10
        self.publisher_ = self.create_publisher(String, 'python_topic', 10)

        # 每 0.5 秒调用一次发布回调函数
        period = 0.5
        self.timer_ = self.create_timer(period, self.publisher_callback)

        # 消息计数器
        self.i = 0

    def publisher_callback(self):
        # 创建并填充消息
        msg = String()
        msg.data = f'Hello world {self.i}'
        self.i += 1

        # 发布消息
        self.publisher_.publish(msg)

        # 输出发布日志
        self.get_logger().info(msg.data)

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建发布者节点
    publisher_node = FirstPublisher()
    publisher_node.get_logger().info(
        '创建 publisher_python，即将开始发布消息...'
    )

    # 保持节点运行，处理定时器等 ROS 2 事件
    rclpy.spin(publisher_node)

    # 关闭 ROS 2 Python 客户端库
    rclpy.shutdown()

if __name__ == '__main__':
    main()
