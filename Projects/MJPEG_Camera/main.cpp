#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <chrono>

// OpenCV Mobile doesn't support VideoWriter

#define DEVICE_PATH 0
#define VIDEO_RECORD_FRAME_WIDTH 640
#define VIDEO_RECORD_FRAME_HEIGHT 640

double avgFps = 0.0;
int framesRead = 0;
int main()
{
	// Camera init
	cv::VideoCapture cap;
	cap.set(cv::CAP_PROP_FRAME_WIDTH, VIDEO_RECORD_FRAME_WIDTH);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, VIDEO_RECORD_FRAME_HEIGHT);
	cap.open(0);
	cv::VideoWriter http;
	http.open("httpjpg", 7766);

	cv::Mat bgr;

	// "Warmup" camera
	for (int i = 0; i < 5; i++){cap >> bgr;}

	while (1){
		cap >> bgr;
		http << bgr;
	}

	cap.release();

	return 0;
}
