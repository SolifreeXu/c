#include "performance.h"

PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// 图片指针数组，存放加载图片的地址
PIMAGE background;	// 游戏背景图片指针
PIMAGE scoreZone;	// 分数背景图片指针
PIMAGE timeZone;	// 时间背景图片指针
PIMAGE leaveChannel;	// 退出通道图片指针
char szBuffer[BUFSIZ];	// 游戏字符串专用缓冲区
static mouse_msg msg, msg1, msg2;	// mouse_msg鼠标消息结构体变量
static int clicksNumber;	// 鼠标点击次数，控制色块的消除规则

// 初始鼠标消息位置
inline void initMouseMsg()
{
	// 鼠标消息位置初始为-COLOR_BLOCK_PIXELS，方便判断是否产生有效鼠标消息
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;
}

// 初始化游戏环境
void initEnviron()
{
	randomize();	// 初始化随机数种子
	score = 0;	// 得分清零
	leftTime = INITIAL_TIME;	// 初始剩余时间
	// 给数组赋值，初始化地图状态
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		map[i][j] = random(COLOR_BLOCKS_NUMBER) + 1;	// random获取整型随机数
	initMouseMsg();	// 初始鼠标消息位置
	clicksNumber = 0;	// 初始鼠标点击次数
}

// 游戏结局画面
void displayEnd()
{
	if (score < SCORE_UPPER_LIMIT)
		sprintf(szBuffer, "The final score is %d", score);	// 将最终得分提示写入缓冲区
	else
		strcpy(szBuffer, "Congratulations!");
	int len = strlen(szBuffer) * 18;	// 计算提示信息长度
	setfillcolor(WHITE);	// 设置当前填充颜色
	bar(0, (WINDOW_HEIGHT - 50) / 2, WINDOW_WIDTH, (WINDOW_HEIGHT - 50) / 2 + 50);	// 绘制矩形
	rectprintf((WINDOW_WIDTH - len) / 2, (WINDOW_HEIGHT - 36) / 2, len, 36, szBuffer);	// 输出最终得分提示
	delay_jfps(300);	// 平均延迟1000/fps毫秒，用于稳定逻辑帧率控制，绘图带跳帧，这里fps即300
}

// 显示游戏信息
void displayInfo()
{
	// 打印分数
	putimage_transparent(NULL, scoreZone, 25, 30, BLACK);
	rectprintf(47, 60, 54, 40, "%3d", score);
	// 打印时间
	putimage_transparent(NULL, timeZone, 25, 190, BLACK);
	rectprintf(58, 220, 36, 40, "%2.0lf", leftTime);
	delay_jfps(300);
}

// 绘图（资源可导入rc文件）
void render()
{
	cleardevice();	// 清屏
	putimage(0, 0, background);	// 画地图背景
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		// 在屏幕上以透明方式绘制指定图像（如果map[i][j]==0，没有对应的图，不会贴出来）
		putimage_transparent(NULL, img[map[i][j]], COLOR_BLOCK_PIXELS * i + MAP_X, COLOR_BLOCK_PIXELS * j + MAP_Y, BLACK);
	putimage_transparent(NULL, leaveChannel, 25, 360, BLACK);	// 显示退出游戏通道
	displayInfo();	// 显示游戏信息
}

// 填充消掉的色块
void fill()
{
	// 从地图最下面开始，往上依次检测一行
	for (int j = MAP_SIZE - 1; j >= 0; --j)
	{
		for (int i = MAP_SIZE - 1; i >= 0; --i)
		{
			while (map[i][j] == 0)	// 判断当前位置是否为空
			{
				delay_jfps(10);
				// 色块往下掉
				for (int k = j; k > 0; --k)
					map[i][k] = map[i][k - 1];
				map[i][0] = random(COLOR_BLOCKS_NUMBER) + 1;	// 最上面一行的空位置随机产生色块
				render();	// 每次色块变化后，绘制出当前地图
			}
		}
	}
}

// 点坐标是否位于矩形块内
inline int contain(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY)
{
	return dstX > srcX && dstX < srcX + srcWidth && dstY > srcY && dstY < srcY + srcHeight;
}

// 鼠标点击位置转换为色块矩阵下标
inline int positionToIndex(int position, int startingPosition)
{
	return (position - startingPosition) / COLOR_BLOCK_PIXELS;
}

// 玩家控制部分
int play()
{
	static int x1, y1, x2, y2;
	// 未完成两次鼠标点击且有鼠标消息
	if (!mousemsg() || msg1.x != -COLOR_BLOCK_PIXELS && msg2.x != -COLOR_BLOCK_PIXELS)
		return 0;
	msg = getmouse();	// 获取鼠标消息
	// 点击方法
	if (!(msg.is_down() && msg.is_left()))	// 鼠标左键按下
		return 0;
	if (contain(LEAVE_X, LEAVE_Y, LEAVE_PIXELS, LEAVE_PIXELS, msg.x, msg.y))	// 点击退出游戏区域
		return ESC;	// 返回退出游戏消息
	if (!contain(MAP_X, MAP_Y, MAP_PIXELS, MAP_PIXELS, msg.x, msg.y))	// 超出色块矩阵界限
		return -1;
	++clicksNumber;	// 鼠标点击次数加一
	if (clicksNumber == 1)	// 第一次点击
	{
		// 计算鼠标点击的色块四角的坐标
		x1 = msg.x - (msg.x - MAP_X) % COLOR_BLOCK_PIXELS;
		y1 = msg.y - (msg.y - MAP_Y) % COLOR_BLOCK_PIXELS;
		x2 = x1 + COLOR_BLOCK_PIXELS;
		y2 = y1 + COLOR_BLOCK_PIXELS;
		rectangle(x1, y1, x2, y2);	// 绘制空心矩形，提示用户第一次点击的色块
		delay_jfps(300);
		msg1 = msg;
		return 1;	// 返回，等待用户下一次点击色块
	}
	else	// 第二次点击
	{
		msg2 = msg;
		clicksNumber = 0;	// 已完成鼠标两次点击操作，恢复初始状态
		render();	// 绘制界面
	}
	// 界面坐标映射成色块矩阵下标
	x1 = positionToIndex(msg1.x, MAP_X);
	y1 = positionToIndex(msg1.y, MAP_Y);
	x2 = positionToIndex(msg2.x, MAP_X);
	y2 = positionToIndex(msg2.y, MAP_Y);
	initMouseMsg();	// 达到一次交换条件，让鼠标消息位置恢复到原始状态
	// 上下左右均不相邻，直接退出
	if (!adjacent(x1, y1, x2, y2))
		return -1;
	swap(&map[x1][y1], &map[x2][y2]);	// 交换两色块位置
	render();	// 绘制色块位置交换后的地图
	if (count() == 0)	// 交换之后，返回0说明没有消除色块
	{
		delay_ms(100);
		swap(&map[x1][y1], &map[x2][y2]);	// 再次交换回去
		render();	// 并且绘制出交换后的地图
	}
	return 2;
}