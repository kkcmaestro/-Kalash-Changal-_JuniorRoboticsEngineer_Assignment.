#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

class JointMover : public rclcpp::Node
{
public:
  JointMover()
  : Node("joint_mover"), angle_(0.0), step_(0.05), state_(0)
  {
    publisher_ = this->create_publisher<sensor_msgs::msg::JointState>("joint_states", 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(200), std::bind(&JointMover::timer_callback, this));
  }

private:
  void timer_callback()
  {
    sensor_msgs::msg::JointState msg;
    msg.header.stamp = this->get_clock()->now();
    msg.name = {"joint1", "joint2"};
    msg.position.resize(2);

    switch (state_) {
      case 0:
        joint1_ += step_;
        if (joint1_ >= 1.0) state_ = 1;
        break;
      case 1:
        joint2_ += step_;
        if (joint2_ >= 1.0) state_ = 2;
        break;
      case 2:
        joint1_ -= step_;
        if (joint1_ <= 0.0) state_ = 3;
        break;
      case 3:
        joint2_ -= step_;
        if (joint2_ <= 0.0) state_ = 0;
        break;
    }

    msg.position[0] = joint1_;
    msg.position[1] = joint2_;
    publisher_->publish(msg);
  }

  rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;

  double joint1_ = 0.0;
  double joint2_ = 0.0;
  double angle_;
  double step_;
  int state_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<JointMover>());
  rclcpp::shutdown();
  return 0;
}
