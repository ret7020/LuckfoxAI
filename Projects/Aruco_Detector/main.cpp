#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <chrono>
#include <opencv2/objdetect/aruco_detector.hpp>
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
	cv::aruco::DetectorParameters detectorParams = cv::aruco::DetectorParameters();
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	cv::aruco::ArucoDetector detector(dictionary, detectorParams);

	std::vector<int> markerIds;
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;

	// "Warmup" camera
	for (int i = 0; i < 5; i++){cap >> bgr;}
	
	while (1){
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		detector.detectMarkers(bgr, markerCorners, markerIds, rejectedCandidates);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		printf("Det FPS: %lf\n", 1 / std::chrono::duration<double>(end - begin).count());

		printf("Count: %d\n", markerIds.size());
		cap >> bgr;
		http << bgr;
	}

	cap.release();

	return 0;
}
