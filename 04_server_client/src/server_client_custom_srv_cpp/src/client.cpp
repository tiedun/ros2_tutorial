#include "rclcpp/rclcpp.hpp"
#include "custom_srv_interfaces/srv/custom_service.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>

using namespace std::chrono_literals;

// 自定义异步客户端节点
class FirstClientAsync : public rclcpp::Node
{
public:
    FirstClientAsync()
        // 初始化节点，节点名为 custom_srv_client_async_cpp
        : Node("custom_srv_client_async_cpp")
    {
        // 创建客户端：
        // 服务类型：CustomService
        // 服务名称：custom_srv_cpp_service
        client_ =
            this->create_client<custom_srv_interfaces::srv::CustomService>(
                "custom_srv_cpp_service"
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
        request_ = std::make_shared<
            custom_srv_interfaces::srv::CustomService::Request>();
    }

    // 发送异步请求
    rclcpp::Client<
        custom_srv_interfaces::srv::CustomService>::SharedFuture
    send_request(int32_t a, int32_t b)
    {
        // 设置请求参数
        request_->a = a;
        request_->b = b;

        // 异步发送请求
        auto future_and_request_id =
            client_->async_send_request(request_);

        // 获取 SharedFuture 对象并返回
        return future_and_request_id.future.share();
    }

private:
    // 客户端对象
    rclcpp::Client<
        custom_srv_interfaces::srv::CustomService>::SharedPtr client_;

    // 请求对象
    std::shared_ptr<
        custom_srv_interfaces::srv::CustomService::Request> request_;
};

int main(int argc, char **argv)
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 检查命令行参数
    if (argc != 3)
    {
        RCLCPP_ERROR(
            rclcpp::get_logger("custom_srv_client_async_cpp"),
            "用法：custom_srv_client_async_cpp X Y"
        );

        rclcpp::shutdown();
        return 1;
    }

    // 获取命令行参数
    int32_t a = std::atoi(argv[1]);
    int32_t b = std::atoi(argv[2]);

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

    // 将历史计算结果转换为字符串
    std::ostringstream history_sums_stream;
    history_sums_stream << "[";

    for (std::size_t i = 0; i < response->history_sums.size(); ++i)
    {
        history_sums_stream << response->history_sums[i];

        if (i < response->history_sums.size() - 1)
        {
            history_sums_stream << ", ";
        }
    }

    history_sums_stream << "]";

    // 输出响应数据
    RCLCPP_INFO(
        client_node->get_logger(),
        "收到响应，计算结果：%d + %d = %d\n"
        "历史计算结果: %s",
        a,
        b,
        response->sum,
        history_sums_stream.str().c_str()
    );

    // 等待键盘输入，暂缓执行后续代码，便于调试观察
    std::cin.get();

    // 销毁节点并关闭 ROS 2
    client_node.reset();
    rclcpp::shutdown();

    return 0;
}
