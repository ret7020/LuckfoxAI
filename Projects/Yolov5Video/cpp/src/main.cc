#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yolov5.h"
#include "image_utils.h"
#include "file_utils.h"
#include "image_drawing.h"

#include <unistd.h>   
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <time.h>

//opencv
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "dma_alloc.cpp"

#define USE_DMA 1
#define MODEL_INPUT_SIZE 640
#define CAMERA_DEVICE_PATH 11

int main(int argc, char **argv)
{
    const char *modelPath = argv[1];
    const char *imagePath = argv[2];

    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_post_process();
    object_detect_result_list od_results;


    ret = init_yolov5_model(modelPath, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov5_model fail! ret=%d model_path=%s\n", ret, modelPath);
        return 1;
    }
    // If yolo OK
    cv::VideoCapture cap;
    // OpenCV can't read square images
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.open(CAMERA_DEVICE_PATH);

    cv::Mat camFrame;
    while (1){
	    cap >> camFrame;
	    cv::resize(camFrame, camFrame, cv::Size(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE), 0, 0, cv::INTER_LINEAR);
	    printf("%dx%d\n", camFrame.size().width, camFrame.size().height);
	    cv::Mat bgr640(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr); // Feed to yolo
	    cv::resize(camFrame, bgr640, cv::Size(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE), 0, 0, cv::INTER_LINEAR);
	    inference_yolov5_model(&rknn_app_ctx, &od_results);
	    for (int i = 0; i < od_results.count; i++)
	    {
            	object_detect_result *det_result = &(od_results.results[i]);
		det_remap(det_result);
	   	printf("%s ", coco_cls_to_name(det_result->cls_id));
    	   }
	   printf("\n");
    }
    return 0;
}
