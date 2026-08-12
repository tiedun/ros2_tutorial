#include <chrono>   // 时间相关功能
#include <memory>   // 智能指针
#include <string>   // 字符串处理

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// 允许直接使用 500ms 等时间字面量
using namespace std::chrono_literals;

// 自定义发布者节点，继承 rclcpp::Node
class MinimalPublisher : public rclcpp::Node
{
public:
    // 构造函数：创建节点并初始化消息计数器
    MinimalPublisher()
        : Node("minimal_publisher"), count_(0)
    {
        // 创建发布者：
        // 消息类型为 std_msgs::msg::String
        // 话题名为 topic
        // QoS 队列深度为 10
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);

        // 定义定时器回调函数
        auto timer_callback =
            [this]() -> void
        {
            // 创建 String 消息对象
            auto message = std_msgs::msg::String();

            // 设置消息内容，并将计数器加 1
            message.data = "Hello, world! " + std::to_string(this->count_++);

            // 输出发布日志
            RCLCPP_INFO(
                this->get_logger(),
                "Publishing: '%s'",
                message.data.c_str()
            );

            // 发布消息
            this->publisher_->publish(message);
        };

        // 创建周期为 500 ms 的定时器，并注册回调函数
        timer_ = this->create_wall_timer(500ms, timer_callback);
    }

private:
    // 定时器对象
    rclcpp::TimerBase::SharedPtr timer_;

    // String 类型的话题发布者
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    // 消息计数器
    size_t count_;
};

int main(int argc, char *argv[])
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建 MinimalPublisher 节点并进入事件循环
    rclcpp::spin(std::make_shared<MinimalPublisher>());

    // 关闭 ROS 2 C++ 客户端库
    rclcpp::shutdown();

    return 0;
}
