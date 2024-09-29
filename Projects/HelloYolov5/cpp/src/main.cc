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
#include <sys/types.h>
#include <dirent.h>
#include <chrono>

#define USE_DMA 1
#define MODEL_INPUT_SIZE 640
#define DATASET_COCO_PATH "/oem/train2017/"

int main(int argc, char **argv)
{
    const char *modelPath = argv[1];
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
    // If yolo OK iterate over dataset (128 images)
    DIR* dirp = opendir(DATASET_COCO_PATH);
    struct dirent * dp;
    unsigned int inferenced = 0;
    double fpsSum;
    double currFps;
    double minFps;
    double maxFps;

    cv::Mat image;
    cv::Mat bgr640(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr); // Feed to yolo

    while ((dp = readdir(dirp)) != NULL) {
	    if (!strcmp(dp->d_name, "..") || !strcmp(dp->d_name, ".")) continue; // Skip ../ path
	    char absFilePath[128] = DATASET_COCO_PATH;
	    strncat(absFilePath, dp->d_name, strlen(dp->d_name));
	    image = cv::imread(absFilePath); // Read from file 
	    cv::resize(image, bgr640, cv::Size(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE), 0, 0, cv::INTER_LINEAR);
	    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	    inference_yolov5_model(&rknn_app_ctx, &od_results);
	    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	    currFps = 1 / std::chrono::duration<double>(end - begin).count();
	    minFps = std::min(currFps, minFps);
	    maxFps = std::max(currFps, maxFps);
	    fpsSum += currFps;
	    inferenced++;
	    printf("Inferenced: %d\n", inferenced);
	    for (int i = 0; i < od_results.count; i++)
	    {	
		    object_detect_result *det_result = &(od_results.results[i]);
		    det_remap(det_result);
		    printf("%d %.3f %Lf %Lf % \n", det_result->cls_id, det_result->prop, );
	    }

    }
    printf("AVG FPS: %Lf\nMax FPS: %Lf\nMin FPS: %Lf\n", fpsSum / inferenced, maxFps, minFps);

    closedir(dirp);
    return 0;
}
