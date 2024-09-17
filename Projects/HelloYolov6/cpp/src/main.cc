#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yolov6.h"
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
#include <chrono>

#define USE_DMA 1
#define MODEL_INPUT_SIZE 640
int main(int argc, char **argv)
{
    const char *modelPath = argv[1];
    const char *imagePath = argv[2];

    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_post_process();
    object_detect_result_list od_results;


    ret = init_yolov6_model(modelPath, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov5_model fail! ret=%d model_path=%s\n", ret, modelPath);
        return 1;
    }
    
    // If yolo OK
    cv::Mat image = cv::imread(imagePath); // Read from file
    cv::Mat bgr640(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr); // Feed to yolo
    cv::resize(image, bgr640, cv::Size(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE), 0, 0, cv::INTER_LINEAR);
    for (;;){ 
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	    inference_yolov6_model(&rknn_app_ctx, &od_results);
	    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	    double fps = 1 / std::chrono::duration<double>(end - begin).count();
    	    printf("FPS: %lf", fps);
    }

    for (int i = 0; i < od_results.count; i++)
    {
            object_detect_result *det_result = &(od_results.results[i]);
	    printf("%s\n", coco_cls_to_name(det_result->cls_id));
    } 
    return 0;
}
