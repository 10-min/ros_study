#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include <chrono>
#include <memory>

using namespace std::chrono_literals;

class TurtlesimPublisher : public rclcpp::Node {
    public:
        TurtlesimPublisher() : Node("turtlesim_publisher") {
            publisher = this->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
            timer_ = this->create_wall_timer(500ms, std::bind(&TurtlesimPublisher::timer_callback, this));
        }

    private:
        void timer_callback() {
            auto msg = geometry_msgs::msg::Twist();
            msg.linear.x = 2.0;
            msg.angular.z = 2.0;
            publisher->publish(msg);
        }
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> pub = std::make_shared<TurtlesimPublisher>();
    rclcpp::spin(pub);
    rclcpp::shutdown();
}