from custom_srv_interfaces.srv import CustomService

import rclpy
from rclpy.node import Node

# 自定义服务端节点
class FirstServer(Node):
    def __init__(self):
        # 初始化节点，节点名为 custom_srv_server_python
        super().__init__('custom_srv_server_python')

        # 创建服务：
        # 服务类型：CustomService
        # 服务名称：custom_srv_python_service
        # 回调函数：custom_srv_callback
        self.service = self.create_service(
            CustomService,
            'custom_srv_python_service',
            self.custom_srv_callback
        )

        # 保存历史计算结果
        self.history_sums = []

    # 服务回调函数：
    # request  保存客户端发送的请求数据
    # response 用于保存并返回响应数据
    def custom_srv_callback(self, request, response):
        # 计算两个整数之和
        cur_sum = request.a + request.b

        # 保存本次计算结果
        self.history_sums.append(cur_sum)

        # 设置响应数据
        response.sum = cur_sum
        response.history_sums = self.history_sums.copy()

        self.get_logger().info(
            '收到请求：\na: %d b: %d' % (request.a, request.b)
        )

        # 返回响应
        return response

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建服务端节点
    server_node = FirstServer()

    # 保持节点运行，等待并处理客户端请求
    rclpy.spin(server_node)

    # 销毁节点并关闭 ROS 2
    server_node.destroy_node()
    rclpy.shutdown()
