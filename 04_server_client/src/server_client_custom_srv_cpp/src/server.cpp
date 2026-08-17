#include "rclcpp/rclcpp.hpp"
#include "custom_srv_interfaces/srv/custom_service.hpp"

#include <functional>
#include <memory>
#include <vector>

// 自定义服务端节点
class FirstServer : public rclcpp::Node
{
public:
    FirstServer()
        // 初始化节点，节点名为 custom_srv_server_cpp
        : Node("custom_srv_server_cpp")
    {
        // 创建服务：
        // 服务类型：CustomService
        // 服务名称：custom_srv_cpp_service
        // 回调函数：custom_srv_callback
        service_ = this->create_service<custom_srv_interfaces::srv::CustomService>(
            "custom_srv_cpp_service",
            std::bind(
                &FirstServer::custom_srv_callback,
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
    void custom_srv_callback(
        const std::shared_ptr<
            custom_srv_interfaces::srv::CustomService::Request> request,
        std::shared_ptr<
            custom_srv_interfaces::srv::CustomService::Response> response)
    {
        // 计算两个整数之和
        auto cur_sum = request->a + request->b;

        // 保存本次计算结果
        history_sums_.push_back(cur_sum);

        // 设置响应数据
        response->sum = cur_sum;
        response->history_sums = history_sums_;

        RCLCPP_INFO(
            this->get_logger(),
            "收到请求：\na: %d b: %d",
            request->a,
            request->b
        );
    }

    // 服务对象
    rclcpp::Service<
        custom_srv_interfaces::srv::CustomService>::SharedPtr service_;

    // 保存历史计算结果
    std::vector<int32_t> history_sums_;
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
