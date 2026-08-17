import time

import rclpy
from rclpy.action import ActionServer
from rclpy.node import Node

from custom_action_interfaces.action import CustomAction

# 自定义动作服务端节点
class FirstActionServer(Node):
    def __init__(self):
        # 初始化节点，节点名为 custom_action_server_python
        super().__init__('custom_action_server_python')

        # 创建动作服务端：
        # 动作类型：CustomAction
        # 动作名称：custom_action_python_action
        # 执行回调函数：execute_callback
        self.action_server = ActionServer(
            self,
            CustomAction,
            'custom_action_python_action',
            self.execute_callback
        )

    # 动作执行回调函数：
    # goal_handle 保存客户端发送的目标以及当前目标的执行状态
    def execute_callback(self, goal_handle):
        self.get_logger().info(
            f'收到目标，开始执行：n = {goal_handle.request.n}'
        )

        # 创建反馈对象
        feedback = CustomAction.Feedback()

        # 设置 Fibonacci 数列的初始值
        feedback.partial_sequence = [0, 1]

        # 根据客户端发送的目标计算 Fibonacci 数列
        for _ in range(1, goal_handle.request.n):
            # 计算下一个数
            next_number = (
                feedback.partial_sequence[-1]
                + feedback.partial_sequence[-2]
            )

            # 保存计算结果
            feedback.partial_sequence.append(next_number)

            # 发布动作反馈
            goal_handle.publish_feedback(feedback)

            self.get_logger().info(
                f'发送反馈：{feedback.partial_sequence}'
            )

            # 模拟动作执行过程
            time.sleep(0.1)

        # 将当前目标设置为执行成功状态
        goal_handle.succeed()

        # 创建结果对象
        result = CustomAction.Result()

        # 设置最终结果
        result.sequence = feedback.partial_sequence

        self.get_logger().info(
            f'目标执行完成，最终结果：{result.sequence}'
        )

        # 返回动作结果
        return result

def main(args=None):
    # 初始化 ROS 2 Python 客户端库
    rclpy.init(args=args)

    # 创建动作服务端节点
    action_server_node = FirstActionServer()

    # 保持节点运行，等待并处理客户端发送的目标
    rclpy.spin(action_server_node)

    # 销毁节点并关闭 ROS 2
    action_server_node.destroy_node()
    rclpy.shutdown()
