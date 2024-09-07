#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <chrono>

// OpenCV Mobile doesn't support VideoWriter

#define DEVICE_PATH 11
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
	cap.open(DEVICE_PATH);

	cv::Mat bgr;

	// "Warmup" camera
	for (int i = 0; i < 5; i++){cap >> bgr;}

	for (int i = 0; i < 25 * 10; i++){
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		cap >> bgr;
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		double fps = 1 / std::chrono::duration<double>(end - begin).count();
		avgFps += fps;
		framesRead++;
		if (bgr.empty()) break;
	}
	printf("AVG FPS: %lf\n", avgFps / framesRead);

	cap.release();

	return 0;
}
