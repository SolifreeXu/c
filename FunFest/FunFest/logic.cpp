#include <memory.h>	// memset����
#include "logic.h"

int map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };	// ��Ϸɫ�����ȫ�ֶ�ά���飩
static unsigned scoreGroup[] = { 0, 0, 0, 3, 5, 8, 13, 21, 34, 55 };	// ��ȥɫ��ɵõķ�����һ����ȥԽ�࣬�÷�Խ��
unsigned score;	// ��Ϸ�÷�
double leftTime;	// ��Ϸʣ��ʱ��

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

// ������������һ�л�����������һ��ɫ��
int clear()
{
	static int horizontalFlag[MAP_SIZE][MAP_SIZE];	// �������map�к�����Ҫ������ɫ��
	static int verticalFlag[MAP_SIZE][MAP_SIZE];	// ����������
	memset(horizontalFlag, 0, sizeof(horizontalFlag));	// ��transFlag����ȫ������
	memset(verticalFlag, 0, sizeof(verticalFlag));	// ��lengFlag����ȫ������
	int num;	// ����ɫ���ܸ���
	// �Ӿ������Ͻ������½ǿ�ʼ�������ɫ��
	for (int i = 0, j; i < MAP_SIZE; ++i)
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
			}
		}
	}
	num = 0;
	int flag;	// ��¼��ǰ����ɫ��֮ǰɫ��ı����
	// �����Ͻ������½Ǳ���ɫ���Ǿ������ɫ��������÷ֵļ���
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		// �������������ɫ�����������ɫ�������������������ɫ�飨����ֵΪ0��
		flag = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (horizontalFlag[i][j])	// ��ǰɫ���ǲ�Ϊ0������Ҫ����
			{
				if (flag == 0)	// ��ǰɫ��֮ǰ��ɫ������������˵����ǰ�ǵ�һ����ʼ������ɫ��
				{
					num += horizontalFlag[i][j];	// ������ɫ���������ϴ˴�ѭ�������������ɫ����
					increaseScore(horizontalFlag[i][j]);	// ���ӷ���
					increaseTime(horizontalFlag[i][j]);	// ����ʱ��
				}
				map[i][j] = 0;	// ��ɫ������н���ǰɫ������
			}
			flag = horizontalFlag[i][j];	// ��¼��ǰɫ������
		}
		// �������������ɫ�����������ɫ�������������������ɫ��
		flag = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (verticalFlag[j][i])	// ��ǰɫ���ǲ�Ϊ0������Ҫ����
			{
				if (flag == 0)	// ��ǰɫ��֮ǰ��ɫ������������˵����ǰ�ǵ�һ����ʼ������ɫ��
				{
					num += verticalFlag[j][i];	// ������ɫ���������ϴ˴�ѭ�������������ɫ����
					increaseScore(verticalFlag[j][i]);	// ���ӷ���
					increaseTime(verticalFlag[j][i]);	// ����ʱ��
				}
				map[j][i] = 0;	// ��ɫ������н���ǰɫ������
			}
			flag = verticalFlag[j][i];	// ��¼��ǰɫ������
		}
	}
	return num; // һ��ѭ����������Ԫ��
}