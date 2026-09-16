#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/transform_broadcaster.hpp"

using namespace std::chrono_literals;

class DynamicTFBroadcaster : public rclcpp::Node
{
public:
  DynamicTFBroadcaster()
  : Node("dynamic_tf_broadcaster_cpp"),
    x_(0.0)
  {
    // 创建动态Transform广播器，并关联到当前节点
    // TransformBroadcaster发布的Transform会进入/tf话题
    tf_broadcaster_ =
      std::make_shared<tf2_ros::TransformBroadcaster>(*this);

    // 创建定时器
    // 每100 ms调用一次timer_callback()，即发布频率约为10 Hz
    timer_ = this->create_wall_timer(
      100ms,
      std::bind(
        &DynamicTFBroadcaster::timer_callback,
        this));

    // 打印启动日志
    RCLCPP_INFO(
      this->get_logger(),
      "开始发布动态变换: odom -> base_link");
  }

private:
  void timer_callback()
  {
    // 获取当前节点时间
    rclcpp::Time now = this->now();

    // 创建一条带时间戳的Transform消息
    geometry_msgs::msg::TransformStamped transform;

    // 设置Transform对应的时间
    // 动态Transform每次发布时都需要使用当前状态对应的时间戳
    transform.header.stamp = now;

    // 设置父Frame和子Frame
    // 表示发布base_link相对于odom的位姿
    transform.header.frame_id = "odom";
    transform.child_frame_id = "base_link";

    // 每次定时器触发，X方向位置增加0.01 m
    // 发布周期为0.1 s，因此相当于以0.1 m/s沿X轴正方向运动
    x_ += 0.01;

    // 设置平移分量，单位为m
    transform.transform.translation.x = x_;
    transform.transform.translation.y = 0.0;
    transform.transform.translation.z = 0.0;

    // 设置旋转分量
    // 单位四元数(0, 0, 0, 1)表示没有旋转
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = 0.0;
    transform.transform.rotation.w = 1.0;

    // 发布动态Transform
    // 每次定时器回调都会发布一条新的odom -> base_link Transform
    tf_broadcaster_->sendTransform(transform);

    // 打印发布日志
    RCLCPP_INFO(
        this->get_logger(),
        "发布动态变换，此时 x=%.2f",
        x_
    );
  }

  // 动态Transform广播器
  std::shared_ptr<tf2_ros::TransformBroadcaster>
    tf_broadcaster_;

  // 定时器
  rclcpp::TimerBase::SharedPtr timer_;

  // base_link在odom坐标系X轴方向上的当前位置，单位为m
  double x_;
};

int main(int argc, char * argv[])
{
  // 初始化ROS 2
  rclcpp::init(argc, argv);

  // 创建节点
  auto node =
    std::make_shared<DynamicTFBroadcaster>();

  // 进入事件循环，处理定时器等回调
  rclcpp::spin(node);

  // 关闭ROS 2
  rclcpp::shutdown();

  return 0;
}
