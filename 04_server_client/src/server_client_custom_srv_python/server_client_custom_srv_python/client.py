import sys

from custom_srv_interfaces.srv import CustomService

import rclpy
from rclpy.node import Node

# 自定义异步客户端节点
class FirstClientAsync(Node):
    def __init__(self):
        # 初始化节点，节点名为 custom_srv_client_async_python
        super().__init__('custom_srv_client_async_python')

        # 创建客户端：
        # 服务类型：CustomService
        # 服务名称：custom_srv_python_service
        self.client = self.create_client(
            CustomService,
            'custom_srv_python_service'
        )

        # 等待服务端启动
        while not self.client.wait_for_service(timeout_sec=1.0):
            self.get_logger().info(
                '服务不可用，正在等待服务端启动...'
            )

        # 创建请求对象
        self.request = CustomService.Request()

    # 发送异步请求
    def send_request(self, a, b):
        # 设置请求参数
        self.request.a = a
        self.request.b = b

        # 异步发送请求，返回 Future 对象
        return self.client.call_async(self.request)

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 获取命令行参数
    a = int(sys.argv[1])
    b = int(sys.argv[2])

    # 创建客户端节点
    client_node = FirstClientAsync()

    # 异步发送请求
    future = client_node.send_request(a, b)

    # 等待服务端返回响应
    rclpy.spin_until_future_complete(client_node, future)

    # 获取响应结果
    response = future.result()

    client_node.get_logger().info(
        f'收到响应，计算结果：{a} + {b} = {response.sum}\n'
        f'历史计算结果: {response.history_sums}'
    )

    # 等待键盘输入，暂缓执行后续代码，便于调试观察
    input()

    # 销毁节点并关闭 ROS 2
    client_node.destroy_node()
    rclpy.shutdown()
