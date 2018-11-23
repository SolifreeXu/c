#include <memory.h>	// memset����
#include "logic.h"

int map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };	// ��Ϸɫ�����ȫ�ֶ�ά���飩
unsigned score;	// ��Ϸ�÷�
double leftTime;	// ��Ϸʣ��ʱ��

static unsigned scoreGroup[] = { 0, 0, 0, 3, 5, 8, 13, 21, 34, 55 };	// ��ȥɫ��ɵõķ�����һ����ȥԽ�࣬�÷�Խ��
static int horizontalFlag[MAP_SIZE][MAP_SIZE];	// �������map�к�����Ҫ������ɫ��
static int verticalFlag[MAP_SIZE][MAP_SIZE];	// ����������

// ��Ϸ��������
bool gameOver()
{
	return leftTime <= 0 || score >= SCORE_UPPER_LIMIT;
}

// �������ӻ���
inline void increaseScore(int nBlocks)
{
	score += scoreGroup[nBlocks];
	if (score > SCORE_UPPER_LIMIT)
		score = SCORE_UPPER_LIMIT;
}

// ʱ�����ӻ���
inline void increaseTime(int nBlocks)
{
	leftTime += (nBlocks - 1);
	if (leftTime - TIME_UPPER_LIMIT > 0)
		leftTime = TIME_UPPER_LIMIT;
}

// ������Ϸ��ʱ����ʣ��ʱ��
void decreaseTime(double time)
{
	leftTime -= time;
	if (leftTime < 0)
		leftTime = 0;
}

// ������Ϣ
void update()
{
	int num;	// ��¼֮ǰ����ɫ�����
	// �����Ͻ������½Ǳ���ɫ���Ǿ������ɫ��������÷ֵļ���
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		// �������������ɫ�����������ɫ�������������������ɫ�飨����ֵΪ0��
		num = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			// ��ǰɫ���ǲ�Ϊ0������Ҫ������ͬʱ֮ǰɫ������������˵����ǰ�ǵ�һ����ʼ������ɫ��
			if (horizontalFlag[i][j] > 0 && num == 0)
			{
				increaseScore(horizontalFlag[i][j]);	// ���ӷ���
				increaseTime(horizontalFlag[i][j]);	// ����ʱ��
			}
			num = horizontalFlag[i][j];	// ��¼��ǰɫ������
		}
		// �������������ɫ�����������ɫ�������������������ɫ��
		num = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			// ��ǰɫ���ǲ�Ϊ0������Ҫ������ͬʱ֮ǰɫ������������˵����ǰ�ǵ�һ����ʼ������ɫ��
			if (verticalFlag[j][i] > 0 && num == 0)
			{
				increaseScore(verticalFlag[j][i]);	// ���ӷ���
				increaseTime(verticalFlag[j][i]);	// ����ʱ��
			}
			num = verticalFlag[j][i];	// ��¼��ǰɫ������
		}
	}
}

// ����ɫ��
void erase()
{
	for (int i = 0; i < MAP_SIZE;++i)
	for (int j = 0; j < MAP_SIZE;++j)
	if (horizontalFlag[i][j] > 0 || verticalFlag[i][j] > 0)
		map[i][j] = 0;	// ��ɫ������н���ǰɫ������
}

// ͳ����������һ�л�����������һ��ɫ������
int count()
{
	memset(horizontalFlag, 0, sizeof(horizontalFlag));	// ��horizontalFlag��������
	memset(verticalFlag, 0, sizeof(verticalFlag));	// ��verticalFlag��������
	int counter = 0;	// ����ɫ�������
	// �Ӿ������Ͻ������½ǿ�ʼ�������ɫ��
	for (int i = 0, j, num; i < MAP_SIZE; ++i)
	for (j = 0; j < MAP_SIZE; ++j)
	{
		if (map[i][j] == 0)	// ��ǰ�ط�Ϊ��
			continue;	// ���Դ���ѭ���������Ĳ���
		// �������������ɫ��
		if (horizontalFlag[i][j] == 0)
		{
			// Ϊ����Խ�����⣬����Χ��һȦ��������ɫ�����ֵΪ0��������ɫ��ֵ������ͬ����ѭ����������ͬ����˱�֤���󲻻�Խ�磩
			for (num = 1; map[i][j + num] == map[i][j] /*&& j + num <= LOWER_LIMIT*/; ++num);
			if (num >= COLOR_BLOCKS_LOWER_LIMIT)	// ��������num���ڵ�����������
			{
				// ��ǵ�ǰλ�õ�ɫ��Ϊ������״̬
				for (int k = 0; k < num; ++k)
					horizontalFlag[i][j + k] = num;	// ������ɫ��������ɫ����
				counter += num;	// ͳ������ɫ������
			}
		}
		// �������������ɫ��
		if (verticalFlag[i][j] == 0)
		{
			for (num = 1; map[i + num][j] == map[i][j] /*&& i + num < MAP_SIZE*/; ++num);
			if (num >= COLOR_BLOCKS_LOWER_LIMIT)	// ��������num���ڵ�����������
			{
				// �����Ҫ������ɫ��
				for (int f = 0; f < num; ++f)
					verticalFlag[i + f][j] = num;
				counter += num;
			}
		}
	}
	return counter; // ����ɫ������
}