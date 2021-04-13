 #include <ros/ros.h>
 #include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <iostream>
#include "autoware_msgs/DetectedObjectArray.h"
#include "autoware_msgs/DetectedObject.h"
#include "laser_geometry/laser_geometry.h"
#include <vector>
#include <algorithm>
#include <geometry_msgs/PointStamped.h>
// #include <boost/geometry.hpp>
// #include <boost/geometry/geometries/linestring.hpp>
// #include <boost/geometry/geometries/point_xy.hpp>
// #include <boost/assign.hpp>
#include <cmath>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/message_filter.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"

 ros::Publisher pub;
 tf2_ros::Buffer tf_buffer;

 

 void transformer(const autoware_msgs::DetectedObjectArray& poly)
{
    geometry_msgs::TransformStamped tfstamped;
    geometry_msgs::PointStamped in,out;
    
    ros::Rate sleep(10);
    while(ros::ok())
    {
        try{
            tfstamped = tf_buffer.lookupTransform("map", poly.header.frame_id,ros::Time(0));
        } catch (tf2::TransformException &ex) {
            ROS_WARN("Could NOT transform laser to map: %s", ex.what());
        }

        
        visualization_msgs::MarkerArray ma;
      
        int i,j;

        for ( j =0; j < poly.objects.size(); j++ )

        
        {
        visualization_msgs::Marker bounding;
        bounding.points.clear();
        bounding.id= j;
        bounding.header.frame_id = "map";
        bounding.header.stamp = poly.header.stamp;
        bounding.type = visualization_msgs::Marker::LINE_STRIP;
        bounding.action = visualization_msgs::Marker::ADD;
        bounding.scale.x = 0.2;
        bounding.color.r = 0.1;
        bounding.color.g =  0.4;
        bounding.color.b = 0.9;
        bounding.color.a = 1.0;


        geometry_msgs::Pose pose_transformed;

        tf2::doTransform(poly.objects[j].pose, pose_transformed, tfstamped);

        bounding.pose = pose_transformed;




        
        geometry_msgs::Point po,po2;
        


        
            //autoware_msgs::DetectedObject obj;
            //obj = poly.objects[j];

            
            for (i = 0; i < poly.objects[j].convex_hull.polygon.points.size(); i++)
                
            {
                po2.x = poly.objects[j].convex_hull.polygon.points[i].x;
                po2.y = poly.objects[j].convex_hull.polygon.points[i].y;
                po2.z = poly.objects[j].convex_hull.polygon.points[i].z;

                
                
                tf2::doTransform(po2, po, tfstamped);
                
                
                bounding.points.push_back(po);
                
                
            }            
                
        ma.markers.push_back(bounding);
        
         
        }

        
        pub.publish(ma);
        
        sleep.sleep();
        break;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "boundingbox_transfomer");
    ros::NodeHandle nodeh;

    //tf2_ros::Buffer tf_buffer;
    tf2_ros::TransformListener tf2_list(tf_buffer);
    tf::StampedTransform local_transform;
    geometry_msgs::PoseStamped converted;

    ros::Subscriber sub = nodeh.subscribe("/detection/lidar_detector/objects",1000,transformer);
    
    pub = nodeh.advertise<visualization_msgs::MarkerArray>("converted_euclidean_objects",1);

    ros::spin();
    
    return 0;

}