#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std::chrono_literals;

// 自定义异步客户端节点
class FirstClientAsync : public rclcpp::Node
{
public:
    FirstClientAsync()
        // 初始化节点，节点名为 client_async_cpp
        : Node("client_async_cpp")
    {
        // 创建客户端：
        // 服务类型：AddTwoInts
        // 服务名称：cpp_service
        client_ = this->create_client<example_interfaces::srv::AddTwoInts>(
            "cpp_service"
        );

        // 等待服务端启动
        while (!client_->wait_for_service(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(
                    this->get_logger(),
                    "等待服务时 ROS 2 已关闭。"
                );
                return;
            }

            RCLCPP_INFO(
                this->get_logger(),
                "服务不可用，正在等待服务端启动..."
            );
        }

        // 创建请求对象
        request_ =
            std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
    }

    // 发送异步请求
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture
    send_request(int64_t a, int64_t b)
    {
        // 设置请求参数
        request_->a = a;
        request_->b = b;

        // 异步发送请求
        auto future_and_request_id = client_->async_send_request(request_);
        
        // 获取 SharedFuture 对象并返回
        return future_and_request_id.future.share();
    }

private:
    // 客户端对象
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;

    // 请求对象
    std::shared_ptr<example_interfaces::srv::AddTwoInts::Request> request_;
};

int main(int argc, char **argv)
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 检查命令行参数
    if (argc != 3)
    {
        RCLCPP_ERROR(
            rclcpp::get_logger("client_async_cpp"),
            "用法：client_async_cpp X Y"
        );

        rclcpp::shutdown();
        return 1;
    }

    // 获取命令行参数
    int64_t a = std::atoll(argv[1]);
    int64_t b = std::atoll(argv[2]);

    // 创建客户端节点
    auto client_node = std::make_shared<FirstClientAsync>();

    // 异步发送请求
    auto future = client_node->send_request(a, b);

    // 等待服务端返回响应
    auto result = rclcpp::spin_until_future_complete(
        client_node,
        future
    );

    // 判断是否成功收到响应
    if (result != rclcpp::FutureReturnCode::SUCCESS)
    {
        RCLCPP_ERROR(
            client_node->get_logger(),
            "等待服务端响应失败。"
        );

        // 销毁节点并关闭 ROS 2
        client_node.reset();
        rclcpp::shutdown();

        // 发生错误，返回非零值
        return 1;
    }

    // 获取响应结果
    auto response = future.get();

    RCLCPP_INFO(
        client_node->get_logger(),
        "收到响应，计算结果：%ld + %ld = %ld",
        a,
        b,
        response->sum
    );

    // 等待键盘输入，暂缓执行后续代码，便于调试观察
    std::cin.get();

    // 销毁节点并关闭 ROS 2
    client_node.reset();
    rclcpp::shutdown();

    return 0;
}
