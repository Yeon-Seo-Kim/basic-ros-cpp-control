#include <ros/ros.h>
#include "ackermann_msgs/AckermannDriveStamped.h"
#include "ackermann_msgs/AckermannDrive.h"

int main(int argc, char** argv)
{
    ros::init(argc, argv, "vanila_pub");
    ros::NodeHandle nh;

    ackermann_msgs::AckermannDrive driver;
    
    ackermann_msgs::AckermannDriveStamped adsmsg;
    std_msgs::Header head;
    ros::Publisher vanila_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("drive_custom",1000);
    ros::Rate loop_rate(1);
    
    float steer_angle_con = 0;
    float steer_angle_vel_con = 0;
    float spd_con = 0;
    float accel_con = 0;
    float jerk_con = 0;

    


    while(ros::ok())
    {
        driver.steering_angle = steer_angle_con;
        driver.steering_angle_velocity = steer_angle_vel_con;
        driver.speed = spd_con;
        driver.acceleration = accel_con;
        driver.jerk = jerk_con;

        head.stamp = ros::Time::now();
        head.frame_id = "Vanila";
        head.seq = 1;

        adsmsg.drive = driver;
        adsmsg.header = head;

        vanila_pub.publish(adsmsg);
        ros::spinOnce();

        loop_rate.sleep();
    }
    return 0;
}