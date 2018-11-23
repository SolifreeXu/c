#include "performance.h"

//#pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")

// 游戏过程
bool game()
{
	initData();	// 初始游戏数据
	render(false);	// 显示开局地图
	while (count() != 0)	// 判断是否有消除的色块，返回0说明未消除色块
	{
		erase();	// 消除色块
		fill();	// 消掉过后填满色块
	}
	double startTime, endTime;	// 每次循环计时的开始时间与结束时间
	startTime = fclock();	// 获取系统时间作为游戏开始时间
	for (; !gameOver(); delay_jfps(240))	// 判断游戏是否未结束
	{
		if (play() == ESC)	// 进行人为操作
			return false;	// 如果返回消息是ESC，退出游戏过程
		while (count() != 0)	// 判断是否有消除的色块，返回0说明未消除色块
		{
			erase();	// 消除色块
			update();	// 更新信息
			fill();	// 消掉过后填满色块
		}
		endTime = fclock();	// 获取系统时间作为本次循环结束时间
		decreaseTime(endTime - startTime);	// 计算本次循环后剩余时间
		startTime = endTime;	// 将本次循环结束时间设置为接下来循环的开始时间
		render(true);	// 只渲染游戏提示信息
	}
	return !end();	// 显示结局界面，并返回用户是否结束游戏
}

int main()
{
	initEnviron();	// 初始游戏环境
	while (game());	// 游戏过程
	closeEnviron();	// 关闭游戏环境
	return 0;
}