#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yolov8.h"
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

    ret = init_yolov8_model(modelPath, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov8_model fail! ret=%d model_path=%s\n", ret, modelPath);
	return 1;
    }

    // If yolo OK
    // cv::Mat image = cv::imread(imagePath); // Read from file
    // cv::Mat bgr640(640, 640, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr); // Feed to yolo
    // cv::resize(image, bgr640, cv::Size(640, 640), 0, 0, cv::INTER_LINEAR);
    //
 
    image_buffer_t src_image;
    memset(&src_image, 0, sizeof(image_buffer_t));
    ret = read_image(imagePath, &src_image);

    #if defined(RV1106_1103)
    	    printf("Using DMA\n");
	    //RV1106 rga requires that input and output bufs are memory allocated by dma
	    ret = dma_buf_alloc(RV1106_CMA_HEAP_PATH, src_image.size, &rknn_app_ctx.img_dma_buf.dma_buf_fd,
                       (void **) & (rknn_app_ctx.img_dma_buf.dma_buf_virt_addr));
	    memcpy(rknn_app_ctx.img_dma_buf.dma_buf_virt_addr, src_image.virt_addr, src_image.size);
	    dma_sync_cpu_to_device(rknn_app_ctx.img_dma_buf.dma_buf_fd);
	    free(src_image.virt_addr);
	    src_image.virt_addr = (unsigned char *)rknn_app_ctx.img_dma_buf.dma_buf_virt_addr;
	    src_image.fd = rknn_app_ctx.img_dma_buf.dma_buf_fd;
	    rknn_app_ctx.img_dma_buf.size = src_image.size;
    #endif


    object_detect_result_list od_results;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    ret = inference_yolov8_model(&rknn_app_ctx, &src_image, &od_results);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double fps = 1 / std::chrono::duration<double>(end - begin).count();
    printf("FPS: %lf", fps);
    printf("Objects found: %d\n", od_results.count);
    for (int i = 0; i < od_results.count; i++)
    {
	    object_detect_result *det_result = &(od_results.results[i]);
	    printf("%s\n", coco_cls_to_name(det_result->cls_id));

    }



    return 0;
}
