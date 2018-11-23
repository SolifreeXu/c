#include "performance.h"

//#pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")

// ��Ϸ����
bool game()
{
	initData();	// ��ʼ��Ϸ����
	render(false);	// ��ʾ���ֵ�ͼ
	while (count() != 0)	// �ж��Ƿ���������ɫ�飬����0˵��δ����ɫ��
	{
		erase();	// ����ɫ��
		fill();	// ������������ɫ��
	}
	double startTime, endTime;	// ÿ��ѭ����ʱ�Ŀ�ʼʱ�������ʱ��
	startTime = fclock();	// ��ȡϵͳʱ����Ϊ��Ϸ��ʼʱ��
	for (; !gameOver(); delay_jfps(240))	// �ж���Ϸ�Ƿ�δ����
	{
		if (play() == ESC)	// ������Ϊ����
			return false;	// ���������Ϣ��ESC���˳���Ϸ����
		while (count() != 0)	// �ж��Ƿ���������ɫ�飬����0˵��δ����ɫ��
		{
			erase();	// ����ɫ��
			update();	// ������Ϣ
			fill();	// ������������ɫ��
		}
		endTime = fclock();	// ��ȡϵͳʱ����Ϊ����ѭ������ʱ��
		decreaseTime(endTime - startTime);	// ���㱾��ѭ����ʣ��ʱ��
		startTime = endTime;	// ������ѭ������ʱ������Ϊ������ѭ���Ŀ�ʼʱ��
		render(true);	// ֻ��Ⱦ��Ϸ��ʾ��Ϣ
	}
	return !end();	// ��ʾ��ֽ��棬�������û��Ƿ������Ϸ
}

int main()
{
	initEnviron();	// ��ʼ��Ϸ����
	while (game());	// ��Ϸ����
	closeEnviron();	// �ر���Ϸ����
	return 0;
}