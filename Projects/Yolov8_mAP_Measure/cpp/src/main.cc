#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "yolov8.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <sys/types.h>
#include <dirent.h>

#define MODEL_INPUT_SIZE 640


int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("%s <model_path> <val_path_dir> <results_path>\n", argv[0]);
        return -1;
    }

    const char *model_path = argv[1];
    const char *val_path = argv[2];
    const char *results_path = argv[3];

    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_post_process();

    ret = init_yolov8_model(model_path, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov8_model fail! ret=%d model_path=%s\n", ret, model_path);
    }

    cv::Mat bgr640(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr);
    DIR* dirp = opendir(val_path);
    struct dirent * dp;
    FILE *resWriteptr;

    while ((dp = readdir(dirp)) != NULL) {
	if (!strcmp(dp->d_name, "..") || !strcmp(dp->d_name, ".")) continue; // Skip ../ path

	char absFilePath[128];
	sprintf(absFilePath, "%s/%s", val_path, dp->d_name);
	cv::Mat img = cv::imread(absFilePath);
        cv::resize(img, bgr640, cv::Size(MODEL_INPUT_SIZE, MODEL_INPUT_SIZE), 0, 0, cv::INTER_LINEAR);
        rknn_run(rknn_app_ctx.rknn_ctx, nullptr);
        object_detect_result_list od_results;

        post_process(&rknn_app_ctx, rknn_app_ctx.output_mems, 0.25, 0.45, &od_results);
        printf("%d\n\n", od_results.count);
        for (int i = 0; i < od_results.count; i++)
        {
            object_detect_result *det_result = &(od_results.results[i]);
            int x1 = det_result->box.left;
            int y1 = det_result->box.top;
            int x2 = det_result->box.right;
            int y2 = det_result->box.bottom;
	    printf("IMG: %s -> x1=%d1 y1=%d x2=%d y2=%d class: %d\n", dp->d_name, x1, y1, x2, y2, det_result->cls_id);
	    char resFilePath[256];
	    char fileName[128];
	    strcpy(fileName, dp->d_name);
	    //char fileName[128];
	    fileName[strlen(fileName) - 4] = 0;
	    sprintf(resFilePath, "%s/%s.txt", results_path, fileName);
	    resWriteptr = fopen(resFilePath, "w");
	    fprintf(resWriteptr, "%s %lf %d %d %d %d\n", coco_cls_to_name(det_result->cls_id), det_result->prop, x1, y1, x2, y2);


	    fclose(resWriteptr);
        }
	printf("----------\n");
    }

    return 0;
}
