#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <chrono>

// OpenCV Mobile doesn't support VideoWriter

#define DEVICE_PATH 11
#define VIDEO_RECORD_FRAME_WIDTH 640
#define VIDEO_RECORD_FRAME_HEIGHT 640


int main()
{
	// Camera init
	cv::VideoCapture cap;
	cap.set(cv::CAP_PROP_FRAME_WIDTH, VIDEO_RECORD_FRAME_WIDTH);
	cap.set(cv::CAP_PROP_FRAME_HEIGHT, VIDEO_RECORD_FRAME_HEIGHT);
	cap.open(DEVICE_PATH);

	cv::Mat bgr;
	char classId[30];
	char dummy[2];
	scanf("%s", &classId);
	printf("Gathering data for class: %s\n", classId);

	// "Warmup" camera
	for (int i = 0; i < 5; i++){cap >> bgr;}

	for (int i = 0; i < 100; i++){
		cap >> bgr;
		char fileName[100];
		sprintf(fileName, "./%s/%d.jpg", classId, i);
		
		cv::imwrite(fileName, bgr);
		fgets(dummy, sizeof(dummy), stdin);
		printf("OK - %d\n", i);

		if (bgr.empty()) break;
	}

	cap.release();

	return 0;
}
