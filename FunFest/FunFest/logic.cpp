#include <memory.h>	// memset函数
#include "logic.h"

int map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };	// 游戏色块矩阵（全局二维数组）
unsigned score;	// 游戏得分
double leftTime;	// 游戏剩余时间

static unsigned scoreGroup[] = { 0, 0, 0, 3, 5, 8, 13, 21, 34, 55 };	// 消去色块可得的分数，一次消去越多，得分越多
static int horizontalFlag[MAP_SIZE][MAP_SIZE];	// 用来标记map中横向需要消除的色块
static int verticalFlag[MAP_SIZE][MAP_SIZE];	// 纵向标记数组

// 游戏结束条件
bool gameOver()
{
	return leftTime <= 0 || score >= SCORE_UPPER_LIMIT;
}

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

// 用于游戏计时减少剩余时间
void decreaseTime(double time)
{
	leftTime -= time;
	if (leftTime < 0)
		leftTime = 0;
}

// 更新信息
void update()
{
	int num;	// 记录之前消除色块个数
	// 从左上角向右下角遍历色块标记矩阵，完成色块个数、得分的计算
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		// 横向计算消除的色块个数，并在色块矩阵中消掉可消除的色块（即赋值为0）
		num = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			// 当前色块标记不为0，即需要消除；同时之前色块无需消除，说明当前是第一个开始消除的色块
			if (horizontalFlag[i][j] > 0 && num == 0)
			{
				increaseScore(horizontalFlag[i][j]);	// 增加分数
				increaseTime(horizontalFlag[i][j]);	// 增加时间
			}
			num = horizontalFlag[i][j];	// 记录当前色块标记数
		}
		// 纵向计算消除的色块个数，并在色块矩阵中消掉可消除的色块
		num = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			// 当前色块标记不为0，即需要消除；同时之前色块无需消除，说明当前是第一个开始消除的色块
			if (verticalFlag[j][i] > 0 && num == 0)
			{
				increaseScore(verticalFlag[j][i]);	// 增加分数
				increaseTime(verticalFlag[j][i]);	// 增加时间
			}
			num = verticalFlag[j][i];	// 记录当前色块标记数
		}
	}
}

// 消除色块
void erase()
{
	for (int i = 0; i < MAP_SIZE;++i)
	for (int j = 0; j < MAP_SIZE;++j)
	if (horizontalFlag[i][j] > 0 || verticalFlag[i][j] > 0)
		map[i][j] = 0;	// 在色块矩阵中将当前色块消除
}

// 统计三个以上一行或者三个以上一列色块总数
int count()
{
	memset(horizontalFlag, 0, sizeof(horizontalFlag));	// 将horizontalFlag数组清零
	memset(verticalFlag, 0, sizeof(verticalFlag));	// 将verticalFlag数组清零
	int counter = 0;	// 消除色块计数器
	// 从矩阵左上角向右下角开始检测消除色块
	for (int i = 0, j, num; i < MAP_SIZE; ++i)
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
				counter += num;	// 统计消除色块总数
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
				counter += num;
			}
		}
	}
	return counter; // 消除色块总数
}