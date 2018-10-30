#ifndef _PERFORMANCE_H
#define _PERFORMANCE_H

#include <graphics.h>	// EGE的PIMAGE类型
#include "logic.h"

extern PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// 图片指针数组，存放加载图片的地址
extern PIMAGE background;	// 游戏背景图片指针
extern PIMAGE scoreZone;	// 分数背景图片指针
extern PIMAGE timeZone;	// 时间背景图片指针
extern PIMAGE leaveChannel;	// 退出通道图片指针
extern char szBuffer[BUFSIZ];	// 游戏字符串专用缓冲区

void initEnviron();	// 初始化游戏引擎
void displayEnd();	// 游戏结局画面
void displayInfo();	// 显示游戏信息
void render();	// 绘图
void fill();	// 填充消掉的色块
int play();	// 玩家控制部分

#endif