#pragma once

#include <stdio.h>
#include <math.h>	// abs函数
#include <stdbool.h>	// bool类型

#define COLOR_BLOCKS_LOWER_LIMIT 3	// 至少消除的相邻色块数
#define INITIAL_TIME 10	// 游戏开始剩余时间（以秒为单位）
#define TIME_UPPER_LIMIT 20	// 最大剩余时间
#define SCORE_UPPER_LIMIT 999	// 分数上限
#define MAP_SIZE 9	// 色块矩阵大小

bool gameOver();	// 游戏结束条件
void decreaseTime(double time);	// 用于游戏计时减少剩余时间
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

// 点坐标结构体
struct Point
{
	int x;	// x坐标
	int y;	// y坐标
};

// 判断两色块在矩阵中是否相邻
inline bool adjacent(const Point *point1, const Point *point2)
{
	if (point1 == NULL || point2 == NULL)
		return false;
	return abs((point1->x + point1->y) - (point2->x + point2->y)) == 1 
		&& (point1->x == point2->x || point1->y == point2->y);
}