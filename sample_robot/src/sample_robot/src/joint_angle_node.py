import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState

class JointMover(Node):
    def __init__(self):
        super().__init__('joint_mover')
        self.publisher = self.create_publisher(JointState, 'joint_states', 10)
        self.timer = self.create_timer(0.1, self.timer_callback)  # 10 Hz

        self.joint_names = ['joint1', 'joint2']
        self.angle = 0.0
        self.direction = 1

    def timer_callback(self):
        msg = JointState()
        msg.header.stamp = self.get_clock().now().to_msg()
        msg.name = self.joint_names
        msg.position = [self.angle for _ in self.joint_names]

        self.publisher.publish(msg)

        # Oscillate angle between -1.0 and +1.0 radians
        self.angle += 0.05 * self.direction
        if abs(self.angle) >= 1.0:
            self.direction *= -1

def main(args=None):
    rclpy.init(args=args)
    node = JointMover()
    rclpy.spin(node)
    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()
