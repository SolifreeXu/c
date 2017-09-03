#include"FunFest.h"
#include<stdio.h>
#include<memory.h>	//memset函数需要

int map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };	//游戏色块矩阵（全局二维数组）
static unsigned scoreGroup[] = { 0, 0, 0, 3, 5, 8, 13, 21, 34, 55 };	//消去色块可得的分数，一次消去越多，得分越多
unsigned score;	//游戏得分
time_t timeLeft;	//游戏剩余时间，time_t是__int64宏定义

/*
函数名：交换数（异或）
参数，两int型操作数的地址
原理：同一个数自相异或为0
优势：无溢出风险，无需临时变量，速度较快
*/
int swap(int *left, int *right)
{
	if (left == NULL || right == NULL)	//如果有一个操作数为空便返回错误代码
		return 1;
	*left ^= *right;
	*right ^= *left;
	*left ^= *right;
	//以上三条语句可以合成*left ^= *right ^= *left ^= *right;
	return 0;
}

//消掉三个以上一行或者三个以上一列色块
int clear()
{
	static int transFlag[MAP_SIZE][MAP_SIZE];	//用来标记map中横向需要消除的色块
	static int lengFlag[MAP_SIZE][MAP_SIZE];	//纵向标记数组
	memset(transFlag, 0, sizeof(transFlag));	//将transFlag数组全部清0
	memset(lengFlag, 0, sizeof(lengFlag));	//将lengFlag数组全部清0
	//从矩阵左上角向右下角开始检测消除色块
	for (int i = 0, j, k; i < MAP_SIZE; ++i)
	for (j = 0; j < MAP_SIZE; ++j)
	{
		if (map[i][j] == 0)	//当前地方为空
			continue;	//跳出本轮循环
		//横着检测可消除的色块
		//为了避免越界问题，给外围加了一圈（最外层的色块矩阵值是0，与所有色块值都不相同，而循环条件是相同，因此保证矩阵不会越界）
		for (k = 1; map[i][j + k] == map[i][j] /*&& j + k <= LOWER_LIMIT*/; ++k);
		if (k >= LOWER_LIMIT)	//k消掉的个数大于等于消除下限
		{
			//标记当前位置的色块为可消除状态
			for (int f = 0; f < k; ++f)
				transFlag[i][j + f] = k;	//将消除色块数赋给色块标记
		}
		//纵向检测可消除的色块
		for (k = 1; map[i + k][j] == map[i][j] /*&& i + k < MAP_SIZE*/; ++k);
		if (k >= LOWER_LIMIT)	//k消掉的个数大于等于消除下限
		{
			//标记需要消掉的色块
			for (int f = 0; f < k; ++f)
				lengFlag[i + f][j] = k;
		}
	}
	int num = 0;	//消掉色块总个数
	int k;	//记录当前遍历色块之前一个色块的标记数
	//从左上角向右下角遍历色块标记矩阵，完成色块个数、得分的计算
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		//横着计算消除的色块个数，并在色块矩阵中消掉可消除的色块（即赋值为0）
		k = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (transFlag[i][j])	//当前色块标记不为0，即需要消除
			{
				if (k == 0)	//当前色块之前的色块无需消除，说明当前色块可能是第一个开始消除的
				{
					num += transFlag[i][j];	//消除的色块总数加上此次循环计算出的消除色块数
					//分数增加机制
					score += scoreGroup[transFlag[i][j]];
					//时间增加机制
					if (timeLeft + transFlag[i][j] > 20)
						timeLeft = 20;
					else
						timeLeft += (transFlag[i][j] - 1);
				}
				map[i][j] = 0;	//在色块矩阵中将当前色块消除
			}
			k = transFlag[i][j];	//记录当前色块标记数
		}
		//横着计算消除的色块个数，并在色块矩阵中消掉可消除的色块
		k = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (lengFlag[j][i])	//当前色块标记不为0，即需要消除
			{
				if (k == 0)	//当前色块之前的色块无需消除，说明当前色块可能是第一个开始消除的
				{
					num += lengFlag[j][i];	//消除的色块总数加上此次循环计算出的消除色块数
					//分数增加机制
					score += scoreGroup[lengFlag[j][i]];
					//时间增加机制
					if (timeLeft + lengFlag[j][i] > 20)
						timeLeft = 20;
					else
						timeLeft += (lengFlag[j][i] - 1);
				}
				map[j][i] = 0;	//在色块矩阵中将当前色块消除
			}
			k = lengFlag[j][i];	//记录当前色块标记数
		}
	}
	return num; //一轮循环中消掉的元素
}