/**
  ****************************(C) COPYRIGHT 2024 Polarbear*************************
  * @file       standard_robot_pp_ros2.hpp/cpp
  * @brief      上下位机通信模块
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Jul-18-2024     Penguin         1. done
  @verbatim
  =================================================================================

  =================================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2024 Polarbear*************************
  */
#ifndef STANDARD_ROBOT_PP_ROS2__ROS2_STANDARD_ROBOT_HPP_
#define STANDARD_ROBOT_PP_ROS2__ROS2_STANDARD_ROBOT_HPP_

#include <tf2_ros/transform_broadcaster.h>

#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <serial_driver/serial_driver.hpp>
#include <std_msgs/msg/float64.hpp>
#include <std_msgs/msg/int64.hpp>

#include <pb_rm_interfaces/msg/game_robot_hp.hpp>
#include <pb_rm_interfaces/msg/game_status.hpp>
#include <pb_rm_interfaces/msg/event_data.hpp>
#include <pb_rm_interfaces/msg/ground_robot_position.hpp>
#include <pb_rm_interfaces/msg/rfid_status.hpp>

#include "packet_typedef.hpp"
#include "robot_info.hpp"

namespace standard_robot_pp_ros2
{
class StandardRobotPpRos2Node : public rclcpp::Node
{
public:
  explicit StandardRobotPpRos2Node(const rclcpp::NodeOptions & options);

  ~StandardRobotPpRos2Node() override;

private:
  rclcpp::Time node_start_time_stamp;
  RobotModels robot_models_;
  bool usb_is_ok_;

  void getParams();

  // Cmmand related
  SendRobotCmdData send_robot_cmd_data_;

  // Debug data related
  std::unordered_map<std::string, rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr>
    debug_pub_map_;

  // Serial port
  std::unique_ptr<IoContext> owned_ctx_;
  std::string device_name_;
  std::unique_ptr<drivers::serial_driver::SerialPortConfig> device_config_;
  std::unique_ptr<drivers::serial_driver::SerialDriver> serial_driver_;

  // Publish
  rclcpp::Publisher<pb_rm_interfaces::msg::EventData>::SharedPtr event_data_pub_;
  rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_pub_;
  rclcpp::Publisher<pb_rm_interfaces::msg::GameRobotHP>::SharedPtr all_robot_hp_pub_;
  rclcpp::Publisher<pb_rm_interfaces::msg::GameStatus>::SharedPtr game_progress_pub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr robot_motion_pub_;
  rclcpp::Publisher<pb_rm_interfaces::msg::GroundRobotPosition>::SharedPtr ground_robot_position_pub_;
  rclcpp::Publisher<pb_rm_interfaces::msg::RfidStatus>::SharedPtr rfid_status_pub_;

  std::unique_ptr<tf2_ros::TransformBroadcaster> imu_tf_broadcaster_;

  void createPublisher();
  void createNewDebugPublisher(const std::string & name);

  void publishDebugData(ReceiveDebugData & debug_data);
  void publishImuData(ReceiveImuData & imu_data);
  void publishEventData(ReceiveEventDate & event_data);
  void publishAllRobotHp(ReceiveAllRobotHpData & all_robot_hp);
  void publishGameStatus(ReceiveGameStatusData & game_status);
  void publishRobotMotion(ReceiveRobotMotionData & robot_motion);
  void publishGroundRobotPosition(ReceiveGroundRobotPosition & ground_robot_position);
  void publishRfidStatus(ReceiveRfidStatus & rfid_status);

  // Subscribe
  rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_sub_;
  void createSubscription();
  void updateCmdVel(const geometry_msgs::msg::Twist::SharedPtr msg);

  // receive_thread
  std::thread receive_thread_;
  void receiveData();

  // send thread
  std::thread send_thread_;
  void sendData();

  // Serial port protect thread
  std::thread serial_port_protect_thread_;
  void serialPortProtect();
};
}  // namespace standard_robot_pp_ros2

#endif  // STANDARD_ROBOT_PP_ROS2__ROS2_STANDARD_ROBOT_HPP_