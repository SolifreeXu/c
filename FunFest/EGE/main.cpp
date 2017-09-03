#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Engine.h"

#pragma comment(linker,"/NODEFAULTLIB:libcpmt.lib")

//��ʼ����Ϸ���ݼ�����
void initGame()
{
	initEngine();	//��ʼ����Ϸ����
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);	//��ʼ����ͼ����
	setcaption("������");	//���ô��ڱ���
	setfont(32, 18, "��Բ");	//���������С����ʽ
	setbkmode(TRANSPARENT);	//�������屳��ģʽΪ͸��
	setcolor(0xff0000);	//���õ�ǰ��ͼǰ��ɫ
	//����ͼƬ
	background = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	//�½�һIMAGE���󣬽�����ĵ�ַ����PIMAGEָ��
	getimage(background, "image/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);	//��������ͼƬ�������ڴ�IMAGE������
	scoreZone = newimage(100, 100);
	getimage(scoreZone, "image/score zone.jpg", 100, 100);
	timeZone = newimage(100, 100);
	getimage(timeZone, "image/time zone.png", 100, 100);
	leave = newimage(100, 100);
	getimage(leave, "image/leave.png", 100, 100);
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)	//ѭ������ɫ��
	{
		img[i] = newimage(COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
		_snprintf(strBuffer, _MAX_PATH, "image/%d.jpg", i);	//����ɫ�����·������д�뻺����strBuffer��
		getimage(img[i], strBuffer, COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	}
}

//������Ϸ���ݼ�����
void closeGame()
{
	//�ͷ�new��image����
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)
		delimage(img[i]);
	delimage(leave);
	delimage(timeZone);
	delimage(scoreZone);
	delimage(background);
	//�ر�ͼ�λ���
	closegraph();
}

//��Ϸ����
void game()
{
	draw();	//��ʾ���ֵ�ͼ
		//��ͼ����û�п�λ�ã�����������
	static time_t startTime, endTime;	//ÿ��ѭ����ʱ�Ŀ�ʼʱ�������ʱ��
	startTime = time(NULL);	//��ȡϵͳʱ����Ϊ��Ϸ��ʼʱ��
	while (timeLeft > 0)	//�ж���Ϸ�Ƿ���ʣ��ʱ��
	{
		while (clear() != 0);	//�ж��Ƿ��п��������ĵط�������0��ʾû�п������ĵط�
			fill();	//������������ɫ��
		if (play() == ESC)	//������Ϊ����
			return;	//���������Ϣ��ESC���˳���Ϸ����
		_sleep(1);	//��ͣ1����
		displayInfo();	//��ʾ��Ϸ��Ϣ
		endTime = time(NULL);	//��ȡϵͳʱ����Ϊ����ѭ������ʱ��
		timeLeft -= (endTime - startTime);	//���㱾��ѭ����ʣ��ʱ��
		startTime = endTime;	//������ѭ������ʱ������Ϊ������ѭ���Ŀ�ʼʱ��
	}
	timeLeft = 0;	//ʣ��ʱ������
	draw();	//������Ϸ��ֳ���
	endInterface();	//��ʾ��Ϸ��ֻ���
	Sleep(1000);	//��ͣ1000ms
}

int main()
{
	initGame();	//��ʼ����Ϸ���ݼ�����
	game();	//��Ϸ����
	closeGame();	//������Ϸ���ݼ�����
	return 0;
}