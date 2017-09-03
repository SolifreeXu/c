#ifndef _ENGINE_H
#define _ENGINE_H

#include"FunFest.h"
#include<graphics.h>	//EGE�е�PIMAGE������Ҫ

extern PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	//ͼƬָ�����飬��ż���ͼƬ�ĵ�ַ
extern PIMAGE background;	//��Ϸ����ͼƬָ��
extern PIMAGE scoreZone;	//��������ͼƬָ��
extern PIMAGE timeZone;	//ʱ�䱳��ͼƬָ��
extern PIMAGE leave;	//�˳�ͨ��ͼƬָ��
extern char strBuffer[_MAX_PATH];	//��Ϸ�ַ���ר�û�����

void initEngine();	//��ʼ����Ϸ����
void displayInfo();	//��ʾ��Ϸ��Ϣ
void draw();	//��ͼ
void fill();	//���������ɫ��
int play();	//��ҿ��Ʋ���
void endInterface();	//��Ϸ��ֻ���

#endif