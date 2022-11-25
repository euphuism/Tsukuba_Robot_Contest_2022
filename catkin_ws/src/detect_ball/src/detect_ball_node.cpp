#include <iostream>
#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include <tf/transform_datatypes.h>
#include <msgs/BallCDN.h>

class DetectBallNode
{
public:
	ros::Subscriber sub_scan_;
  ros::Publisher pub_cdn_;
	int i;

	double cx = 0.0;//   The x-coordinate of the detected ball.
	double cy = 0.0;//   The y-coordinate of the detected ball.
	double r  = 0.0;//   The radius of the detected ball.
	int sumx  = 0;//     Σx
	int sumy  = 0;//     Σy
	int sumx2 = 0;//     Σx^2
	int sumy2 = 0;//     Σy^2
	int sumxy = 0;//     Σxy
	int sumx3_xy2 = 0;// Σ(x^3 + x * y^2)
	int sumx2y_y3 = 0;// Σ(x^2 * y + y^3)
	int sumx2_y2  = 0;// Σ(x^2 + y^2)
	
	void cbScan(const sensor_msgs::LaserScan::ConstPtr &msg)
	{
		i = msg->ranges.size() / 2;
		if(msg->ranges[i] < msg->range_min ||
			 msg->ranges[i] > msg->range_max ||
			 std::isnan(msg->ranges[i]))
		{
			ROS_INFO("front-range:measurement error");
		}
		else
		{
			ROS_INFO("front-range: %0.3f", msg->ranges[msg->ranges.size() / 2]);
		}

		// https://imagingsolution.blog.fc2.com/blog-entry-16.html
		
		int x[i];
		int y[i];
		for(int j = 0; j < 2 * i; j++)
		{
			x[j] = msg->ranges[2 * j];
			y[j] = msg->ranges[2 * j + 1];
			
			sumx += x[j];
			sumy += y[j];
			sumx2 += x[j] * x[j];
			sumy2 += y[j] * y[j];
			sumxy += x[j] * y[j];
			sumx3_xy2 += x[j] * x[j] * x[j] + x[j] * y[j] *y[j];
			sumx2y_y3 += x[j] * x[j] * y[j] + y[j] * y[j] *y[j];
			sumx2_y2  += x[j] * x[j] + y[j] * y[j];
		}
		/*
		int F[3][3] = {{sumx2, sumxy, sumx}, 
									 {sumxy, sumy2, sumy}, 
									 {sumx, sumy, i}};
		int G[3][1] = {{-sumx3_xy2}, 
									 {-sumx2y_y3}, 
									 {-sumx2_y2}};
		*/
		int detF = sumx2 * sumy2 * i + 2 * sumx * sumxy * sumy - sumx * sumx * sumy2 - sumx2 * sumy * sumy - sumxy * sumxy * i;
		
    // T = F^(-1) * G
		int T[3] = 
			{((sumy2 * i     - sumy  * sumy) * sumx3_xy2 + (sumxy * i    - sumx  * sumy ) * sumx2y_y3 + (sumx  * sumy  - sumxy * sumy ) * sumx2_y2) / detF, 
			 ((sumxy * i     - sumx  * sumy) * sumx3_xy2 + (sumx  * sumx - sumx2 * i    ) * sumx2y_y3 + (sumx2 * sumy  - sumx  * sumxy) * sumx2_y2) / detF, 
			 ((sumx  * sumy2 - sumxy * sumy) * sumx3_xy2 + (sumx2 * sumy - sumx  * sumxy) * sumx2y_y3 + (sumxy * sumxy - sumx2 * sumy2) * sumx2_y2) / detF};
		
		cx = double(T[0] / (-2));
		cy = double(T[1] / (-2));
		r  = std::sqrt(cx * cx + cy * cy - T[2]);
	}

  DetectBallNode()
  {
    ros::NodeHandle nh;
		sub_scan_ = nh.subscribe("scan", 10, &DetectBallNode::cbScan, this);
    pub_cdn_ = nh.advertise<msgs::BallCDN>("ball_cdn", 10);
  }
  void mainloop()
  {
    //ROS_INFO("DetectBallNode");

    ros::Rate rate(10.0);
    while (ros::ok())
    {
      ros::spinOnce();
			
			msgs::BallCDN ball_cdn;
			ball_cdn.cx_ = cx;
			ball_cdn.cy_ = cy;
			ball_cdn.r_  = r;
			
			pub_cdn_.publish(ball_cdn);
	
      rate.sleep();
    }
  }
};

int main(int argc, char* argv[])
{
  ros::init(argc, argv, "detect_ball_node");

  DetectBallNode detect_ball;

  detect_ball.mainloop();
}
