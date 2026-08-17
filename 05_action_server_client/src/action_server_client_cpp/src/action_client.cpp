#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "custom_action_interfaces/action/custom_action.hpp"

#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>

using namespace std::chrono_literals;

// 自定义动作客户端节点
class FirstActionClient : public rclcpp::Node
{
public:
    // 动作类型
    using CustomAction =
        custom_action_interfaces::action::CustomAction;

    // 动作客户端目标句柄类型
    using GoalHandleCustomAction =
        rclcpp_action::ClientGoalHandle<CustomAction>;

    FirstActionClient()
        // 初始化节点，节点名为 custom_action_client_cpp
        : Node("custom_action_client_cpp")
    {
        // 创建动作客户端：
        // 动作类型：CustomAction
        // 动作名称：custom_action_cpp_action
        action_client_ =
            rclcpp_action::create_client<CustomAction>(
                this,
                "custom_action_cpp_action"
            );
    }

    // 发送动作目标
    void send_goal(int32_t n)
    {
        // 创建目标对象
        CustomAction::Goal goal;

        // 设置目标参数
        goal.n = n;

        // 等待动作服务端启动
        RCLCPP_INFO(
            this->get_logger(),
            "正在等待动作服务端启动..."
        );

        while (!action_client_->wait_for_action_server(1s))
        {
            if (!rclcpp::ok())
            {
                RCLCPP_ERROR(
                    this->get_logger(),
                    "等待动作服务端时 ROS 2 已关闭。"
                );
                return;
            }
        }

        // 创建发送目标选项对象
        auto send_goal_options =
            rclcpp_action::Client<
                CustomAction>::SendGoalOptions();

        // 设置目标响应回调函数
        send_goal_options.goal_response_callback =
            std::bind(
                &FirstActionClient::goal_response_callback,
                this,
                std::placeholders::_1
            );

        // 设置动作反馈回调函数
        send_goal_options.feedback_callback =
            std::bind(
                &FirstActionClient::feedback_callback,
                this,
                std::placeholders::_1,
                std::placeholders::_2
            );

        // 设置动作结果回调函数
        send_goal_options.result_callback =
            std::bind(
                &FirstActionClient::get_result_callback,
                this,
                std::placeholders::_1
            );

        // 异步发送目标：
        // goal               需要发送的目标对象
        // send_goal_options  保存目标响应、反馈和结果回调函数
        action_client_->async_send_goal(
            goal,
            send_goal_options
        );
    }

private:
    // 目标响应回调函数
    void goal_response_callback(
        const GoalHandleCustomAction::SharedPtr &goal_handle)
    {
        // 空指针表示动作服务端拒绝了目标
        if (!goal_handle)
        {
            RCLCPP_INFO(
                this->get_logger(),
                "目标被拒绝"
            );

            rclcpp::shutdown();
            return;
        }

        RCLCPP_INFO(
            this->get_logger(),
            "目标已接受，等待执行结果..."
        );
    }

    // 动作反馈回调函数
    void feedback_callback(
        GoalHandleCustomAction::SharedPtr goal_handle,
        const std::shared_ptr<
            const CustomAction::Feedback> feedback)
    {
        // 本回调中不需要使用目标句柄
        (void)goal_handle;

        // 将反馈数据转换为字符串
        std::ostringstream feedback_stream;
        feedback_stream << "[";

        for (
            std::size_t i = 0;
            i < feedback->partial_sequence.size();
            ++i)
        {
            feedback_stream
                << feedback->partial_sequence[i];

            if (
                i
                < feedback->partial_sequence.size() - 1)
            {
                feedback_stream << ", ";
            }
        }

        feedback_stream << "]";

        // 输出反馈数据
        RCLCPP_INFO(
            this->get_logger(),
            "收到反馈：%s",
            feedback_stream.str().c_str()
        );
    }

    // 动作结果回调函数
    void get_result_callback(
        const GoalHandleCustomAction::WrappedResult &wrapped_result)
    {
        // 判断目标最终执行状态
        switch (wrapped_result.code)
        {
            case rclcpp_action::ResultCode::SUCCEEDED:
                break;

            case rclcpp_action::ResultCode::ABORTED:
                RCLCPP_ERROR(
                    this->get_logger(),
                    "目标执行被中止。"
                );

                rclcpp::shutdown();
                return;

            case rclcpp_action::ResultCode::CANCELED:
                RCLCPP_INFO(
                    this->get_logger(),
                    "目标已取消。"
                );

                rclcpp::shutdown();
                return;

            default:
                RCLCPP_ERROR(
                    this->get_logger(),
                    "收到未知的目标结果状态。"
                );

                rclcpp::shutdown();
                return;
        }

        // 获取动作结果
        const auto result = wrapped_result.result;

        // 将动作结果转换为字符串
        std::ostringstream result_stream;
        result_stream << "[";

        for (
            std::size_t i = 0;
            i < result->sequence.size();
            ++i)
        {
            result_stream << result->sequence[i];

            if (i < result->sequence.size() - 1)
            {
                result_stream << ", ";
            }
        }

        result_stream << "]";

        // 输出动作结果
        RCLCPP_INFO(
            this->get_logger(),
            "收到结果：%s",
            result_stream.str().c_str()
        );

        // 等待键盘输入，暂缓执行后续代码，便于调试观察
        std::cin.get();
        
        // 已经获得最终结果，关闭ROS 2上下文，终止spin()循环
        rclcpp::shutdown();
    }

    // 动作客户端对象
    rclcpp_action::Client<CustomAction>::SharedPtr
        action_client_;
};

int main(int argc, char **argv)
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 检查命令行参数
    if (argc != 2)
    {
        RCLCPP_ERROR(
            rclcpp::get_logger("custom_action_client_cpp"),
            "用法：custom_action_client_cpp N"
        );

        rclcpp::shutdown();
        return 1;
    }

    // 获取命令行参数
    int32_t n = std::atoi(argv[1]);

    // 创建动作客户端节点
    auto action_client_node =
        std::make_shared<FirstActionClient>();

    // 发送动作目标
    action_client_node->send_goal(n);

    // 保持节点运行，
    // 等待并处理目标响应、动作反馈和最终结果
    rclcpp::spin(action_client_node);

    // 销毁节点并关闭 ROS 2
    action_client_node.reset();

    return 0;
}
