#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
//Aslam Pandu Tasminto

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap ("/home/apt2736/Downloads/Video asli.avi");

    Mat frame;
    Mat frame_hsv;
    Mat threshold_hsv;

    int firstMove = 0;
    double prevX = 0, prevY = 0;
    double posX = 0, posY = 0;
    double v, vx, vy;

    while(true) {
        while(true) {
            cap >> frame;

            cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
            inRange(frame_hsv, Scalar(7, 131, 120), Scalar(20, 255, 255), threshold_hsv);

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

            if (maxArea > 100 && maxArea < 2300) {
                circle(frame, center, (int)radius, Scalar(255, 0, 0), 2);
                circle(frame, center, 1, Scalar(0, 0, 0), 2);
                circle(frame, Point(630, 336), 1, Scalar(0, 0, 0), 2);

                if (firstMove <= 15) {
                    prevX = center.x - 630;
                    prevY = 336 - center.y;
                    ++firstMove;
                    break;
                }

                posX += prevX - (center.x - 630);
                vx = prevX - (center.x - 630);

                if ((336 - center.y) < 0) {
                    if (prevY < 0) {
                        posY += prevY - (336 - center.y);
                        vy = prevY - (336 - center.y);
                    } else {
                        posY += (336 - center.y) - prevY;
                        vy = (336 - center.y) - prevY;
                    }
                } else {
                    if (prevY < 0) {
                        posY += (336 - center.y) - prevY;
                        vy = (336 - center.y) - prevY;
                    } else {
                        posY += prevY - (336 - center.y);
                        vy = prevY - (336 - center.y);
                    }
                }

                v = (double)(sqrt(pow(vx * 10, 2) + pow(vy * 10, 2)) / 30);
                prevX = center.x - 630;
                prevY = 336 - center.y;

                line(frame, Point(630, 336), center, Scalar(0, 0, 0), 1.5);

                ostringstream textStream, textStream2, textStream3, textStream4;
                textStream << "Robot Position relative to start: [" << posX * 10 << " cm][" << posY * 10 << " cm]";
                string text = textStream.str();
                textStream2 << "Ball Position relative to robot: [" << (center.x - 630) * 10 << " cm][" << (336 - center.y) * 10 << " cm]";
                string text2 = textStream2.str();
                textStream3 << "[3195 cm, 1925 cm]";
                string text3 = textStream3.str();
                textStream4 << "Robot Velocity: " << v << " cm/s";
                string text4 = textStream4.str();

                putText(frame, text, Point(50, 50), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1.7);
                putText(frame, text2, Point(50, 80), FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255), 1.7);
                putText(frame, text3, center, FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 255, 255), 1.5);
                putText(frame, text4, Point(50, 110), FONT_HERSHEY_COMPLEX, 0.5, Scalar(255, 0, 255), 1.7);
            }

            imshow("Video", frame);

            if (waitKey(30) == 'q') {
                break;
            }
        }
    }
}
