#include <ros/ros.h>
#include "ackermann_msgs/AckermannDriveStamped.h"
#include "ackermann_msgs/AckermannDrive.h"

void ackermann_update(ackermann_msgs::AckermannDrive &base, 
 float steer_angle, float steer_vel, float spd, float acc, float jerk)
{
    base.steering_angle = steer_angle;
    base.steering_angle_velocity = steer_vel;
    base.speed = spd;
    base.acceleration = acc;
    base.jerk = jerk;
}

int main(int argc, char** argv)
{
    //define initial values
    float steer_angle     = 0.000;
    float steer_angle_vel = 0.000;
    float speed           = 0.000;
    float accel           = 1.000;
    float jerk            = 0.000;

    float period          = 1.000;
    
    //declare objects
    ros::init(argc, argv, "acc_pub_drive_node");
    ros::NodeHandle nh;

    ackermann_msgs::AckermannDriveStamped adsmsg;
    ackermann_msgs::AckermannDrive driver;
    
    ros::Publisher vanila_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("drive",1000);
    ros::Rate loop_rate(1.000 / period);
    
    //loop code
    while(ros::ok())
    {

        if(speed <= 2.0)
        {
           speed += accel * period;

            ackermann_update(driver, steer_angle, steer_angle_vel, speed, accel, jerk);
        }
        else // brake
        {
            speed = 0;
            accel = 0;
            ackermann_update(driver, steer_angle, steer_angle_vel, speed, accel, jerk);

        }


        adsmsg.header.stamp = ros::Time::now();
        adsmsg.drive = driver;
        vanila_pub.publish(adsmsg);

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}