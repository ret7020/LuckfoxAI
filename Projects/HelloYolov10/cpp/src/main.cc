#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yolov10.h"
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

#include <chrono>

#include "dma_alloc.cpp"

#define USE_DMA 1

int main(int argc, char **argv)
{
    const char *modelPath = argv[1];
    const char *imagePath = argv[2];
    
    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_post_process();

    ret = init_yolov10_model(modelPath, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov10_model fail! ret=%d model_path=%s\n", ret, modelPath);
        return 1;
    }

    
    
    return 0;
}
