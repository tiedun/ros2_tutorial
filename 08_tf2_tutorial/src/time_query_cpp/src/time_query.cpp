#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.hpp"
#include "tf2/time.hpp"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals;

class TimeQuery : public rclcpp::Node
{
public:
  TimeQuery()
  : Node("time_query_cpp")
  {
    // 创建Buffer，用于缓存和查询Transform
    tf_buffer_ = std::make_shared<tf2_ros::Buffer>(
      this->get_clock()
    );

    // 创建TransformListener，将接收到的Transform写入Buffer
    tf_listener_ =
      std::make_shared<tf2_ros::TransformListener>(
        *tf_buffer_
      );

    // 创建定时器，按照1 Hz的频率查询Transform
    timer_ = this->create_wall_timer(
      1s,
      std::bind(&TimeQuery::query_timer, this)
    );
  }

private:
  // 查询指定时间的Transform
  void query_transform(
    const rclcpp::Time & time,
    const std::string & desc)
  {
    try {
      // 查询指定时间base_link到odom的Transform
      auto transform = tf_buffer_->lookupTransform(
        "odom",
        "base_link",
        time
      );

      // 输出Transform的X轴平移量
      RCLCPP_INFO(
        this->get_logger(),
        "%sTransform：x=%.2f",
        desc.c_str(),
        transform.transform.translation.x
      );
    }
    catch (const tf2::TransformException & ex) {
      // Transform不存在或无法计算时输出警告
      RCLCPP_WARN(
        this->get_logger(),
        "查询%sTransform失败：%s",
        desc.c_str(),
        ex.what()
      );
    }
  }

  // 定时执行不同时间的Transform查询
  void query_timer()
  {
    RCLCPP_INFO(
      this->get_logger(),
      "--------------------"
    );

    // 时间为0表示查询Buffer中最新可用的Transform
    query_transform(
      rclcpp::Time(0, 0, this->get_clock()->get_clock_type()),
      "最新"
    );

    // 查询当前节点时间3秒前的Transform
    query_transform(
      this->get_clock()->now() -
      rclcpp::Duration::from_seconds(3.0),
      "3秒前"
    );

    // 查询当前节点时间3秒后的Transform
    query_transform(
      this->get_clock()->now() +
      rclcpp::Duration::from_seconds(3.0),
      "3秒后"
    );

    // 查询当前节点时间20秒前的Transform
    query_transform(
      this->get_clock()->now() -
      rclcpp::Duration::from_seconds(20.0),
      "20秒前"
    );

    RCLCPP_INFO(
      this->get_logger(),
      "--------------------"
    );
  }

  // Buffer：缓存Transform并提供查询接口
  std::shared_ptr<tf2_ros::Buffer> tf_buffer_;

  // TransformListener：接收Transform并写入Buffer
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

  // 定时器
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  // 初始化ROS 2
  rclcpp::init(argc, argv);

  // 创建节点
  auto node = std::make_shared<TimeQuery>();

  // 运行节点
  rclcpp::spin(node);

  // 关闭ROS 2
  rclcpp::shutdown();

  return 0;
}
