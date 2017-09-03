#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include"Engine.h"

#pragma comment(linker,"/NODEFAULTLIB:libcpmt.lib")

//初始化游戏数据及环境
void initGame()
{
	initEngine();	//初始化游戏引擎
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);	//初始化绘图环境
	setcaption("消消乐");	//设置窗口标题
	setfont(32, 18, "幼圆");	//设置字体大小及样式
	setbkmode(TRANSPARENT);	//设置字体背景模式为透明
	setcolor(0xff0000);	//设置当前绘图前景色
	//加载图片
	background = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	//新建一IMAGE对象，将对象的地址赋予PIMAGE指针
	getimage(background, "image/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);	//加载物理图片数据至内存IMAGE对象中
	scoreZone = newimage(100, 100);
	getimage(scoreZone, "image/score zone.jpg", 100, 100);
	timeZone = newimage(100, 100);
	getimage(timeZone, "image/time zone.png", 100, 100);
	leave = newimage(100, 100);
	getimage(leave, "image/leave.png", 100, 100);
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)	//循环加载色块
	{
		img[i] = newimage(COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
		_snprintf(strBuffer, _MAX_PATH, "image/%d.jpg", i);	//生成色块相对路径名，写入缓冲区strBuffer中
		getimage(img[i], strBuffer, COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	}
}

//清理游戏数据及环境
void closeGame()
{
	//释放new的image对象
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)
		delimage(img[i]);
	delimage(leave);
	delimage(timeZone);
	delimage(scoreZone);
	delimage(background);
	//关闭图形环境
	closegraph();
}

//游戏过程
void game()
{
	draw();	//显示开局地图
		//地图处理（没有空位置，不能消除）
	static time_t startTime, endTime;	//每次循环计时的开始时间与结束时间
	startTime = time(NULL);	//获取系统时间作为游戏开始时间
	while (timeLeft > 0)	//判断游戏是否还有剩余时间
	{
		while (clear() != 0);	//判断是否有可以消除的地方，返回0表示没有可消除的地方
			fill();	//消掉过后填满色块
		if (play() == ESC)	//进行人为操作
			return;	//如果返回消息是ESC，退出游戏过程
		_sleep(1);	//暂停1毫秒
		displayInfo();	//显示游戏信息
		endTime = time(NULL);	//获取系统时间作为本次循环结束时间
		timeLeft -= (endTime - startTime);	//计算本次循环后剩余时间
		startTime = endTime;	//将本次循环结束时间设置为接下来循环的开始时间
	}
	timeLeft = 0;	//剩余时间清零
	draw();	//绘制游戏结局场景
	endInterface();	//显示游戏结局画面
	Sleep(1000);	//暂停1000ms
}

int main()
{
	initGame();	//初始化游戏数据及环境
	game();	//游戏过程
	closeGame();	//清理游戏数据及环境
	return 0;
}