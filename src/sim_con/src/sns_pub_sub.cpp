#include<vector>
#include<string>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cmath>

#include <ros/ros.h>
#include "ackermann_msgs/AckermannDriveStamped.h"
#include "geometry_msgs/PoseStamped.h"

// declare current pose, waypoint position

class current_pose{
private:
    float cpx;
    float cpy;
    float coz;
    float cow;
public:
    float get_px(){return cpx;}
    float get_py(){return cpy;}
    float get_oz(){return coz;}
    float get_ow(){return cow;}

    // return steering angle for pure pursuit
    void update(float ncpx, float ncpy, float ncoz, float ncow)
    {
        cpx = ncpx; cpy = ncpy; coz = ncoz; cow = ncow;
    }
    float goal_steer(float gpx, float gpy)
    {
        float ori_angle = 1.000 - 2.000 * pow(coz, 2) == 0.000 ?
            1.571 : atan2(2 * coz * cow, 1.000 - 2.000 * pow(coz, 2));
        float g_v_angle = gpx == cpx ? gpy > cpy ? 1.571 : -1.571 : atan2(gpy - cpy, gpx - cpx);
        float distance = sqrt((gpx - cpx) * (gpx - cpx) + (gpy - cpy) * (gpy - cpy));
        return atan(1.200 * sin(ori_angle - g_v_angle) / distance);
    }
};

current_pose curpose;
float goal_steer_angle;
// update motion
void ackermann_update(ackermann_msgs::AckermannDrive &base, float steer_angle, float steer_vel, float spd, float acc, float jerk)
{
    base.steering_angle = steer_angle;
    base.steering_angle_velocity = steer_vel;
    base.speed = spd;
    base.acceleration = acc;
    base.jerk = jerk;
}

// update current pose
void poseUpdate(const geometry_msgs::PoseStamped::ConstPtr object, float& pos_x, float& pos_y, float& ori_z, float& ori_w)
{
    pos_x = object->pose.position.x;
    pos_y = object->pose.position.y;
    ori_z = object->pose.orientation.z;
    ori_w = object->pose.orientation.w;
}

// callback function
void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& object)
{
    static ros::Time lastouttime = ros::Time::now();
    ros::Duration delta_time = ros::Time::now() - lastouttime;
    //poseUpdate(object, cpx, cpy, coz, cow);
    curpose.update(object->pose.position.x , object->pose.position.y, object->pose.orientation.z, object->pose.orientation.w);
    goal_steer_angle = curpose.goal_steer(1.000,-1.000);

    if(delta_time.toSec()>=1.0)
    {
        ROS_INFO("subscribe gt_pose : position.x = %.3f , position.y = %.3f , orientation.z = %.3f , orientation.w = %.3f", object->pose.position.x , object->pose.position.y, object->pose.orientation.z, object->pose.orientation.w);
        ROS_INFO("goal steering angle: %.3f", goal_steer_angle);


    
        /*if( object->pose.position.x >=1.0 && object->pose.position.x <= 2.0 && object->pose.position.y >= 1.0 && object->pose.position.y <= 2.0)
        {
            ROS_INFO("destination reached!!!");
        }
        */


        lastouttime = ros::Time::now();
    }
}

//using object : current_pose curpose
//using variable: float goal_steer_angle

int main(int argc, char** argv)
{
    //define initial values
    float steer_angle     = 0.000;
    float steer_angle_vel = 0.000;
    float speed           = 0.400;
    float accel           = 0.000;
    float jerk            = 0.000;

    float period          = 1.000;


    //declare node, publisher, subscriber
    ros::init(argc, argv, "sns_pub_sub");
    ros::NodeHandle nh;

    ackermann_msgs::AckermannDriveStamped adsmsg;
    ackermann_msgs::AckermannDrive driver;

    ros::Publisher vanila_pub = nh.advertise<ackermann_msgs::AckermannDriveStamped>("drive",1000);
    ros::Subscriber vanila_sub = nh.subscribe("gt_pose", 1000, poseCallback);

    ros::Rate loop_rate(1.000/period);
    
    // initial pose update
    ros::spinOnce();
    loop_rate.sleep();

    while(ros::ok())
    {

        adsmsg.header.stamp = ros::Time::now();
        adsmsg.drive = driver;
        vanila_pub.publish(adsmsg);

        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}