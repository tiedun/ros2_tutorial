#include <chrono>
#include <cmath>
#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/pose_stamped.hpp"

#include "tf2/exceptions.hpp"
#include "tf2_ros/buffer.hpp"
#include "tf2_ros/transform_listener.hpp"

// 提供PoseStamped等geometry_msgs消息的tf2转换支持
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

using namespace std::chrono_literals;

class PoseTransform : public rclcpp::Node
{
public:
  PoseTransform()
  : Node("pose_transform_cpp")
  {
    // 创建Buffer，用于保存和查询Transform
    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(
      this->get_clock()
    );

    // 创建TransformListener
    // 接收/tf和/tf_static中的Transform，并写入Buffer
    // false表示不创建独立的spin线程，由当前节点的Executor处理回调
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(
      *tf_buffer_,
      this,
      false
    );

    // 创建定时器，每1秒尝试执行一次位姿转换
    // 转换成功后取消定时器
    timer_ = this->create_wall_timer(
      1s,
      [this]() {
        this->transform_pose();
      }
    );
  }

private:
  void transform_pose()
  {
    // 创建camera_link中的目标位姿
    geometry_msgs::msg::PoseStamped pose;

    // 设置数据所属的Frame
    pose.header.frame_id = "camera_link";

    // 设置数据对应的时间
    pose.header.stamp = this->get_clock()->now();

    // 设置目标位置：[1.0, 0.0, 0.0]
    pose.pose.position.x = 1.0;
    pose.pose.position.y = 0.0;
    pose.pose.position.z = 0.0;

    // 设置目标姿态：绕Z轴旋转90°
    constexpr double pi = 3.14159265358979323846;

    pose.pose.orientation.x = 0.0;
    pose.pose.orientation.y = 0.0;
    pose.pose.orientation.z = std::sin(pi / 4.0);
    pose.pose.orientation.w = std::cos(pi / 4.0);

    try {
      // 根据pose中的Frame和时间查询Transform
      // 并将目标位姿从camera_link转换到base_link
      auto result = tf_buffer_->transform(
        pose,
        "base_link"
      );

      // 输出目标在camera_link中的位置
      RCLCPP_INFO(
        this->get_logger(),
        "目标在 camera_link 中的 position: [%.2f, %.2f, %.2f]",
        pose.pose.position.x,
        pose.pose.position.y,
        pose.pose.position.z
      );

      // 输出目标在camera_link中的姿态
      RCLCPP_INFO(
        this->get_logger(),
        "目标在 camera_link 中的 orientation: [%.3f, %.3f, %.3f, %.3f]",
        pose.pose.orientation.x,
        pose.pose.orientation.y,
        pose.pose.orientation.z,
        pose.pose.orientation.w
      );

      // 输出目标在base_link中的位置
      RCLCPP_INFO(
        this->get_logger(),
        "目标在 base_link 中的 position: [%.2f, %.2f, %.2f]",
        result.pose.position.x,
        result.pose.position.y,
        result.pose.position.z
      );

      // 输出目标在base_link中的姿态
      RCLCPP_INFO(
        this->get_logger(),
        "目标在 base_link 中的 orientation: [%.3f, %.3f, %.3f, %.3f]",
        result.pose.orientation.x,
        result.pose.orientation.y,
        result.pose.orientation.z,
        result.pose.orientation.w
      );

      // 转换成功后取消定时器
      timer_->cancel();
    } catch (const tf2::TransformException & ex) {
      // Buffer中暂时没有可用于转换的Transform时输出警告
      // 定时器下一次触发时会继续尝试
      RCLCPP_WARN(
        this->get_logger(),
        "位姿转换失败：%s",
        ex.what()
      );
    }
  }

  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  // 初始化ROS 2
  rclcpp::init(argc, argv);

  // 创建节点
  auto node = std::make_shared<PoseTransform>();

  // 进入事件循环，处理订阅和定时器等回调
  rclcpp::spin(node);

  // 关闭ROS 2
  rclcpp::shutdown();

  return 0;
}
