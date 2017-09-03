#ifndef _FUNFEST_H
#define _FUNFEST_H

#include<time.h>	//time_t类型需要

#define WINDOW_WIDTH 625	//窗口宽度
#define WINDOW_HEIGHT 500	//窗口高度
#define MAP_X 150	//色块矩阵开始X坐标
#define MAP_Y 25	//色块矩阵开始Y坐标
#define COLOR_BLOCKS_NUMBER 6	//色块种数
#define COLOR_BLOCK_PIXELS	50	//色块像素大小
#define MAP_SIZE 9	//色块矩阵大小
#define LOWER_LIMIT 3	//能够消除的至少相邻色块数
#define INITIAL_TIME 10	//游戏开始剩余时间（以秒为单位）
#define ESC 27	//退出游戏键ASCII码

extern int map[MAP_SIZE + 1][MAP_SIZE + 1];	//游戏色块矩阵
extern unsigned score;	//游戏得分
extern time_t timeLeft;	//游戏剩余时间，time_t是__int64宏定义

int swap(int*, int*);	//交换数
int clear();	//消除色块

#endif