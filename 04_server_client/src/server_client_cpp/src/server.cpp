#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

#include <functional>
#include <memory>

// 自定义服务端节点
class FirstServer : public rclcpp::Node
{
public:
    FirstServer()
        // 初始化节点，节点名为 server_cpp
        : Node("server_cpp")
    {
        // 创建服务：
        // 服务类型：AddTwoInts
        // 服务名称：cpp_service
        // 回调函数：add_two_ints_callback
        service_ = this->create_service<example_interfaces::srv::AddTwoInts>(
            "cpp_service",
            std::bind(
                &FirstServer::add_two_ints_callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            )
        );
    }

private:
    // 服务回调函数：
    // request  保存客户端发送的请求数据
    // response 用于保存并返回响应数据
    void add_two_ints_callback(
        const std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request,
        std::shared_ptr<example_interfaces::srv::AddTwoInts::Response> response)
    {
        // 计算两个整数之和
        response->sum = request->a + request->b;

        RCLCPP_INFO(
            this->get_logger(),
            "收到请求：\na: %ld b: %ld",
            request->a,
            request->b
        );
    }

    // 服务对象
    rclcpp::Service<example_interfaces::srv::AddTwoInts>::SharedPtr service_;
};

int main(int argc, char **argv)
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建服务端节点
    auto server_node = std::make_shared<FirstServer>();

    // 保持节点运行，等待并处理客户端请求
    rclcpp::spin(server_node);

    // 销毁节点并关闭 ROS 2
    server_node.reset();
    rclcpp::shutdown();

    return 0;
}
