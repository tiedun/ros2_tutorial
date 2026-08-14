#include <memory> // 智能指针

#include "rclcpp/rclcpp.hpp"
#include "custom_msg_interfaces/msg/custom_message.hpp"

// 自定义订阅者节点，继承 rclcpp::Node
class FirstSubscriber : public rclcpp::Node
{
public:
    FirstSubscriber()
        // 初始化节点，节点名称为 custom_msg_subscriber_cpp
        : Node("custom_msg_subscriber_cpp")
    {
        // 定义订阅回调函数
        // 每接收到一条消息就会被调用一次
        auto topic_callback =
            [this](
                custom_msg_interfaces::msg::CustomMessage::UniquePtr msg
            ) -> void
        {
            // 输出接收到的消息内容
            RCLCPP_INFO(
                this->get_logger(),
                "接收到消息，内容为："
                "log=\"%s\", "
                "timestamp=%d.%09u, "
                "number=%lld",
                msg->log.data.c_str(),
                msg->timestamp.sec,
                msg->timestamp.nanosec,
                static_cast<long long>(msg->number)
            );
        };

        // 创建订阅者：
        // 消息类型：CustomMessage
        // 话题名称：custom_msg_cpp_topic
        // 接收到消息后调用 topic_callback
        // QoS 队列深度：10
        subscription_ =
            this->create_subscription<
                custom_msg_interfaces::msg::CustomMessage
            >(
                "custom_msg_cpp_topic",
                10,
                topic_callback
            );
    }

private:
    // CustomMessage 类型的话题订阅者
    rclcpp::Subscription<
        custom_msg_interfaces::msg::CustomMessage
    >::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建订阅者节点
    auto subscriber_node =
        std::make_shared<FirstSubscriber>();

    // 输出节点启动日志
    RCLCPP_INFO(
        subscriber_node->get_logger(),
        "已创建 custom_msg_subscriber_cpp，准备接收消息..."
    );

    // 保持节点运行，等待并处理接收到的消息
    rclcpp::spin(subscriber_node);

    // 关闭 ROS 2 C++ 客户端库
    rclcpp::shutdown();

    return 0;
}
