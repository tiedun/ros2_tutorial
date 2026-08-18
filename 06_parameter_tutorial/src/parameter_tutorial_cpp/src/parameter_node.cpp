#include <chrono>
#include <memory>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"

using namespace std::chrono_literals;

// 自定义参数节点
class FirstParam : public rclcpp::Node
{
public:
    FirstParam()
    : Node("param_node_cpp")
    {
        // 创建参数描述信息
        // description：参数的说明信息
        rcl_interfaces::msg::ParameterDescriptor first_parameter_descriptor;
        first_parameter_descriptor.description = "用于学习的第一个参数~";

        // 声明参数：
        // 参数名称：first_parameter
        // 默认值：world
        // 参数描述：first_parameter_descriptor
        this->declare_parameter<std::string>(
            "first_parameter",
            "world",
            first_parameter_descriptor
        );

        // 创建定时器：
        // 每 1 秒调用一次 timer_callback()
        timer_ = this->create_wall_timer(
            1s,
            std::bind(
                &FirstParam::timer_callback,
                this
            )
        );
    }

private:
    // 定时器回调函数
    void timer_callback()
    {
        // 获取 first_parameter 参数
        // get_parameter() 获取参数对象
        // as_string() 获取其中保存的字符串值
        std::string first_parameter =
            this->get_parameter("first_parameter").as_string();

        // 输出当前参数值
        RCLCPP_INFO(
            this->get_logger(),
            "Hello %s!",
            first_parameter.c_str()
        );

        // 创建 Parameter 对象：
        // 参数名称：first_parameter
        // 参数值：world
        // C++ 可以根据参数值自动确定参数类型为 STRING
        rclcpp::Parameter default_param(
            "first_parameter",
            "world"
        );

        // set_parameters() 接收 Parameter 对象列表
        std::vector<rclcpp::Parameter> all_parameters = {
            default_param
        };

        // 将 first_parameter 参数重新设置为 world
        this->set_parameters(all_parameters);
    }

    // 定时器对象
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
    // 初始化 ROS 2 C++ 客户端库
    rclcpp::init(argc, argv);

    // 创建自定义参数节点
    auto node = std::make_shared<FirstParam>();

    // 进入事件循环，使节点持续运行并处理定时器等回调
    rclcpp::spin(node);

    // 关闭 ROS 2 Context
    rclcpp::shutdown();

    return 0;
}
