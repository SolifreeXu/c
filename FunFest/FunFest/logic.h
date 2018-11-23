#pragma once

#include <stdio.h>
#include <math.h>	// abs����
#include <stdbool.h>	// bool����

#define COLOR_BLOCKS_LOWER_LIMIT 3	// ��������������ɫ����
#define INITIAL_TIME 10	// ��Ϸ��ʼʣ��ʱ�䣨����Ϊ��λ��
#define TIME_UPPER_LIMIT 20	// ���ʣ��ʱ��
#define SCORE_UPPER_LIMIT 999	// ��������
#define MAP_SIZE 9	// ɫ������С

bool gameOver();	// ��Ϸ��������
void decreaseTime(double time);	// ������Ϸ��ʱ����ʣ��ʱ��
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

// ������ṹ��
struct Point
{
	int x;	// x����
	int y;	// y����
};

// �ж���ɫ���ھ������Ƿ�����
inline bool adjacent(const Point *point1, const Point *point2)
{
	if (point1 == NULL || point2 == NULL)
		return false;
	return abs((point1->x + point1->y) - (point2->x + point2->y)) == 1 
		&& (point1->x == point2->x || point1->y == point2->y);
}