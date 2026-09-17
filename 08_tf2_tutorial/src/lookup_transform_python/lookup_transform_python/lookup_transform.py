import rclpy
from rclpy.node import Node
from rclpy.time import Time

from tf2_ros import TransformException
from tf2_ros.buffer import Buffer
from tf2_ros.transform_listener import TransformListener

class LookupTransform(Node):
    def __init__(self):
        # 初始化节点，节点名为lookup_transform_python
        super().__init__('lookup_transform_python')

        # 创建Buffer
        self.tf_buffer = Buffer(node=self)

        # 创建TransformListener
        self.tf_listener = TransformListener(
            self.tf_buffer,
            self,
            spin_thread=False
        )

        # 创建定时器
        # 每 1s 查询一次Transform，即查询频率约为1 Hz
        self.timer = self.create_timer(
            1.0,
            self.timer_callback
        )

        # 打印启动日志
        self.get_logger().info(
            '开始查询Transform: odom -> base_link'
        )

    def _lookup_latest_transform(self, target, source):
        try:
            # 查询最新可用的 Transform
            # target_frame 为 target
            # source_frame 为 source
            transform = self.tf_buffer.lookup_transform(
                target,
                source,
                Time()
            )

        except TransformException as ex:
            # 当前Buffer中不存在可用Transform时打印警告
            self.get_logger().warn(
                f'暂时无法查询 {source} 转换到 {target} 的 Transform: {ex}'
            )
            return

        # 获取平移分量
        translation = transform.transform.translation

        # 获取旋转分量
        rotation = transform.transform.rotation

        # 打印查询结果
        self.get_logger().info(
            f'查询到 {source} 转换到 {target} Transform: '
            f'translation=[{translation.x:.3f}, '
            f'{translation.y:.3f}, '
            f'{translation.z:.3f}], '
            f'rotation=[{rotation.x:.3f}, '
            f'{rotation.y:.3f}, '
            f'{rotation.z:.3f}, '
            f'{rotation.w:.3f}]'
        )
    
    def timer_callback(self):
        self.get_logger().info("=" * 20)
        # 查询 base_link(source) 转换到 odom(target) 的 Transform
        self._lookup_latest_transform('odom', 'base_link')

        # 查询 odom 转换到 base_link 的 Transform
        # Buffer 帮我们完成逆变换
        self._lookup_latest_transform('base_link', 'odom')

        # 查询 base_link 转换到 map 的 Transform
        # Buffer 帮我们完成复合变换
        self._lookup_latest_transform('map', 'base_link')      
        self.get_logger().info("=" * 20)

def main(args=None):
    # 初始化ROS 2
    rclpy.init(args=args)

    # 创建节点
    node = LookupTransform()

    # 进入事件循环，处理定时器和TransformListener等回调
    rclpy.spin(node)

    # 销毁节点
    node.destroy_node()

    # 关闭ROS 2
    rclpy.shutdown()
