
// Copyright (c) 2023 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*-------------------------------------------
                Includes
-------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yolov8.h"


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("%s <model_path> <image_path>\n", argv[0]);
        return -1;
    }

    const char *model_path = argv[1];
    const char *image_path = argv[2];

    int ret;
    rknn_app_context_t rknn_app_ctx;
    memset(&rknn_app_ctx, 0, sizeof(rknn_app_context_t));

    init_post_process();

    ret = init_yolov8_model(model_path, &rknn_app_ctx);
    if (ret != 0)
    {
        printf("init_yolov8_model fail! ret=%d model_path=%s\n", ret, model_path);
    }

    //ret = read_image(image_path, &src_image);
    cv::Mat img = cv::imread(image_path);
    /*rknn_input inputs[rknn_app_ctx.io_num.n_input];
    void* buf = nullptr;
    buf = (void*) img.data;

    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].size = rknn_app_ctx.model_width * rknn_app_ctx.model_height * rknn_app_ctx.model_channel;
    inputs[0].buf = buf;

    ret = rknn_inputs_set(rknn_app_ctx.rknn_ctx, rknn_app_ctx.io_num.n_input, inputs);
    if(ret < 0){
    	printf("rknn_input_set fail! ret=%d\n", ret);
        return -1;
    }*/

    rknn_output outputs[rknn_app_ctx.io_num.n_output];
    memset(outputs, 0, sizeof(outputs));
    for(uint32_t i = 0; i < rknn_app_ctx.io_num.n_output; i++){
    	outputs[i].index = i;
        outputs[i].want_float = (!rknn_app_ctx.is_quant);
    }


    cv::Mat bgr640(640, 640, CV_8UC3, rknn_app_ctx.input_mems[0]->virt_addr);
    cv::resize(img, bgr640, cv::Size(640, 640), 0, 0, cv::INTER_LINEAR);
    rknn_run(rknn_app_ctx.rknn_ctx, nullptr);
    //app_ctx->output_mems
    object_detect_result_list od_results;
    post_process(&rknn_app_ctx, rknn_app_ctx.output_mems, 0.25, 0.45, &od_results);
    //rknn_outputs_get(rknn_app_ctx.rknn_ctx, rknn_app_ctx.io_num.n_output, outputs, NULL);
    printf("%d\n\n", od_results.count);
    for (int i = 0; i < od_results.count; i++)
    {
	    object_detect_result *det_result = &(od_results.results[i]);
	    int x1 = det_result->box.left;
            int y1 = det_result->box.top;
	    int x2 = det_result->box.right;
	    int y2 = det_result->box.bottom;
	    cv::Rect r = cv::Rect(x1, y1, x2 - x1, y2 - y1); 
	    cv::rectangle(img, r, cv::Scalar(255,0,0), 1, 8, 0);
    }
    cv::imwrite("out.jpg", img);


  
    
    return 0;
}
