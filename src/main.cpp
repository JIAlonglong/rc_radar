#include<iostream>
// ros
#include <ros/ros.h>
#include <math.h>
#include "std_msgs/String.h"
#include <std_msgs/Float64MultiArray.h>
#include <std_msgs/Bool.h>
#include <sensor_msgs/LaserScan.h>
#include<sensor_msgs/PointCloud2.h>

#include "lidar.h"
#include "filter_my.h"


using namespace std;

ros::Publisher ladar_pub;
ros::Publisher whereToshoot;
ros::Publisher whether;

Lidar ladar;
Filter filter;

// std_msgs::Bool yes_or_no;


void LidarCallback(const sensor_msgs::LaserScan::ConstPtr &scan)
{
    //获取雷达数据
    ladar.getData(scan); 
    ladar.init();
    filter.out_range_points(ladar.nowData,ladar.THETA,PI/4);
    //对雷达数据进行滤波
    // 1.去除离群点
    filter.delete_outlier(ladar.nowData,ladar.THETA, 0.15,6);

    //2.滤掉点数过远的点
    filter.dis_longer_filter(ladar.nowData,5);
    
    //3.滤掉点数过近的点
    filter.dis_shorter_filter(ladar.nowData,0.5);

    //4.分离是圆弧的连续段数据
    filter.get_circle(ladar.nowData, ladar.THETA, 0.025);
    
    //5.滤掉点数过少的连续段
    filter.num_less_filter(ladar.nowData,3);
    
    //6.滤掉点数过多的连续段
    filter.num_more_filter(ladar.nowData,300);

    //7.获得连续段
    std::vector<float> start_index; //连续段的起始坐标
    std::vector<float> end_index;   //连续段的终止坐标
    find_continue(ladar.nowData, start_index, end_index);
    //测试用12月4日16.41
    // ROS_INFO("start:%ld",start_index.size());
    // ROS_INFO("end:%ld",end_index.size());
    
    if(start_index.size()!=0)
    {
    //6.拟合圆心
        vector< vector<float> > disR;//(start_index.size(),vector<float>(3))
        output_circle(start_index, end_index, ladar.nowData, disR);
        int num = end_index.size();
        for(int i=0;i<num;i++)
        {
//            ROS_INFO("第%d个圆柱的,x值为:%f,y值为:%f,圆半径为:%f",i+1,disR[i][0],disR[i][1],disR[i][2]);
        }
        
    //7.寻找最好的射环点
        std_msgs::Float64MultiArray the_best_way;
        float best_num=0;
        float best_num_x=0;
        float best_num_y=0;
        float theth = 0;
        
        find_best_data(disR, best_num, best_num_x, best_num_y);  
        theth = atan(best_num_y/best_num_x);
//        ROS_INFO("最好的圆在： x:%f,y:%f,theth:%f",best_num_x, best_num_y,theth);
        the_best_way.data.push_back(best_num_x);
        the_best_way.data.push_back(best_num_y);
        the_best_way.data.push_back(theth);
        whereToshoot.publish(the_best_way);
        the_best_way.data.clear();

    }
    else{
        printf("no circle!!!!\n");
    }
    
    //发布点云，使其可视化
    ladar.prePublish(scan);
    ladar_pub.publish(ladar.result);

}

int main(int argc, char *argv[])
{
    
    setlocale(LC_ALL, "");
    ros::init(argc,argv,"ladar");

    ros::NodeHandle nh;

    ros::Rate loop_rate(5);
    // serialInit();
    ros::Subscriber subscriber = nh.subscribe("/scan01",1 ,LidarCallback);
    while(ros::ok())
    {
        ros::spinOnce();
        //发布雷达当前数据  
        ladar_pub = nh.advertise<sensor_msgs::LaserScan>("/now", 10);
        //发布最好射击位置的数据，第一个数据为x,第二个数据为y，第三个数据是角度
        whereToshoot = nh.advertise<std_msgs::Float64MultiArray>("/radar/post_Position",10);
        // whether =nh.advertise<std_msgs::Bool>("whether",10);
        // ladar_pub = nh.advertise<sensor_msgs::PointCloud2>("/now", 10);
        loop_rate.sleep();
    }    
    ros::spin();
    return 0;
}


