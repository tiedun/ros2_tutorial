#include <memory> // 智能指针

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

// 自定义订阅者节点，继承 rclcpp::Node
class FirstSubscriber : public rclcpp::Node
{
public:
    FirstSubscriber()
        : Node("subscriber_cpp")
    {
        // 定义话题消息回调函数
        // 当订阅者接收到消息时，该函数会被调用
        auto topic_callback =
            [this](std_msgs::msg::String::UniquePtr msg) -> void
        {
            // 输出接收到的消息内容
            RCLCPP_INFO(
                this->get_logger(),
                "I heard: '%s'",
                msg->data.c_str());
        };

        // 创建订阅者：
        // 消息类型为 std_msgs::msg::String
        // 话题名为 cpp_topic
        // QoS 队列深度为 10
        // 接收到消息后调用 topic_callback
        subscription_ =
            this->create_subscription<std_msgs::msg::String>(
                "cpp_topic",
                10,
                topic_callback);
    }

private:
    // String 类型的话题订阅者
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
};

int main(int argc, char *argv[])
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建 FirstSubscriber 节点并进入事件循环，
    // 等待并处理订阅消息
    rclcpp::spin(std::make_shared<FirstSubscriber>());

    // 关闭 ROS 2 C++ 客户端库
    rclcpp::shutdown();

    return 0;
}
