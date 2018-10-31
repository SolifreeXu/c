#ifndef _LOGIC_H
#define _LOGIC_H

#include <stdio.h>
#include <math.h>	// abs函数

#define WINDOW_WIDTH 625	// 窗口宽度
#define WINDOW_HEIGHT 500	// 窗口高度
#define COLOR_BLOCKS_NUMBER 6	// 色块种数
#define COLOR_BLOCK_PIXELS	50	// 色块像素大小
#define COLOR_BLOCKS_LOWER_LIMIT 3	// 至少消除的相邻色块数
#define INITIAL_TIME 10	// 游戏开始剩余时间（以秒为单位）
#define TIME_UPPER_LIMIT 20	// 最大剩余时间
#define SCORE_UPPER_LIMIT 999	// 分数上限
#define MAP_X 150	// 色块矩阵左上角X坐标
#define MAP_Y 25	// 色块矩阵左上角Y坐标
#define MAP_SIZE 9	// 色块矩阵大小
#define MAP_PIXELS (MAP_SIZE*COLOR_BLOCK_PIXELS)	// 色块矩阵像素大小
#define LEAVE_X 25	// 退出通道左上角X坐标
#define LEAVE_Y 360	// 退出通道左上角Y坐标
#define LEAVE_PIXELS 100	// 退出通道像素大小
#define ESC 27	// 退出游戏键ASCII码

extern int map[MAP_SIZE + 1][MAP_SIZE + 1];	// 游戏色块矩阵
extern unsigned score;	// 游戏得分
extern double leftTime;	// 游戏剩余时间

int count();	// 统计消除色块总数
void erase();	// 消除色块
void update();	// 更新信息

/*
*	函数名：交换数（异或）
*	参数，两整型操作数的地址
*	原理：同一个数自相异或为0
*	优势：无溢出风险，无需临时变量，效率高
*	缺陷：消耗CPU
*/
inline void swap(int *left, int *right)
{
	if (left == NULL || right == NULL)	// 如果有一个操作数地址为空，直接返回
		return;
	// 以下三条语句可以合成*left ^= *right ^= *left ^= *right;
	*left ^= *right;
	*right ^= *left;
	*left ^= *right;
}

// 两色块在矩阵中是否相邻
inline int adjacent(int x1, int y1, int x2, int y2)
{
	return abs((x1 + y1) - (x2 + y2)) == 1 && (x1 == x2 || y1 == y2);
}

// 用于游戏计时减少剩余时间
inline void decreaseTime(double time)
{
	leftTime -= time;
	if (leftTime < 0)
		leftTime = 0;
}

// 游戏结束条件
inline int gameOver()
{
	return leftTime <= 0 || score >= SCORE_UPPER_LIMIT;
}

#endif