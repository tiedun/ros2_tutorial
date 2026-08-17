#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "custom_action_interfaces/action/custom_action.hpp"

#include <chrono>
#include <functional>
#include <memory>
#include <sstream>
#include <thread>

using namespace std::chrono_literals;

// 自定义动作服务端节点
class FirstActionServer : public rclcpp::Node
{
public:
    // 动作类型
    using CustomAction =
        custom_action_interfaces::action::CustomAction;

    // 动作服务端目标句柄类型
    using GoalHandleCustomAction =
        rclcpp_action::ServerGoalHandle<CustomAction>;

    FirstActionServer()
        // 初始化节点，节点名为 custom_action_server_cpp
        : Node("custom_action_server_cpp")
    {
        // 创建动作服务端：
        // 动作类型：CustomAction
        // 动作名称：custom_action_cpp_action
        // 目标处理回调函数：handle_goal
        // 取消处理回调函数：handle_cancel
        // 目标接受回调函数：handle_accepted
        action_server_ =
            rclcpp_action::create_server<CustomAction>(
                this,
                "custom_action_cpp_action",
                std::bind(
                    &FirstActionServer::handle_goal,
                    this,
                    std::placeholders::_1,
                    std::placeholders::_2
                ),
                std::bind(
                    &FirstActionServer::handle_cancel,
                    this,
                    std::placeholders::_1
                ),
                std::bind(
                    &FirstActionServer::handle_accepted,
                    this,
                    std::placeholders::_1
                )
            );
    }

private:
    // 目标处理回调函数：
    // uuid 保存本次目标对应的唯一标识
    // goal 保存客户端发送的目标数据
    //
    // 返回值用于决定是否接受该目标
    rclcpp_action::GoalResponse handle_goal(
        const rclcpp_action::GoalUUID &uuid,
        std::shared_ptr<const CustomAction::Goal> goal)
    {
        // 本示例不使用目标 UUID
        (void)uuid;

        RCLCPP_INFO(
            this->get_logger(),
            "收到目标：n = %d",
            goal->n
        );

        // 接受目标并立即进入执行状态
        return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
    }

    // 取消处理回调函数：
    // goal_handle 保存请求取消的目标
    //
    // 当前示例与 Python 版本保持一致，不接受取消请求
    rclcpp_action::CancelResponse handle_cancel(
        const std::shared_ptr<GoalHandleCustomAction> goal_handle)
    {
        // 本示例不使用目标句柄
        (void)goal_handle;

        RCLCPP_INFO(
            this->get_logger(),
            "收到取消目标请求，本示例不支持取消目标。"
        );

        // 拒绝取消目标
        return rclcpp_action::CancelResponse::REJECT;
    }

    // 目标接受回调函数：
    // goal_handle 保存已经接受的目标以及当前目标的执行状态
    void handle_accepted(
        const std::shared_ptr<GoalHandleCustomAction> goal_handle)
    {
        // 目标执行过程可能持续较长时间，
        // 因此创建独立线程执行 execute()，
        // 避免阻塞 ROS 2 Executor
        std::thread{
            std::bind(
                &FirstActionServer::execute,
                this,
                goal_handle
            )
        }.detach();
    }

    // 动作执行函数：
    // goal_handle 保存客户端发送的目标以及当前目标的执行状态
    void execute(
        const std::shared_ptr<GoalHandleCustomAction> goal_handle)
    {
        // 获取客户端发送的目标
        const auto goal = goal_handle->get_goal();

        RCLCPP_INFO(
            this->get_logger(),
            "开始执行目标：n = %d",
            goal->n
        );

        // 创建反馈对象
        auto feedback =
            std::make_shared<CustomAction::Feedback>();

        // 设置 Fibonacci 数列的初始值
        feedback->partial_sequence = {0, 1};

        // 根据客户端发送的目标计算 Fibonacci 数列
        for (int i = 1; i < goal->n && rclcpp::ok(); ++i)
        {
            // 计算下一个数
            auto next_number =
                feedback->partial_sequence[
                    feedback->partial_sequence.size() - 1
                ]
                +
                feedback->partial_sequence[
                    feedback->partial_sequence.size() - 2
                ];

            // 保存计算结果
            feedback->partial_sequence.push_back(next_number);

            // 发布动作反馈
            goal_handle->publish_feedback(feedback);

            // 将反馈数据转换为字符串
            std::ostringstream feedback_stream;
            feedback_stream << "[";

            for (
                std::size_t j = 0;
                j < feedback->partial_sequence.size();
                ++j)
            {
                feedback_stream
                    << feedback->partial_sequence[j];

                if (
                    j
                    < feedback->partial_sequence.size() - 1)
                {
                    feedback_stream << ", ";
                }
            }

            feedback_stream << "]";

            // 输出反馈数据
            RCLCPP_INFO(
                this->get_logger(),
                "发送反馈：%s",
                feedback_stream.str().c_str()
            );

            // 模拟动作执行过程
            std::this_thread::sleep_for(100ms);
        }

        // ROS 2 已经关闭时，不再继续设置动作结果
        if (!rclcpp::ok())
        {
            return;
        }

        // 创建结果对象
        auto result =
            std::make_shared<CustomAction::Result>();

        // 设置最终结果
        result->sequence = feedback->partial_sequence;

        // 将当前目标设置为执行成功状态
        goal_handle->succeed(result);

        // 将最终结果转换为字符串
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

        RCLCPP_INFO(
            this->get_logger(),
            "目标执行完成，最终结果：%s",
            result_stream.str().c_str()
        );
    }

    // 动作服务端对象
    rclcpp_action::Server<CustomAction>::SharedPtr
        action_server_;
};

int main(int argc, char **argv)
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建动作服务端节点
    auto action_server_node =
        std::make_shared<FirstActionServer>();

    // 保持节点运行，等待并处理客户端发送的目标
    rclcpp::spin(action_server_node);

    // 销毁节点并关闭 ROS 2
    action_server_node.reset();
    rclcpp::shutdown();

    return 0;
}
