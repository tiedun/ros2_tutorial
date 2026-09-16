#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"
#include "tf2_ros/static_transform_broadcaster.hpp"

class StaticTFBroadcaster : public rclcpp::Node
{
public:
  StaticTFBroadcaster()
  : Node("static_tf_broadcaster_cpp")
  {
    // 创建静态Transform广播器，并关联到当前节点
    tf_static_broadcaster_ =
      std::make_shared<tf2_ros::StaticTransformBroadcaster>(*this);

    // 创建一条带时间戳的Transform消息
    geometry_msgs::msg::TransformStamped transform;

    // 设置消息时间戳
    transform.header.stamp = this->now();

    // 设置父Frame和子Frame
    transform.header.frame_id = "base_link";
    transform.child_frame_id = "base_laser";

    // 设置平移分量，单位为m
    transform.transform.translation.x = 0.1;
    transform.transform.translation.y = 0.0;
    transform.transform.translation.z = 0.2;

    // 设置旋转分量：单位四元数表示没有旋转
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = 0.0;
    transform.transform.rotation.w = 1.0;

    // 发布静态Transform
    tf_static_broadcaster_->sendTransform(transform);

    // 打印日志
    RCLCPP_INFO(
      this->get_logger(),
      "发布静态变换: base_link -> base_laser");
  }

private:
  std::shared_ptr<tf2_ros::StaticTransformBroadcaster>
    tf_static_broadcaster_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<StaticTFBroadcaster>();

  // 保持节点运行
  rclcpp::spin(node);

  rclcpp::shutdown();

  return 0;
}
