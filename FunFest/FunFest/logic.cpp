#include <memory.h>	// memset函数
#include "logic.h"

int map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };	// 游戏色块矩阵（全局二维数组）
static unsigned scoreGroup[] = { 0, 0, 0, 3, 5, 8, 13, 21, 34, 55 };	// 消去色块可得的分数，一次消去越多，得分越多
unsigned score;	// 游戏得分
double leftTime;	// 游戏剩余时间

// 分数增加机制
inline void increaseScore(int nBlocks)
{
	score += scoreGroup[nBlocks];
	if (score > SCORE_UPPER_LIMIT)
		score = SCORE_UPPER_LIMIT;
}

// 时间增加机制
inline void increaseTime(int nBlocks)
{
	leftTime += (nBlocks - 1);
	if (leftTime - TIME_UPPER_LIMIT > 0)
		leftTime = TIME_UPPER_LIMIT;
}

// 消掉三个以上一行或者三个以上一列色块
int clear()
{
	static int horizontalFlag[MAP_SIZE][MAP_SIZE];	// 用来标记map中横向需要消除的色块
	static int verticalFlag[MAP_SIZE][MAP_SIZE];	// 纵向标记数组
	memset(horizontalFlag, 0, sizeof(horizontalFlag));	// 将transFlag数组全部清零
	memset(verticalFlag, 0, sizeof(verticalFlag));	// 将lengFlag数组全部清零
	int num;	// 消掉色块总个数
	// 从矩阵左上角向右下角开始检测消除色块
	for (int i = 0, j; i < MAP_SIZE; ++i)
	for (j = 0; j < MAP_SIZE; ++j)
	{
		if (map[i][j] == 0)	// 当前地方为空
			continue;	// 忽略此轮循环接下来的步骤
		// 横向检测可消除的色块
		if (horizontalFlag[i][j] == 0)
		{
			// 为避免越界问题，给外围加一圈（最外层的色块矩阵值为0，与所有色块值都不相同，而循环条件是相同，因此保证矩阵不会越界）
			for (num = 1; map[i][j + num] == map[i][j] /*&& j + num <= LOWER_LIMIT*/; ++num);
			if (num >= COLOR_BLOCKS_LOWER_LIMIT)	// 消掉个数num大于等于消除下限
			{
				// 标记当前位置的色块为可消除状态
				for (int k = 0; k < num; ++k)
					horizontalFlag[i][j + k] = num;	// 将消除色块数赋给色块标记
			}
		}
		// 纵向检测可消除的色块
		if (verticalFlag[i][j] == 0)
		{
			for (num = 1; map[i + num][j] == map[i][j] /*&& i + num < MAP_SIZE*/; ++num);
			if (num >= COLOR_BLOCKS_LOWER_LIMIT)	// 消掉个数num大于等于消除下限
			{
				// 标记需要消掉的色块
				for (int f = 0; f < num; ++f)
					verticalFlag[i + f][j] = num;
			}
		}
	}
	num = 0;
	int flag;	// 记录当前遍历色块之前色块的标记数
	// 从左上角向右下角遍历色块标记矩阵，完成色块个数、得分的计算
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		// 横向计算消除的色块个数，并在色块矩阵中消掉可消除的色块（即赋值为0）
		flag = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (horizontalFlag[i][j])	// 当前色块标记不为0，即需要消除
			{
				if (flag == 0)	// 当前色块之前的色块无需消除，说明当前是第一个开始消除的色块
				{
					num += horizontalFlag[i][j];	// 消除的色块总数加上此次循环计算出的消除色块数
					increaseScore(horizontalFlag[i][j]);	// 增加分数
					increaseTime(horizontalFlag[i][j]);	// 增加时间
				}
				map[i][j] = 0;	// 在色块矩阵中将当前色块消除
			}
			flag = horizontalFlag[i][j];	// 记录当前色块标记数
		}
		// 纵向计算消除的色块个数，并在色块矩阵中消掉可消除的色块
		flag = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (verticalFlag[j][i])	// 当前色块标记不为0，即需要消除
			{
				if (flag == 0)	// 当前色块之前的色块无需消除，说明当前是第一个开始消除的色块
				{
					num += verticalFlag[j][i];	// 消除的色块总数加上此次循环计算出的消除色块数
					increaseScore(verticalFlag[j][i]);	// 增加分数
					increaseTime(verticalFlag[j][i]);	// 增加时间
				}
				map[j][i] = 0;	// 在色块矩阵中将当前色块消除
			}
			flag = verticalFlag[j][i];	// 记录当前色块标记数
		}
	}
	return num; // 一轮循环中消掉的元素
}