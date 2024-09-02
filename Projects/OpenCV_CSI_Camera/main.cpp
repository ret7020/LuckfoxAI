#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include "opencv2/opencv.hpp"
#include <opencv2/videoio.hpp>
#include <stdio.h>

// This is a video writer - record video from csi camera

#define DEVICE_PATH 11
#define VIDEO_RECORD_FRAME_WIDTH 640
#define VIDEO_RECORD_FRAME_HEIGHT 480

int main()
{
	// Camera init
	cv::VideoCapture cap;
	cap.set(cv::CAP_PROP_FRAME_WIDTH, VIDEO_RECORD_FRAME_WIDTH);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, VIDEO_RECORD_FRAME_HEIGHT);
	cap.open(DEVICE_PATH);

	// Writer init
	cv::VideoWriter video;
	//("record.avi", CV_FOURCC('M','J','P','G'), 25, Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));


	cv::Mat bgr;

	// "Warmup" camera
	for (int i = 0; i < 5; i++){cap >> bgr;}

	for (int i = 0; i < 25 * 10; i++){
		cap >> bgr;
		if (bgr.empty()) break;
		video.write(bgr);
	}

	cap.release();
	video.release();
	

	return 0;
}
