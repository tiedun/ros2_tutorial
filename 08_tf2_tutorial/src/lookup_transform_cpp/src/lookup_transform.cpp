#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"

#include "geometry_msgs/msg/transform_stamped.hpp"

#include "tf2/exceptions.hpp"
#include "tf2/time.hpp"

#include "tf2_ros/buffer.hpp"
#include "tf2_ros/transform_listener.hpp"

using namespace std::chrono_literals;

class LookupTransform : public rclcpp::Node
{
public:
  LookupTransform()
  : Node("lookup_transform_cpp")
  {
    // 创建Buffer
    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(
      this->get_clock()
    );

    // 创建TransformListener
    tf_listener_ =
      std::make_shared<tf2_ros::TransformListener>(
        *tf_buffer_,
        this,
        false
      );

    // 创建定时器
    // 每1s查询一次Transform，即查询频率约为1 Hz
    timer_ = this->create_timer(
      1s,
      std::bind(&LookupTransform::timer_callback, this)
    );

    // 打印启动日志
    RCLCPP_INFO(
      this->get_logger(),
      "开始查询Transform"
    );
  }

private:
  void lookup_latest_transform(
    const std::string & target,
    const std::string & source)
  {
    try {
      // 查询最新可用的Transform
      // target_frame为target
      // source_frame为source
      auto transform = tf_buffer_->lookupTransform(
        target,
        source,
        tf2::TimePointZero
      );

      // 获取平移分量
      const auto & translation =
        transform.transform.translation;

      // 获取旋转分量
      const auto & rotation =
        transform.transform.rotation;

      // 打印查询结果
      RCLCPP_INFO(
        this->get_logger(),
        "查询到 %s 转换到 %s Transform: "
        "translation=[%.3f, %.3f, %.3f], "
        "rotation=[%.3f, %.3f, %.3f, %.3f]",
        source.c_str(),
        target.c_str(),
        translation.x,
        translation.y,
        translation.z,
        rotation.x,
        rotation.y,
        rotation.z,
        rotation.w
      );
    }
    catch (const tf2::TransformException & ex) {
      // 当前Buffer中不存在可用Transform时打印警告
      RCLCPP_WARN(
        this->get_logger(),
        "暂时无法查询 %s 转换到 %s 的 Transform: %s",
        source.c_str(),
        target.c_str(),
        ex.what()
      );
    }
  }

  void timer_callback()
  {
    RCLCPP_INFO(
      this->get_logger(),
      "===================="
    );

    // 查询base_link(source)转换到odom(target)的Transform
    lookup_latest_transform(
      "odom",
      "base_link"
    );

    // 查询odom转换到base_link的Transform
    // Buffer帮我们完成逆变换
    lookup_latest_transform(
      "base_link",
      "odom"
    );

    // 查询base_link转换到map的Transform
    // Buffer帮我们完成复合变换
    lookup_latest_transform(
      "map",
      "base_link"
    );

    RCLCPP_INFO(
      this->get_logger(),
      "===================="
    );
  }

  // Buffer
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;

  // TransformListener
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  // 定时器
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  // 初始化ROS 2
  rclcpp::init(argc, argv);

  // 创建节点并进入事件循环
  rclcpp::spin(
    std::make_shared<LookupTransform>()
  );

  // 关闭ROS 2
  rclcpp::shutdown();

  return 0;
}
