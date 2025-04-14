#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"

using std::placeholders::_1;

class TurtlesimSubscriber: public rclcpp::Node {
    public:
        TurtlesimSubscriber() : Node("turtlesim_subscriber") {
            subscription = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose", 10, std::bind(&TurtlesimSubscriber::callback, this, _1)
            );
        }
    private:
        void callback(const turtlesim::msg::Pose::SharedPtr msg) const {
            RCLCPP_INFO(this->get_logger(), "X : %lf, Y : %lf", msg->x, msg->y);
        }
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr subscription;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> sub = std::make_shared<TurtlesimSubscriber>();
    rclcpp::spin(sub);
    rclcpp::shutdown();
    return 0;
}