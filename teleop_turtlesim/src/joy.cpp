#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joy.hpp"
#include "geometry_msgs/msg/twist.hpp"

class JoyTeleopTurtle : public rclcpp::Node
{
public:
    JoyTeleopTurtle() : Node("joy_teleop_turtle")
    {
        this->declare_parameter<std::string>("turtle_cmd_vel_topic", "turtle1/cmd_vel");
        this->get_parameter("turtle_cmd_vel_topic", turtle_cmd_vel_topic_);

        joy_subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
            "joy", 10, std::bind(&JoyTeleopTurtle::joy_callback, this, std::placeholders::_1));

        velocity_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>(turtle_cmd_vel_topic_, 10);
    }

private:
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
    {
        auto twist = geometry_msgs::msg::Twist();

        // Mapping from joystick to Twist message
        twist.linear.x = msg->axes[1]; // Assuming the left stick vertical controls forward/backward
        twist.angular.z = msg->axes[0]; // Assuming the left stick horizontal controls turning

        // Publish the message
        velocity_publisher_->publish(twist);
    }

    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr velocity_publisher_;
    std::string turtle_cmd_vel_topic_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<JoyTeleopTurtle>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
