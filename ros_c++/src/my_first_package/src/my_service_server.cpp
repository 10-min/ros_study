#include "my_first_package_msgs/srv/multi_spawn.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/teleport_absolute.hpp"
#include "turtlesim/srv/spawn.hpp"

#include <memory>
#include <chrono>
#include <vector>
#include <cmath>
#include <thread> 

using std::placeholders::_1, std::placeholders::_2;
using namespace std::chrono_literals;
using namespace std;

class MultiSpawning : public rclcpp::Node {
    public:
        MultiSpawning() : rclcpp::Node("multi_spawn") {
            server = this->create_service<my_first_package_msgs::srv::MultiSpawn>("multi_spawn", 
                    std::bind(&MultiSpawning::callback_service, this, _1, _2));
            
            teleport = this->create_client<turtlesim::srv::TeleportAbsolute>("/turtle1/teleport_absolute");

            req_teleport = std::make_shared<turtlesim::srv::TeleportAbsolute::Request>();

            spawn = this->create_client<turtlesim::srv::Spawn>("/spawn");

            req_spawn = std::make_shared<turtlesim::srv::Spawn::Request>();
        }

    private:
        void callback_service(const std::shared_ptr<my_first_package_msgs::srv::MultiSpawn::Request> request,
                            std::shared_ptr<my_first_package_msgs::srv::MultiSpawn::Response> response) {
                                vector<vector<double>> x_y_theta = calc_position(request->num, 3);
                                vector<double> x = x_y_theta[0];
                                vector<double> y = x_y_theta[1];
                                vector<double> theta = x_y_theta[2];

                                for (int i = 0; i < x.size(); i++) {
                                    req_spawn->x = x[i] + center_x;
                                    req_spawn->y = y[i] + center_y;
                                    req_spawn->theta = theta[i];

                                    while (!spawn->wait_for_service(1s)) {
                                        if (!rclcpp::ok()) {
                                          RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
                                          return;
                                        }
                                        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
                                    }
                                    spawn->async_send_request(req_spawn);
                                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                }

                                response->x = x;
                                response->y = y;
                                response->theta = theta;
                            }
        rclcpp::Service<my_first_package_msgs::srv::MultiSpawn>::SharedPtr server;
        rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedPtr teleport;
        rclcpp::Client<turtlesim::srv::TeleportAbsolute>::SharedRequest req_teleport;
        rclcpp::Client<turtlesim::srv::Spawn>::SharedPtr spawn;
        rclcpp::Client<turtlesim::srv::Spawn>::SharedRequest req_spawn;
        double center_x = 5.54;
        double center_y = 5.54;

        vector<vector<double>> calc_position(int n, double r) {
            double gap_theta = 2 * M_PI / n;
            vector<vector<double>> temp;
            vector<double> theta;
            vector<double> x;
            vector<double> y;

            for (int i = 0; i < n; i++) {
                theta.push_back(i * gap_theta);
                x.push_back(r * cos(theta[i]));
                y.push_back(r * sin(theta[i]));
            }
            temp.push_back(x);
            temp.push_back(y);
            temp.push_back(theta);
            
            return temp;
        }
};

int main(int argc, char ** argv) {
    rclcpp::init(argc, argv);
    std::shared_ptr<rclcpp::Node> multi_spawn = std::make_shared<MultiSpawning>();
    rclcpp::spin(multi_spawn);
    rclcpp::shutdown();
    return 0;
}