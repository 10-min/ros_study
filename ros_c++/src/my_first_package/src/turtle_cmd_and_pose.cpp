#include "rclcpp/rclcpp.hpp"
#include "turtlesim/msg/pose.hpp"
#include "my_first_package_msgs/msg/cmd_and_pose_vel.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"

#include <memory>
#include <chrono>

using namespace std::chrono_literals;

using std::placeholders::_1;

class CmdAndPose : public rclcpp::Node {
    public:
        CmdAndPose() : Node("turtle_cmd_pose") {
            sub_pose = this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose", 10, std::bind(&CmdAndPose::callback_pose, this, _1)
            );
            sub_cmdvel = this->create_subscription<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel", 10, std::bind(&CmdAndPose::callback_cmd, this, _1)
            );
            
            timer = this->create_wall_timer(1s, std::bind(&CmdAndPose::timer_callback, this));
            publisher = this->create_publisher<my_first_package_msgs::msg::CmdAndPoseVel>(
                "/cmd_and_pose", 10
            );
        }
    private:
        void callback_pose(const turtlesim::msg::Pose::SharedPtr msg) {
            
            cmd_pose.pose_x = msg->x;
            cmd_pose.pose_y = msg->y;
            cmd_pose.linear_vel = msg->linear_velocity;
            cmd_pose.angular_vel = msg->angular_velocity;
        }

        void callback_cmd(const geometry_msgs::msg::Twist::SharedPtr msg) {
            cmd_pose.cmd_vel_linear = msg->linear.x;
            cmd_pose.cmd_vel_angular = msg->angular.z;
            RCLCPP_INFO(this->get_logger(), "Subscribe: pose_x : %lf, pose_y : %lf, linear x : %lf, angular z : %lf", 
                        cmd_pose.pose_x, cmd_pose.pose_y, cmd_pose.cmd_vel_linear, cmd_pose.cmd_vel_angular);
        }

        void timer_callback() {
            publisher->publish(cmd_pose);
        }
        rclcpp::Subscription<turtlesim::msg::Pose>::SharedPtr sub_pose;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_cmdvel;
        rclcpp::Publisher<my_first_package_msgs::msg::CmdAndPoseVel>::SharedPtr publisher;
        rclcpp::TimerBase::SharedPtr timer;
        my_first_package_msgs::msg::CmdAndPoseVel cmd_pose;
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CmdAndPose>());
    rclcpp::shutdown();
}