cmake_minimum_required(VERSION 3.8)
project(my_first_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(turtlesim REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(my_first_package_msgs REQUIRED)

add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker rclcpp std_msgs)

add_executable(listener src/subscriber_member_function.cpp)
ament_target_dependencies(listener rclcpp std_msgs)

add_executable(server src/add_two_ints_server.cpp)
ament_target_dependencies(server rclcpp example_interfaces)

add_executable(client src/add_two_ints_client.cpp)
ament_target_dependencies(client rclcpp example_interfaces)

add_executable(subscriber src/my_subscriber.cpp)
ament_target_dependencies(subscriber rclcpp turtlesim)

add_executable(publisher src/my_publisher.cpp)
ament_target_dependencies(publisher rclcpp geometry_msgs)

add_executable(sub_pose src/turtle_cmd_and_pose.cpp)
ament_target_dependencies(sub_pose rclcpp turtlesim my_first_package_msgs geometry_msgs)

add_executable(my_service_server src/my_service_server.cpp)
ament_target_dependencies(my_service_server rclcpp my_first_package_msgs turtlesim)

install(TARGETS
  talker
  listener
  server
  client
  subscriber
  publisher
  sub_pose
  my_service_server
  DESTINATION lib/${PROJECT_NAME})

ament_package()
