#include <ros/ros.h>
#include <signal.h>
#include <turtle_mpc_controller/mpc.h>  
#include <geometry_msgs/Twist.h>  
#include <global_planner/global_planner.h>
#include <common/time.hpp>
#include <iostream>
#include <fstream>

/*
ros::Publisher cmdVelPub;  
void shutdown(int sig)  
{  
  cmdVelPub.publish(geometry_msgs::Twist());
  ROS_INFO("move_turtle_goforward ended!");  
  ros::shutdown();  
}  
*/


int main(int argc, char** argv)  
{  
    /*
  ros::init(argc, argv, "turtle_mpc_controller");
  ros::NodeHandle node; 
  //ModelPredictiveController mpc_turtle;
  cmdVelPub = node.advertise<geometry_msgs::Twist>("/mobile_base/commands/velocity", 1);  
  ros::Rate loopRate(10);  
  signal(SIGINT, shutdown);  
  ROS_INFO("turtlebot controller start...");  
  geometry_msgs::Twist speed; 
  while (ros::ok())  
  {  
    mpc_turtle.update();
    speed.linear.x = mpc_turtle.output.velocity; 
    // the actual control variable is the angle position, this need to be transformed 
    speed.angular.z = mpc_turtle.output.omiga;
    cmdVelPub.publish(speed);  
    loopRate.sleep();  
  }  
  */
    ros::init(argc, argv, "globalplanner");
    ros::NodeHandle node;
    std::ofstream outputfile;
    common::Clock clock;

    ROS_INFO_STREAM("start a star");
    GlobalPlanner globalPlanner(1000,1000,25,50,25);
    Point start(0,0);
    Point end(999,999);
    std::list<Point*> path = globalPlanner.solve(start, end);
    ROS_INFO_STREAM("Took " << clock.takeRealTime() << "ms.");

    ROS_INFO_STREAM("start txt output");
    outputfile.open("/home/mpc/course_project/src/turtle_mpc_controller/output/path.txt");
    if(outputfile.is_open()){
      for(auto &p:path){
        outputfile << p->x << " " << p->y << std::endl;
      }
    }
    outputfile.close();
    ROS_INFO_STREAM("txt output successfully");

    ros::spin();
    return 0;  
}  