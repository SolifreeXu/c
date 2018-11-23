#include <string.h>
#include "performance.h"

#define RESTART_TITLE "重新开始"
#define QUIT_TITLE "退出游戏"

// 矩形块结构体
struct Rect
{
	int left;	// 左边位置
	int top;	// 上边位置
	int width;	// 宽
	int height;	// 高
};

extern int map[MAP_SIZE + 1][MAP_SIZE + 1];	// 游戏色块矩阵
extern unsigned score;	// 游戏得分
extern double leftTime;	// 游戏剩余时间

static PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// 图像指针数组，存放加载的图像地址
static PIMAGE background;	// 游戏背景图像指针
static PIMAGE scoreZone;	// 分数背景图像指针
static PIMAGE timeZone;	// 时间背景图像指针
static PIMAGE leaveChannel;	// 退出通道图像指针
static PIMAGE canvas;	// 画布指针
static PIMAGE imageBuffer;	// 图像缓冲区指针
static Rect mapRect = { MAP_X, MAP_Y, MAP_PIXELS, MAP_PIXELS };	// 地图矩形块
static Rect leaveRect = { LEAVE_X, LEAVE_Y, LEAVE_PIXELS, LEAVE_PIXELS };	// 退出通道矩形块
static Rect endRect = { 0, (WINDOW_HEIGHT - 160) >> 1, WINDOW_WIDTH, 160 };	// 结束背景矩形块
static Rect restartRect;	// 重新开始矩形块
static Rect quitRect;	// 退出游戏矩形块
static mouse_msg msg, msg1, msg2;	// mouse_msg鼠标消息结构体变量
static int clicksNumber;	// 鼠标点击次数，实现色块消除规则
static char szBuffer[BUFSIZ];	// 游戏字符串专用缓冲区

// 初始鼠标消息位置
inline void initMouseMsg()
{
	// 鼠标消息位置初始为-COLOR_BLOCK_PIXELS，方便判断是否产生有效鼠标消息
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;
}

// 初始游戏数据
void initData()
{
	initMouseMsg();	// 初始鼠标消息位置
	clicksNumber = 0;	// 初始鼠标点击次数
	score = 0;	// 得分清零
	leftTime = INITIAL_TIME;	// 初始剩余时间
	// 给色块数组赋值，初始化地图状态
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		map[i][j] = random(COLOR_BLOCKS_NUMBER) + 1;	// random获取整型随机数
}

// 设置字体风格
inline void setFont()
{
	setfont(32, 18, "幼圆");	// 设置字体大小及样式
	setbkmode(TRANSPARENT);	// 设置字体背景模式为透明
	setcolor(LIGHTRED);	// 设置前景色
}

