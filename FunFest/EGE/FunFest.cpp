#include"FunFest.h"
#include<stdio.h>
#include<memory.h>	//memset������Ҫ

int map[MAP_SIZE + 1][MAP_SIZE + 1] = { 0 };	//��Ϸɫ�����ȫ�ֶ�ά���飩
static unsigned scoreGroup[] = { 0, 0, 0, 3, 5, 8, 13, 21, 34, 55 };	//��ȥɫ��ɵõķ�����һ����ȥԽ�࣬�÷�Խ��
unsigned score;	//��Ϸ�÷�
time_t timeLeft;	//��Ϸʣ��ʱ�䣬time_t��__int64�궨��

/*
�������������������
��������int�Ͳ������ĵ�ַ
ԭ��ͬһ�����������Ϊ0
���ƣ���������գ�������ʱ�������ٶȽϿ�
*/
int swap(int *left, int *right)
{
	if (left == NULL || right == NULL)	//�����һ��������Ϊ�ձ㷵�ش������
		return 1;
	*left ^= *right;
	*right ^= *left;
	*left ^= *right;
	//�������������Ժϳ�*left ^= *right ^= *left ^= *right;
	return 0;
}

//������������һ�л�����������һ��ɫ��
int clear()
{
	static int transFlag[MAP_SIZE][MAP_SIZE];	//�������map�к�����Ҫ������ɫ��
	static int lengFlag[MAP_SIZE][MAP_SIZE];	//����������
	memset(transFlag, 0, sizeof(transFlag));	//��transFlag����ȫ����0
	memset(lengFlag, 0, sizeof(lengFlag));	//��lengFlag����ȫ����0
	//�Ӿ������Ͻ������½ǿ�ʼ�������ɫ��
	for (int i = 0, j, k; i < MAP_SIZE; ++i)
	for (j = 0; j < MAP_SIZE; ++j)
	{
		if (map[i][j] == 0)	//��ǰ�ط�Ϊ��
			continue;	//��������ѭ��
		//���ż���������ɫ��
		//Ϊ�˱���Խ�����⣬����Χ����һȦ��������ɫ�����ֵ��0��������ɫ��ֵ������ͬ����ѭ����������ͬ����˱�֤���󲻻�Խ�磩
		for (k = 1; map[i][j + k] == map[i][j] /*&& j + k <= LOWER_LIMIT*/; ++k);
		if (k >= LOWER_LIMIT)	//k�����ĸ������ڵ�����������
		{
			//��ǵ�ǰλ�õ�ɫ��Ϊ������״̬
			for (int f = 0; f < k; ++f)
				transFlag[i][j + f] = k;	//������ɫ��������ɫ����
		}
		//�������������ɫ��
		for (k = 1; map[i + k][j] == map[i][j] /*&& i + k < MAP_SIZE*/; ++k);
		if (k >= LOWER_LIMIT)	//k�����ĸ������ڵ�����������
		{
			//�����Ҫ������ɫ��
			for (int f = 0; f < k; ++f)
				lengFlag[i + f][j] = k;
		}
	}
	int num = 0;	//����ɫ���ܸ���
	int k;	//��¼��ǰ����ɫ��֮ǰһ��ɫ��ı����
	//�����Ͻ������½Ǳ���ɫ���Ǿ������ɫ��������÷ֵļ���
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		//���ż���������ɫ�����������ɫ�������������������ɫ�飨����ֵΪ0��
		k = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (transFlag[i][j])	//��ǰɫ���ǲ�Ϊ0������Ҫ����
			{
				if (k == 0)	//��ǰɫ��֮ǰ��ɫ������������˵����ǰɫ������ǵ�һ����ʼ������
				{
					num += transFlag[i][j];	//������ɫ���������ϴ˴�ѭ�������������ɫ����
					//�������ӻ���
					score += scoreGroup[transFlag[i][j]];
					//ʱ�����ӻ���
					if (timeLeft + transFlag[i][j] > 20)
						timeLeft = 20;
					else
						timeLeft += (transFlag[i][j] - 1);
				}
				map[i][j] = 0;	//��ɫ������н���ǰɫ������
			}
			k = transFlag[i][j];	//��¼��ǰɫ������
		}
		//���ż���������ɫ�����������ɫ�������������������ɫ��
		k = 0;
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			if (lengFlag[j][i])	//��ǰɫ���ǲ�Ϊ0������Ҫ����
			{
				if (k == 0)	//��ǰɫ��֮ǰ��ɫ������������˵����ǰɫ������ǵ�һ����ʼ������
				{
					num += lengFlag[j][i];	//������ɫ���������ϴ˴�ѭ�������������ɫ����
					//�������ӻ���
					score += scoreGroup[lengFlag[j][i]];
					//ʱ�����ӻ���
					if (timeLeft + lengFlag[j][i] > 20)
						timeLeft = 20;
					else
						timeLeft += (lengFlag[j][i] - 1);
				}
				map[j][i] = 0;	//��ɫ������н���ǰɫ������
			}
			k = lengFlag[j][i];	//��¼��ǰɫ������
		}
	}
	return num; //һ��ѭ����������Ԫ��
}