import rclpy
from rclpy.node import Node

from rcl_interfaces.msg import ParameterDescriptor

# 自定义参数节点
class FirstParam(Node):
    def __init__(self):
        # 初始化节点，节点名为 param_node_python
        super().__init__('param_node_python')

        # 创建参数描述信息
        # description：参数的说明信息
        first_parameter_descriptor = ParameterDescriptor(
            description='用于学习的第一个参数~'
        )

        # 声明参数：
        # 参数名称：first_parameter
        # 默认值：world
        # 参数描述：first_parameter_descriptor
        self.declare_parameter(
            'first_parameter',
            'world',
            first_parameter_descriptor
        )

        # 创建定时器：
        # 每 1 秒调用一次 timer_callback()
        self.timer = self.create_timer(
            1,
            self.timer_callback
        )

    # 定时器回调函数
    def timer_callback(self):
        # 获取 first_parameter 参数
        # get_parameter() 获取参数对象
        # get_parameter_value() 获取参数值对象
        # string_value 获取其中保存的字符串值
        first_parameter = (
            self.get_parameter('first_parameter')
            .get_parameter_value()
            .string_value
        )

        # 输出当前参数值
        self.get_logger().info(
            'Hello %s!' % first_parameter
        )

        # 创建 Parameter 对象：
        # 参数名称：first_parameter
        # 参数类型：STRING
        # 参数值：world
        default_param = rclpy.parameter.Parameter(
            'first_parameter',
            rclpy.Parameter.Type.STRING,
            'world'
        )

        # set_parameters() 接收参数对象列表
        all_parameters = [default_param]

        # 将 first_parameter 参数重新设置为 world
        self.set_parameters(all_parameters)


def main():
    # 初始化 ROS 2 Python 客户端库
    rclpy.init()

    # 创建自定义参数节点
    node = FirstParam()

    # 进入事件循环，使节点持续运行并处理定时器等回调
    rclpy.spin(node)
    