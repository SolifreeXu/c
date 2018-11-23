#pragma once

#include <graphics.h>	// EGE��PIMAGE����
#include "logic.h"

#define WINDOW_WIDTH 625	// ���ڿ��
#define WINDOW_HEIGHT 500	// ���ڸ߶�
#define COLOR_BLOCKS_NUMBER 6	// ɫ������
#define COLOR_BLOCK_PIXELS 50	// ɫ�����ش�С
#define MAP_X 150	// ɫ��������Ͻ�X����
#define MAP_Y 25	// ɫ��������Ͻ�Y����
#define MAP_PIXELS (MAP_SIZE*COLOR_BLOCK_PIXELS)	// ɫ��������ش�С
#define LEAVE_X 25	// �˳�ͨ�����Ͻ�X����
#define LEAVE_Y 360	// �˳�ͨ�����Ͻ�Y����
#define LEAVE_PIXELS 100	// �˳�ͨ�����ش�С
#define ESC 27	// �˳���Ϸ��ASCII��

void initData();	// ��ʼ��Ϸ����
void initEnviron();	// ��ʼ��Ϸ����
void closeEnviron();	// �ر���Ϸ����
bool end();	// ��Ϸ��ֽ���
void render(bool onlyInfo);	// ��Ⱦ��Ϸ����
void fill();	// ���ɫ��
int play();	// ��ҿ���ģ��