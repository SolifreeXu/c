#pragma once

#include <graphics.h>	// EGE的PIMAGE类型
#include "logic.h"

#define WINDOW_WIDTH 625	// 窗口宽度
#define WINDOW_HEIGHT 500	// 窗口高度
#define COLOR_BLOCKS_NUMBER 6	// 色块种数
#define COLOR_BLOCK_PIXELS 50	// 色块像素大小
#define MAP_X 150	// 色块矩阵左上角X坐标
#define MAP_Y 25	// 色块矩阵左上角Y坐标
#define MAP_PIXELS (MAP_SIZE*COLOR_BLOCK_PIXELS)	// 色块矩阵像素大小
#define LEAVE_X 25	// 退出通道左上角X坐标
#define LEAVE_Y 360	// 退出通道左上角Y坐标
#define LEAVE_PIXELS 100	// 退出通道像素大小
#define ESC 27	// 退出游戏键ASCII码

void initData();	// 初始游戏数据
void initEnviron();	// 初始游戏环境
void closeEnviron();	// 关闭游戏环境
bool end();	// 游戏结局界面
void render(bool onlyInfo);	// 渲染游戏界面
void fill();	// 填充色块
int play();	// 玩家控制模块