// 初始游戏环境
void initEnviron()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);	// 初始化绘图环境
	setcaption("消消乐");	// 设置窗口标题
	setrendermode(RENDER_MANUAL);	// 设置窗口更新模式为手动更新
	randomize();	// 初始化随机数种子

	setFont();
	// 计算重新开始矩形块位置和大小
	restartRect = { 112, endRect.top + endRect.height - 9 - 36, textwidth(RESTART_TITLE), 36 };
	// 计算退出游戏矩形块位置和大小
	int len = textwidth(QUIT_TITLE);
	quitRect = { WINDOW_WIDTH - 112 - len, restartRect.top, len, restartRect.height };
	
	// 加载图片
	background = newimage();	// 新建一IMAGE对象，并把对象地址赋予PIMAGE指针
	getimage(background, "image/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);	// 加载物理图片数据至内存IMAGE对象
	scoreZone = newimage();
	getimage(scoreZone, "image/score zone.jpg", 100, 100);
	timeZone = newimage();
	getimage(timeZone, "image/time zone.png", 100, 100);
	leaveChannel = newimage();
	getimage(leaveChannel, "image/leave channel.png", 100, 100);

	img[0] = newimage(COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	for (int i = 1; i <= COLOR_BLOCKS_NUMBER; ++i)	// 循环加载色块
	{
		sprintf(szBuffer, "image/%d.jpg", i);	// 生成色块相对路径名，写入缓冲区szBuffer中
		img[i] = newimage();
		getimage(img[i], szBuffer, COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	}

	imageBuffer = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	// 创建IMAGE对象作为图像缓冲区
	canvas = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	// 创建IMAGE对象作为画布
	settarget(canvas);	// 设置画布为绘图目标
	putimage(0, 0, background);	// 画地图背景
	putimage_transparent(NULL, leaveChannel, 25, 360, BLACK);	// 显示退出通道
	settarget(NULL);	// 设置窗口为绘图目标
}

// 关闭游戏环境
void closeEnviron()
{
	// 释放调用newimage创建的IMAGE对象
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)
		delimage(img[i]);
	delimage(background);
	delimage(scoreZone);
	delimage(timeZone);
	delimage(leaveChannel);
	delimage(canvas);
	delimage(imageBuffer);
	// 关闭图形环境
	closegraph();
}

// 判断点坐标是否位于矩形块内
inline bool contain(const Rect *rect, const Point *point)
{
	if (rect == NULL || point == NULL)
		return false;
	return point->x > rect->left && point->x < rect->left + rect->width 
		&& point->y > rect->top && point->y < rect->top + rect->height;
}

// 游戏结局界面
bool end()
{
	setfillcolor(WHITE);	// 设置填充颜色为白色
	bar(endRect.left, endRect.top, endRect.left + endRect.width, endRect.top + endRect.height);	// 绘制结束背景矩形块

	if (score < SCORE_UPPER_LIMIT)	// 未达分数上限
		sprintf(szBuffer, "The final score is %d", score);	// 将最终得分提示写入缓冲区
	else	// 通关
		strcpy(szBuffer, "Congratulations!");
	setFont();
	int len = textwidth(szBuffer);	// 计算提示信息长度
	outtextrect((WINDOW_WIDTH - len) >> 1, endRect.top + 9, len, 36, szBuffer);	// 在指定位置给予提示
	outtextrect(restartRect.left, restartRect.top, restartRect.width, restartRect.height, RESTART_TITLE);
	outtextrect(quitRect.left, quitRect.top, quitRect.width, quitRect.height, QUIT_TITLE);

	Point point;
	while (1)
	{
		if (mousemsg())	// 有鼠标消息返回非零值
		{
			msg = getmouse();	// 获取鼠标消息
			if (msg.is_down() && msg.is_left())	// 按下鼠标左键
			{
				point.x = msg.x;
				point.y = msg.y;
				// 点击重新开始
				if (contain(&restartRect, &point))
					return false;	// 游戏还未结束，返回false
				// 点击退出游戏
				if (contain(&quitRect, &point))
					return true;	// 游戏结束，返回true
			}
		}
		delay_ms(10);	// 平均延迟10毫秒
	}
	return false;
}

// 绘图（资源可导入rc文件）
void render(bool onlyInfo)
{
	if (onlyInfo)	// 只更新游戏提示信息
		getimage(imageBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);	// 获取窗口图像至图像缓冲区
	settarget(imageBuffer);	// 设置图像缓冲区为绘图目标
	if (!onlyInfo)	// 渲染整个窗口
		putimage(0, 0, canvas);	// 画地图背景
	
	setFont();
	putimage_transparent(NULL, scoreZone, 25, 30, BLACK);	// 画分数背景
	rectprintf(47, 60, 54, 40, "%3d", score);	// 显示分数
	putimage_transparent(NULL, timeZone, 25, 190, BLACK);	// 画时间背景
	rectprintf(58, 220, 36, 40, "%2.0lf", leftTime);	// 显示时间

	if (!onlyInfo)
	{
		for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			// 以透明方式绘制指定图像（如果map[i][j]==0，没有对应图像，不会贴出来）
			putimage_transparent(NULL, img[map[i][j]], COLOR_BLOCK_PIXELS * i + MAP_X, COLOR_BLOCK_PIXELS * j + MAP_Y, BLACK);
	}

	settarget(NULL);	// 设置窗口为绘图目标
	putimage(0, 0, imageBuffer);	// 把图像缓冲区的图像绘至窗口
}

// 填充色块
void fill()
{
	// 从地图最下面开始，往上依次检测一行
	for (int j = MAP_SIZE - 1; j >= 0; --j)
	{
		for (int i = MAP_SIZE - 1; i >= 0; --i)
		{
			while (map[i][j] == 0)	// 判断当前位置是否为空
			{
				delay_jfps(10);	// 平均延迟1000/fps毫秒，用于稳定逻辑帧率控制，绘图带跳帧，这里fps即10
				// 色块依次往下掉
				for (int k = j; k > 0; --k)
					map[i][k] = map[i][k - 1];
				map[i][0] = random(COLOR_BLOCKS_NUMBER) + 1;	// 最上面一行的空位置随机产生色块
				render(false);	// 每次色块变化后，绘制出当前地图
			}
		}
	}
}

// 鼠标点击位置转换为色块矩阵下标
inline int posToIndex(int pos, int startPos)
{
	return (pos - startPos) / COLOR_BLOCK_PIXELS;
}

// 玩家控制部分
int play()
{
	static Point point1, point2;
	// 无鼠标消息或者已完成两次鼠标点击
	if (mousemsg() == 0 || msg1.x != -COLOR_BLOCK_PIXELS && msg2.x != -COLOR_BLOCK_PIXELS)
		return 0;
	msg = getmouse();	// 获取鼠标消息
	if (!(msg.is_down() && msg.is_left()))	// 未按下鼠标左键
		return 0;
	point1.x = msg.x;
	point1.y = msg.y;
	if (contain(&leaveRect, &point1))	// 点击退出游戏区域
		return ESC;	// 返回退出游戏消息
	if (!contain(&mapRect, &point1))	// 超出色块矩阵界限
		return -1;
	++clicksNumber;	// 鼠标点击次数加一
	if (clicksNumber == 1)	// 第一次点击
	{
		// 计算鼠标点击的色块四角的坐标
		point1.x = msg.x - (msg.x - MAP_X) % COLOR_BLOCK_PIXELS;
		point1.y = msg.y - (msg.y - MAP_Y) % COLOR_BLOCK_PIXELS;
		point2.x = point1.x + COLOR_BLOCK_PIXELS;
		point2.y = point1.y + COLOR_BLOCK_PIXELS;
		setcolor(LIGHTRED);	// 设置前景色
		rectangle(point1.x, point1.y, point2.x, point2.y);	// 绘制空心矩形，提示第一次点击的色块
		delay_jfps(240);
		msg1 = msg;
		return 1;	// 返回，等待用户下一次点击色块
	}
	else	// 第二次点击
	{
		msg2 = msg;
		clicksNumber = 0;	// 已完成鼠标两次点击操作，恢复初始状态
		render(false);	// 绘制界面
	}
	// 界面坐标映射成色块矩阵下标
	point1.x = posToIndex(msg1.x, MAP_X);
	point1.y = posToIndex(msg1.y, MAP_Y);
	point2.x = posToIndex(msg2.x, MAP_X);
	point2.y = posToIndex(msg2.y, MAP_Y);
	initMouseMsg();	// 达到一次交换条件，让鼠标消息位置恢复到原始状态
	// 上下左右均不相邻，直接退出
	if (!adjacent(&point1, &point2))
		return -1;
	swap(&map[point1.x][point1.y], &map[point2.x][point2.y]);	// 交换两色块位置
	render(false);	// 绘制色块位置交换后的地图
	if (count() == 0)	// 交换之后，返回0说明没有消除色块
	{
		delay_ms(100);
		swap(&map[point1.x][point1.y], &map[point2.x][point2.y]);	// 再次交换回去
		render(false);	// 绘制出交换后的地图
	}
	return 2;
}