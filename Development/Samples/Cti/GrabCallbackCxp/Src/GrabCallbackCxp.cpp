// GrabCallbackCxp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "gentl_capture.h"
#include "PFNC.h"
#include <stdio.h>
#include <stdint.h>

#define CACHESNUMBER 16 //缓存个数
#define MAXPICTURES 10 //采图个数

uint32_t g_test_get_data_num = 0;
//回调演示
static void callback_get_data(void* p_cb_contex, void* p_base, uint32_t size)
{
	if (p_cb_contex == NULL)
		return;

	CGentlCap* t_p_gentl = (CGentlCap*)p_cb_contex;	
	if (p_base)//获取数据
	{
		g_test_get_data_num++;
		printf("get frame:%d, width:%d, height:%d\n", g_test_get_data_num, t_p_gentl->getWidth(), t_p_gentl->getHeight());
	}
}

int main(int argc, char* argv[])
{
	bool8_t b_res = false;
	//初始化采集卡
	CGentlCap gentl_cap;

	do
	{
		b_res = gentl_cap.init_sdk_ex();
		if (!b_res)
		{
			printf("init sdk error!\n");
			break;
		}

		// 连接设备。
		b_res = gentl_cap.connect_gentl();
		if (!b_res)
		{
			printf("connect gentl error!\n");
			break;
		}

		// 设置为自由拉流
		b_res = gentl_cap.set_camera_params();
		if (!b_res)
		{
			printf("set camera params error!\n");
			break;
		}

		// 开启gentl 标准采集
		// 创建CACHESNUMBER个缓存， 采图MAXPICTURES张结束, 设为-1，代表一直采集
		b_res = gentl_cap.start_gentl_capture(callback_get_data, &gentl_cap, CACHESNUMBER, MAXPICTURES);
		if (!b_res)
		{
			printf("start gentl capture error!\n");
			break;
		}

		// 停止采集
		b_res =  gentl_cap.capture_wait(INFINITE);
		if (!b_res)
		{
			printf("capture wait error!\n");
			break;
		}

	} while (false);

	printf("Press enter key to exit...\n");
	getchar();
}




