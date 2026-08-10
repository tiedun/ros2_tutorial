import rclpy
from rclpy.node import Node

def main(args=None):
    # 1. 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 2. 创建节点
    node = Node('first_node_python')

    # 3. 输出日志
    node.get_logger().info('Python实现的第一个自定义节点~')

    # 4. 保持节点运行，等待并处理 ROS 2 事件
    rclpy.spin(node)

    # 5. 关闭 ROS 2 Python 客户端库
    rclpy.shutdown()

if __name__ == '__main__':
    main()
    