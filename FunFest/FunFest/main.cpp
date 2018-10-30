#include "performance.h"

#pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")

// ��ʼ��Ϸ���ݺͻ���
void initGame()
{
	initEnviron();	// ��ʼ����Ϸ����
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);	// ��ʼ����ͼ����
	setcaption("������");	// ���ô��ڱ���
	setfont(32, 18, "��Բ");	// ���������С����ʽ
	setbkmode(TRANSPARENT);	// �������屳��ģʽΪ͸��
	setcolor(0xff0000);	// ���õ�ǰ��ͼǰ��ɫ
	// ����ͼƬ
	background = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	// �½�һIMAGE���󣬽�����ĵ�ַ����PIMAGEָ��
	getimage(background, "image/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);	// ��������ͼƬ�������ڴ�IMAGE������
	scoreZone = newimage(100, 100);
	getimage(scoreZone, "image/score zone.jpg", 100, 100);
	timeZone = newimage(100, 100);
	getimage(timeZone, "image/time zone.png", 100, 100);
	leaveChannel = newimage(100, 100);
	getimage(leaveChannel, "image/leave.png", 100, 100);
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)	// ѭ������ɫ��
	{
		img[i] = newimage(COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
		sprintf(szBuffer, "image/%d.jpg", i);	// ����ɫ�����·������д�뻺����szBuffer��
		getimage(img[i], szBuffer, COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	}
}

// ������Ϸ���ݺͻ���
void closeGame()
{
	// �ͷ�new��image����
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)
		delimage(img[i]);
	delimage(leaveChannel);
	delimage(timeZone);
	delimage(scoreZone);
	delimage(background);
	// �ر�ͼ�λ���
	closegraph();
}

// ��Ϸ����
void game()
{
	draw();	// ��ʾ���ֵ�ͼ
	// ��ͼ����û�п�λ�ã�����������
	static double startTime, endTime;	// ÿ��ѭ����ʱ�Ŀ�ʼʱ�������ʱ��
	startTime = fclock();	// ��ȡϵͳʱ����Ϊ��Ϸ��ʼʱ��
	while (!gameOver())	// �ж���Ϸ�Ƿ�δ����
	{
		while (clear() != 0)	// �ж��Ƿ���������ɫ�飬����0˵��δ����ɫ��
			fill();	// ������������ɫ��
		if (play() == ESC)	// ������Ϊ����
			return;	// ���������Ϣ��ESC���˳���Ϸ����
		delay_jfps(200);	// ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡������fps��200
		endTime = fclock();	// ��ȡϵͳʱ����Ϊ����ѭ������ʱ��
		decreaseTime(endTime - startTime);	// ���㱾��ѭ����ʣ��ʱ��
		startTime = endTime;	// ������ѭ������ʱ������Ϊ������ѭ���Ŀ�ʼʱ��
		displayInfo();
	}
	draw();	// ������Ϸ��ֳ���
	displayEnd();	// ��ʾ��Ϸ��ֻ���
	delay_ms(1000);	// ��ͣ1000ms
}

int main()
{
	initGame();	// ��ʼ����Ϸ���ݺͻ���
	game();	// ��Ϸ����
	closeGame();	// ������Ϸ���ݺͻ���
	return 0;
}