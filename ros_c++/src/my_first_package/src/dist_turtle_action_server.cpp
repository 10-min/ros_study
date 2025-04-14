#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "my_first_package_msgs/action/dist_turtle.hpp"

using namespace std::placeholders;

class DistTurtleServer : public rclcpp::Node {
    public:
        using DistTurtle = my_first_package_msgs::action::DistTurtle;
        using GoalHandleDistTurtle = rclcpp_action::ServerGoalHandle<DistTurtle>
        explicit DistTurtleServer(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
        : Node("dist_turtle_action_server") {
            action_server = rclcpp_action::create_server<DistTurtle>(
                this, 
                "dist_turtle", 
                std::bind(&DistTurtleServer::handle_goal, this, _1, _2),
                std::bind(&DistTurtleServer::handle_cancle, this, _1),
                std::bind(&DistTurtleServer::handle_accepted, this, _1)
            );
        }

    private:
        rclcpp_action::Server<DistTurtle>::SharedPtr action_server;
        
        rclcpp_action::GoalResponse handle_goal(
            const rclcpp_action::GoalUUID & uuid,
            std::shared_ptr<const Fibonacci::Goal> goal)
        {
            RCLCPP_INFO(this->get_logger(), "Received goal request with order %d", goal->order);
            (void)uuid;
            return rclcpp_action::GoalResponse::ACCEPT_AND_EXECUTE;
        }
        
        rclcpp_action::CancelResponse handle_cancel(
            const std::shared_ptr<GoalHandleFibonacci> goal_handle)
        {
            RCLCPP_INFO(this->get_logger(), "Received request to cancel goal");
            (void)goal_handle;
            return rclcpp_action::CancelResponse::ACCEPT;
        }
        
        void handle_accepted(const std::shared_ptr<GoalHandleDistTurtle> goal_handle)
        {
            using namespace std::placeholders;
            // this needs to return quickly to avoid blocking the executor, so spin up a new thread
            std::thread{std::bind(&DistTurtleServer::execute_callback, this, _1), goal_handle}.detach();
        }

        rclcpp_action::GoalResponse execute_callback(const rclcpp_action::GoalUUID & uuid,
            std::shared_ptr<const DistTurtle::Goal> goal) 
        {
            if (rclcpp::ok()) {
                auto result = DistTurtle::Result();
                goal_handle->succeed(result);
            }
        }                                                       
        
}

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    
}