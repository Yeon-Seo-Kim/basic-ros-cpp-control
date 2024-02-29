#include<ros/ros.h>
#include"geometry_msgs/PoseStamped.h"

void poseCallback(const geometry_msgs::PoseStamped::ConstPtr& object)
{
    static ros::Time lastouttime = ros::Time::now();
    ros::Duration delta_time = ros::Time::now() - lastouttime;

    
    if(delta_time.toSec()>=1.0){
        ROS_INFO("subscribe gt_pose : position.x = %.3f , position.y = %.3f , orientation.z = %.3f , orientation.w = %.3f", object->pose.position.x , object->pose.position.y, object->pose.orientation.z, object->pose.orientation.w
        );
    
        if( object->pose.position.x >=1.0 && object->pose.position.x <= 2.0 && object->pose.position.y >= 1.0 && object->pose.position.y <= 2.0)
        {
            ROS_INFO("destination reached!!!");
        }

        lastouttime = ros::Time::now();
    }
    
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "pose_sub");
    ros::NodeHandle nh;

    ros::Subscriber vanila_sub
     = nh.subscribe("gt_pose", 1000, poseCallback);

    ros::Rate loop_rate(1);
    while(ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}