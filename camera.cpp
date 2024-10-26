#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <cmath>
//Objeknya adalah Tutup Botol :)
//Aslam Pandu Tasminto

using namespace cv;
using namespace std;

int main()
{
    VideoCapture cap(1);
    
    if (!cap.isOpened()) {
        return -1;
    }

    Mat vid_cam;
    Mat vid_cam_hsv;
    Mat threshold_hsv;

    while (true) {
        cap >> vid_cam;
    
        cvtColor(vid_cam, vid_cam_hsv, COLOR_BGR2HSV);
        inRange(vid_cam_hsv, Scalar(170, 130, 50), Scalar(180, 255, 255), threshold_hsv);

        vector<vector<Point>> contours;
        findContours(threshold_hsv, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        float radius;
        Point2f center;
        double maxArea = 0;

        for (size_t i = 0; i < contours.size(); ++i) {
            double area = contourArea(contours[i]);

            if (area > maxArea) {
                maxArea = area;
                minEnclosingCircle(contours[i], center, radius);
            } else if ((int)area == (int)maxArea) {
                continue;
            }
        }

        if (maxArea > 100 && maxArea < 308000) {
            circle(vid_cam, center, (int)radius, Scalar(255, 0, 0), 2);
            
            double dist = 159.59665197690032 - 6.1134615435764985 * (radius) + 0.095445319642689888 * pow(radius, 2) - 0.00071963853187203511 * pow(radius, 3) + 0.0000027869812247284325 * pow(radius, 4) - 0.0000000053337689624525208 * pow(radius, 5) + 0.0000000000039905365509557071 * pow(radius, 6);
            ostringstream textStream;
            textStream << "Object Distance: " << dist << " cm";
            string text = textStream.str();

            putText(vid_cam, text, Point(50, 50), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 1.5);
        }
        
        imshow("camera", vid_cam);

        if (waitKey(1) == 'q') {
            break;
        }
    }

    return 0;
}