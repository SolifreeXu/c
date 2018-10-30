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

// �������Ƿ�λ�ھ��ο���
inline int contain(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY)
{
	return dstX > srcX && dstX < srcX + srcWidth && dstY > srcY && dstY < srcY + srcHeight;
}

// ��ɫ���ھ������Ƿ�����
inline int adjacent(int dimen01, int dimen02, int dimen11, int dimen12)
{
	return abs((dimen01 + dimen02) - (dimen11 + dimen12)) == 1 && (dimen01 == dimen11 || dimen02 == dimen12);
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