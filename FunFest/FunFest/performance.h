#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#include <graphics.h>	// EGE��PIMAGE����
#include "logic.h"

extern PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// ͼƬָ�����飬��ż���ͼƬ�ĵ�ַ
extern PIMAGE background;	// ��Ϸ����ͼƬָ��
extern PIMAGE scoreZone;	// ��������ͼƬָ��
extern PIMAGE timeZone;	// ʱ�䱳��ͼƬָ��
extern PIMAGE leaveChannel;	// �˳�ͨ��ͼƬָ��
extern char szBuffer[BUFSIZ];	// ��Ϸ�ַ���ר�û�����

void initEnviron();	// ��ʼ����Ϸ����
void displayEnd();	// ��Ϸ��ֻ���
void displayInfo();	// ��ʾ��Ϸ��Ϣ
void render();	// ��ͼ
void fill();	// ���������ɫ��
int play();	// ��ҿ��Ʋ���

#endif