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

            double dist =  47.544410243201867 - 0.84427851222848216 * (radius / 1.6) + 0.0056988650152944471* pow(radius / 1.6, 2) - 0.000012907907394359109* pow(radius / 1.6, 3);

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