from example_interfaces.srv import AddTwoInts

import rclpy
from rclpy.node import Node

# 自定义服务端节点
class FirstService(Node):
    def __init__(self):
        # 初始化节点，节点名为 service_python
        super().__init__('service_python')

        # 创建服务：
        # 服务类型：AddTwoInts
        # 服务名称：first_service
        # 回调函数：add_two_ints_callback
        self.service = self.create_service(
            AddTwoInts,
            'first_service',
            self.add_two_ints_callback
        )

    # 服务回调函数：
    # request  保存客户端发送的请求数据
    # response 用于保存并返回响应数据
    def add_two_ints_callback(self, request, response):
        # 计算两个整数之和
        response.sum = request.a + request.b

        self.get_logger().info(
            '收到请求：\na: %d b: %d' % (request.a, request.b)
        )

        # 返回响应
        return response

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建服务端节点
    service_node = FirstService()

    # 保持节点运行，等待并处理客户端请求
    rclpy.spin(service_node)

    # 销毁节点并关闭 ROS 2
    service_node.destroy_node()
    rclpy.shutdown()
