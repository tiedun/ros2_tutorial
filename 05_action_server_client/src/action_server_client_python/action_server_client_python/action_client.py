import sys

import rclpy
from rclpy.action import ActionClient
from rclpy.node import Node

from custom_action_interfaces.action import CustomAction

# 自定义动作客户端节点
class FirstActionClient(Node):
    def __init__(self):
        # 初始化节点，节点名为 custom_action_client_python
        super().__init__('custom_action_client_python')

        # 创建动作客户端：
        # 动作类型：CustomAction
        # 动作名称：custom_action_python_action
        self.action_client = ActionClient(
            self,
            CustomAction,
            'custom_action_python_action'
        )

    # 发送动作目标
    def send_goal(self, n):
        # 创建目标对象
        goal = CustomAction.Goal()

        # 设置目标参数
        goal.n = n

        # 等待动作服务端启动
        self.get_logger().info(
            '正在等待动作服务端启动...'
        )
        self.action_client.wait_for_server()

        # 异步发送目标：
        # goal               需要发送的目标对象
        # feedback_callback  接收到动作反馈时调用的回调函数
        self.send_goal_future = self.action_client.send_goal_async(
            goal,
            feedback_callback=self.feedback_callback
        )

        # 目标请求处理完成后，调用 goal_response_callback()
        self.send_goal_future.add_done_callback(
            self.goal_response_callback
        )

    # 目标响应回调函数
    def goal_response_callback(self, future):
        # 获取目标句柄
        goal_handle = future.result()

        # 判断动作服务端是否接受目标
        if not goal_handle.accepted:
            self.get_logger().info(
                '目标被拒绝'
            )
            rclpy.shutdown()
            return

        self.get_logger().info(
            '目标已接受，等待执行结果...'
        )

        # 异步请求最终执行结果
        self.get_result_future = goal_handle.get_result_async()

        # 获取结果后，调用 get_result_callback()
        self.get_result_future.add_done_callback(
            self.get_result_callback
        )

    # 动作反馈回调函数
    def feedback_callback(self, feedback_msg):
        # 获取反馈数据
        feedback = feedback_msg.feedback

        self.get_logger().info(
            f'收到反馈：{feedback.partial_sequence}'
        )

    # 动作结果回调函数
    def get_result_callback(self, future):
        # 获取动作结果
        result = future.result().result

        self.get_logger().info(
            f'收到结果：{result.sequence}'
        )

        # 等待键盘输入，暂缓执行后续代码，便于调试观察
        input()

        # 已经获得最终结果，关闭ROS 2上下文，从而终止spin()循环
        rclpy.shutdown()

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 获取命令行参数
    n = int(sys.argv[1])

    # 创建动作客户端节点
    action_client_node = FirstActionClient()

    # 发送动作目标
    action_client_node.send_goal(n)

    # 保持节点运行，等待并处理目标响应、动作反馈和最终结果
    rclpy.spin(action_client_node)

    # 销毁节点
    action_client_node.destroy_node()
