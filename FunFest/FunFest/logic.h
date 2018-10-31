#ifndef _LOGIC_H
#define _LOGIC_H

#include <stdio.h>
#include <math.h>	// abs����

#define WINDOW_WIDTH 625	// ���ڿ��
#define WINDOW_HEIGHT 500	// ���ڸ߶�
#define COLOR_BLOCKS_NUMBER 6	// ɫ������
#define COLOR_BLOCK_PIXELS	50	// ɫ�����ش�С
#define COLOR_BLOCKS_LOWER_LIMIT 3	// ��������������ɫ����
#define INITIAL_TIME 10	// ��Ϸ��ʼʣ��ʱ�䣨����Ϊ��λ��
#define TIME_UPPER_LIMIT 20	// ���ʣ��ʱ��
#define SCORE_UPPER_LIMIT 999	// ��������
#define MAP_X 150	// ɫ��������Ͻ�X����
#define MAP_Y 25	// ɫ��������Ͻ�Y����
#define MAP_SIZE 9	// ɫ������С
#define MAP_PIXELS (MAP_SIZE*COLOR_BLOCK_PIXELS)	// ɫ��������ش�С
#define LEAVE_X 25	// �˳�ͨ�����Ͻ�X����
#define LEAVE_Y 360	// �˳�ͨ�����Ͻ�Y����
#define LEAVE_PIXELS 100	// �˳�ͨ�����ش�С
#define ESC 27	// �˳���Ϸ��ASCII��

extern int map[MAP_SIZE + 1][MAP_SIZE + 1];	// ��Ϸɫ�����
extern unsigned score;	// ��Ϸ�÷�
extern double leftTime;	// ��Ϸʣ��ʱ��

int count();	// ͳ������ɫ������
void erase();	// ����ɫ��
void update();	// ������Ϣ

/*
*	�������������������
*	�����������Ͳ������ĵ�ַ
*	ԭ��ͬһ�����������Ϊ0
*	���ƣ���������գ�������ʱ������Ч�ʸ�
*	ȱ�ݣ�����CPU
*/
inline void swap(int *left, int *right)
{
	if (left == NULL || right == NULL)	// �����һ����������ַΪ�գ�ֱ�ӷ���
		return;
	// �������������Ժϳ�*left ^= *right ^= *left ^= *right;
	*left ^= *right;
	*right ^= *left;
	*left ^= *right;
}

// ��ɫ���ھ������Ƿ�����
inline int adjacent(int x1, int y1, int x2, int y2)
{
	return abs((x1 + y1) - (x2 + y2)) == 1 && (x1 == x2 || y1 == y2);
}

// ������Ϸ��ʱ����ʣ��ʱ��
inline void decreaseTime(double time)
{
	leftTime -= time;
	if (leftTime < 0)
		leftTime = 0;
}

// ��Ϸ��������
inline int gameOver()
{
	return leftTime <= 0 || score >= SCORE_UPPER_LIMIT;
}

#endif