#include <chrono>   // 时间相关功能
#include <cstdint>  // std::int64_t
#include <memory>   // 智能指针

#include "rclcpp/rclcpp.hpp"
#include "custom_msg_interfaces/msg/custom_message.hpp"

// 允许直接使用 500ms 等时间字面量
using namespace std::chrono_literals;

// 自定义发布者节点，继承 rclcpp::Node
class FirstPublisher : public rclcpp::Node
{
public:
    FirstPublisher()
        // 初始化节点，节点名称为 custom_msg_publisher_cpp
        : Node("custom_msg_publisher_cpp"), count_(0)
    {
        // 创建发布者：
        // 消息类型：CustomMessage
        // 话题名称：custom_msg_cpp_topic
        // QoS 队列深度：10
        publisher_ =
            this->create_publisher<custom_msg_interfaces::msg::CustomMessage>(
                "custom_msg_cpp_topic",
                10
            );

        // 定义定时器回调函数
        auto timer_callback =
            [this]() -> void
        {
            // 创建自定义消息对象
            auto message =
                custom_msg_interfaces::msg::CustomMessage();

            // 设置 log 字段
            // log 的类型为 std_msgs/msg/String，
            // 因此需要通过其 data 字段保存字符串
            message.log.data = "Hello world!";

            // 设置 timestamp 字段
            // 获取节点当前时间，并转换为 builtin_interfaces/msg/Time 消息
            message.timestamp = this->get_clock()->now();

            // 设置 number 字段
            message.number = this->count_;

            // 消息计数器递增
            this->count_++;

            // 发布消息
            this->publisher_->publish(message);

            // 输出发布日志
            RCLCPP_INFO(
                this->get_logger(),
                "发布消息："
                "log=\"%s\", "
                "timestamp=%d.%09u, "
                "number=%lld",
                message.log.data.c_str(),
                message.timestamp.sec,
                message.timestamp.nanosec,
                static_cast<long long>(message.number)
            );
        };

        // 创建定时器：
        // 每 500 ms 调用一次 timer_callback
        timer_ =
            this->create_wall_timer(
                500ms,
                timer_callback
            );
    }

private:
    // 定时器对象
    rclcpp::TimerBase::SharedPtr timer_;

    // CustomMessage 类型的话题发布者
    rclcpp::Publisher<
        custom_msg_interfaces::msg::CustomMessage
    >::SharedPtr publisher_;

    // 消息计数器
    std::int64_t count_;
};

int main(int argc, char *argv[])
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建发布者节点
    auto publisher_node =
        std::make_shared<FirstPublisher>();

    // 输出节点启动日志
    RCLCPP_INFO(
        publisher_node->get_logger(),
        "创建 custom_msg_publisher_cpp，"
        "即将向 custom_msg_cpp_topic 发布 CustomMessage 消息..."
    );

    // 保持节点运行，处理定时器等 ROS 2 事件
    rclcpp::spin(publisher_node);

    // 关闭 ROS 2 C++ 客户端库
    rclcpp::shutdown();

    return 0;
}
