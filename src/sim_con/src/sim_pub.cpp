#include <ros/ros.h>
#include "ackermann_msgs/AckermannDriveStamped.h"
#include "ackermann_msgs/AckermannDrive.h"

void ackermann_update(ackermann_msgs::AckermannDrive base, float steer_angle, float steer_vel, float spd, float acc, float jerk){
    base.steering_angle = steer_angle;
    base.steering_angle_velocity = steer_vel;
    base.speed = spd;
    base.acceleration = acc;
    base.jerk = jerk;
}

int main(int argc, char** argv)
{
    float period = 1;
    ros::init(argc, argv, "sim_pub_drive_node");
    ros::NodeHandle nh;

    ackermann_msgs::AckermannDriveStamped adsmsg;
    ackermann_msgs::AckermannDrive driver;
    
    ros::Publisher vanila_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("drive",1000);
    ros::Rate loop_rate(period);
    
    //initial values
    float steer_angle_con = 0.5;
    float steer_angle_vel_con = 0;
    float spd_con = 1;
    float accel_con = 0;
    float jerk_con = 0;
    
    

    while(ros::ok())
    {
        if(true){
            driver.steering_angle = steer_angle_con;
            driver.steering_angle_velocity = steer_angle_vel_con;
            driver.speed = spd_con;
            driver.acceleration = accel_con;
            driver.jerk = jerk_con;
        }

        adsmsg.header.stamp = ros::Time::now();
        adsmsg.drive = driver;
        vanila_pub.publish(adsmsg);
        ros::spinOnce();

        loop_rate.sleep();
    }
    return 0;
}