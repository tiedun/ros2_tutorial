#include "rclcpp/rclcpp.hpp"

int main(int argc, char * argv[])
{
    // 1. 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 2. 创建节点
    auto node = std::make_shared<rclcpp::Node>("first_node_cpp");

    // 3. 输出日志
    RCLCPP_INFO(node->get_logger(), "C++实现的第一个自定义节点~");

    // 4. 保持节点运行，等待并处理 ROS 2 事件
    rclcpp::spin(node);

    // 5. 关闭 ROS 2 C++ 客户端库
    rclcpp::shutdown();

    return 0;
}
