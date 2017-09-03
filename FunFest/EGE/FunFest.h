#ifndef _FUNFEST_H
#define _FUNFEST_H

#include<time.h>	//time_t������Ҫ

#define WINDOW_WIDTH 625	//���ڿ��
#define WINDOW_HEIGHT 500	//���ڸ߶�
#define MAP_X 150	//ɫ�����ʼX����
#define MAP_Y 25	//ɫ�����ʼY����
#define COLOR_BLOCKS_NUMBER 6	//ɫ������
#define COLOR_BLOCK_PIXELS	50	//ɫ�����ش�С
#define MAP_SIZE 9	//ɫ������С
#define LOWER_LIMIT 3	//�ܹ���������������ɫ����
#define INITIAL_TIME 10	//��Ϸ��ʼʣ��ʱ�䣨����Ϊ��λ��
#define ESC 27	//�˳���Ϸ��ASCII��

extern int map[MAP_SIZE + 1][MAP_SIZE + 1];	//��Ϸɫ�����
extern unsigned score;	//��Ϸ�÷�
extern time_t timeLeft;	//��Ϸʣ��ʱ�䣬time_t��__int64�궨��

int swap(int*, int*);	//������
int clear();	//����ɫ��

#endif