#ifndef _ENGINE_H
#define _ENGINE_H

#include"FunFest.h"
#include<graphics.h>	//EGE中的PIMAGE类型需要

extern PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	//图片指针数组，存放加载图片的地址
extern PIMAGE background;	//游戏背景图片指针
extern PIMAGE scoreZone;	//分数背景图片指针
extern PIMAGE timeZone;	//时间背景图片指针
extern PIMAGE leave;	//退出通道图片指针
extern char strBuffer[_MAX_PATH];	//游戏字符串专用缓冲区

void initEngine();	//初始化游戏引擎
void displayInfo();	//显示游戏信息
void draw();	//绘图
void fill();	//填充消掉的色块
int play();	//玩家控制部分
void endInterface();	//游戏结局画面

#endif