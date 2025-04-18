#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yolov8.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>

#define MODEL_INPUT_SIZE 640
#define CAMERA_DEV 11

cv::Scalar colorMap[4] = {
    cv::Scalar(255, 0, 0),
    cv::Scalar(0, 255, 0),
    cv::Scalar(0, 0, 255),
    cv::Scalar(255, 255, 0)
};

int c_x =  0, c_y = 0;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("%s <model_path>\n", argv[0]);
        return -1;
    }

    const char *model_path = argv[1];

    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    ret = init_yolov8_model(model_path, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov8_model fail! ret=%d model_path=%s\n", ret, model_path);
    }
    cv::Mat bgr640(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr);

    cv::VideoCapture cap;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, MODEL_INPUT_SIZE);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, MODEL_INPUT_SIZE);
    cap.open(CAMERA_DEV);
    cv::Mat camFrame;

    cv::VideoWriter http;
    http.open("httpjpg", 7766);

    while (1)
    {
        cap >> camFrame;
        cv::resize(camFrame, bgr640, cv::Size(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE), 0, 0, cv::INTER_LINEAR);
        rknn_run(rknn_app_ctx.rknn_ctx, nullptr);
        object_detect_result_list od_results;
        post_process(&rknn_app_ctx, rknn_app_ctx.output_mems, 0.25, 0.45, &od_results);

        for (int i = 0; i < od_results.count; i++)
        {
            object_detect_result *det_result = &(od_results.results[i]);
            int x1 = det_result->box.left;
            int y1 = det_result->box.top;
            int x2 = det_result->box.right;
            int y2 = det_result->box.bottom;

            c_x = x1 + (x2 - x1) / 2;
            c_y = y1 + (y2 - y1) / 2;

            cv::Rect r = cv::Rect(x1, y1, x2 - x1, y2 - y1);
            printf("%d\n", det_result->cls_id);
            cv::rectangle(camFrame, r, colorMap[det_result->cls_id], 3, 8, 0);
            cv::circle(camFrame, cv::Point(c_x, c_y), 5, cv::Scalar(0, 0, 255), -1);
        }
        http << camFrame;
    }

    return 0;
}
