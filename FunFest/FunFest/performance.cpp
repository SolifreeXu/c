#include <string.h>
#include "performance.h"

#define RESTART_TITLE "���¿�ʼ"
#define QUIT_TITLE "�˳���Ϸ"

// ���ο�ṹ��
struct Rect
{
	int left;	// ���λ��
	int top;	// �ϱ�λ��
	int width;	// ��
	int height;	// ��
};

extern int map[MAP_SIZE + 1][MAP_SIZE + 1];	// ��Ϸɫ�����
extern unsigned score;	// ��Ϸ�÷�
extern double leftTime;	// ��Ϸʣ��ʱ��

static PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// ͼ��ָ�����飬��ż��ص�ͼ���ַ
static PIMAGE background;	// ��Ϸ����ͼ��ָ��
static PIMAGE scoreZone;	// ��������ͼ��ָ��
static PIMAGE timeZone;	// ʱ�䱳��ͼ��ָ��
static PIMAGE leaveChannel;	// �˳�ͨ��ͼ��ָ��
static PIMAGE canvas;	// ����ָ��
static PIMAGE imageBuffer;	// ͼ�񻺳���ָ��
static Rect mapRect = { MAP_X, MAP_Y, MAP_PIXELS, MAP_PIXELS };	// ��ͼ���ο�
static Rect leaveRect = { LEAVE_X, LEAVE_Y, LEAVE_PIXELS, LEAVE_PIXELS };	// �˳�ͨ�����ο�
static Rect endRect = { 0, (WINDOW_HEIGHT - 160) >> 1, WINDOW_WIDTH, 160 };	// �����������ο�
static Rect restartRect;	// ���¿�ʼ���ο�
static Rect quitRect;	// �˳���Ϸ���ο�
static mouse_msg msg, msg1, msg2;	// mouse_msg�����Ϣ�ṹ�����
static int clicksNumber;	// �����������ʵ��ɫ����������
static char szBuffer[BUFSIZ];	// ��Ϸ�ַ���ר�û�����

// ��ʼ�����Ϣλ��
inline void initMouseMsg()
{
	// �����Ϣλ�ó�ʼΪ-COLOR_BLOCK_PIXELS�������ж��Ƿ������Ч�����Ϣ
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;
}

// ��ʼ��Ϸ����
void initData()
{
	initMouseMsg();	// ��ʼ�����Ϣλ��
	clicksNumber = 0;	// ��ʼ���������
	score = 0;	// �÷�����
	leftTime = INITIAL_TIME;	// ��ʼʣ��ʱ��
	// ��ɫ�����鸳ֵ����ʼ����ͼ״̬
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		map[i][j] = random(COLOR_BLOCKS_NUMBER) + 1;	// random��ȡ���������
}

// ����������
inline void setFont()
{
	setfont(32, 18, "��Բ");	// ���������С����ʽ
	setbkmode(TRANSPARENT);	// �������屳��ģʽΪ͸��
	setcolor(LIGHTRED);	// ����ǰ��ɫ
}

// ��ʼ��Ϸ����
void initEnviron()
{
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);	// ��ʼ����ͼ����
	setcaption("������");	// ���ô��ڱ���
	setrendermode(RENDER_MANUAL);	// ���ô��ڸ���ģʽΪ�ֶ�����
	randomize();	// ��ʼ�����������

	setFont();
	// �������¿�ʼ���ο�λ�úʹ�С
	restartRect = { 112, endRect.top + endRect.height - 9 - 36, textwidth(RESTART_TITLE), 36 };
	// �����˳���Ϸ���ο�λ�úʹ�С
	int len = textwidth(QUIT_TITLE);
	quitRect = { WINDOW_WIDTH - 112 - len, restartRect.top, len, restartRect.height };
	
	// ����ͼƬ
	background = newimage();	// �½�һIMAGE���󣬲��Ѷ����ַ����PIMAGEָ��
	getimage(background, "image/background.jpg", WINDOW_WIDTH, WINDOW_HEIGHT);	// ��������ͼƬ�������ڴ�IMAGE����
	scoreZone = newimage();
	getimage(scoreZone, "image/score zone.jpg", 100, 100);
	timeZone = newimage();
	getimage(timeZone, "image/time zone.png", 100, 100);
	leaveChannel = newimage();
	getimage(leaveChannel, "image/leave channel.png", 100, 100);

	img[0] = newimage(COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	for (int i = 1; i <= COLOR_BLOCKS_NUMBER; ++i)	// ѭ������ɫ��
	{
		sprintf(szBuffer, "image/%d.jpg", i);	// ����ɫ�����·������д�뻺����szBuffer��
		img[i] = newimage();
		getimage(img[i], szBuffer, COLOR_BLOCK_PIXELS, COLOR_BLOCK_PIXELS);
	}

	imageBuffer = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	// ����IMAGE������Ϊͼ�񻺳���
	canvas = newimage(WINDOW_WIDTH, WINDOW_HEIGHT);	// ����IMAGE������Ϊ����
	settarget(canvas);	// ���û���Ϊ��ͼĿ��
	putimage(0, 0, background);	// ����ͼ����
	putimage_transparent(NULL, leaveChannel, 25, 360, BLACK);	// ��ʾ�˳�ͨ��
	settarget(NULL);	// ���ô���Ϊ��ͼĿ��
}

// �ر���Ϸ����
void closeEnviron()
{
	// �ͷŵ���newimage������IMAGE����
	for (int i = 0; i <= COLOR_BLOCKS_NUMBER; ++i)
		delimage(img[i]);
	delimage(background);
	delimage(scoreZone);
	delimage(timeZone);
	delimage(leaveChannel);
	delimage(canvas);
	delimage(imageBuffer);
	// �ر�ͼ�λ���
	closegraph();
}

// �жϵ������Ƿ�λ�ھ��ο���
inline bool contain(const Rect *rect, const Point *point)
{
	if (rect == NULL || point == NULL)
		return false;
	return point->x > rect->left && point->x < rect->left + rect->width 
		&& point->y > rect->top && point->y < rect->top + rect->height;
}

// ��Ϸ��ֽ���
bool end()
{
	setfillcolor(WHITE);	// ���������ɫΪ��ɫ
	bar(endRect.left, endRect.top, endRect.left + endRect.width, endRect.top + endRect.height);	// ���ƽ����������ο�

	if (score < SCORE_UPPER_LIMIT)	// δ���������
		sprintf(szBuffer, "The final score is %d", score);	// �����յ÷���ʾд�뻺����
	else	// ͨ��
		strcpy(szBuffer, "Congratulations!");
	setFont();
	int len = textwidth(szBuffer);	// ������ʾ��Ϣ����
	outtextrect((WINDOW_WIDTH - len) >> 1, endRect.top + 9, len, 36, szBuffer);	// ��ָ��λ�ø�����ʾ
	outtextrect(restartRect.left, restartRect.top, restartRect.width, restartRect.height, RESTART_TITLE);
	outtextrect(quitRect.left, quitRect.top, quitRect.width, quitRect.height, QUIT_TITLE);

	Point point;
	while (1)
	{
		if (mousemsg())	// �������Ϣ���ط���ֵ
		{
			msg = getmouse();	// ��ȡ�����Ϣ
			if (msg.is_down() && msg.is_left())	// ����������
			{
				point.x = msg.x;
				point.y = msg.y;
				// ������¿�ʼ
				if (contain(&restartRect, &point))
					return false;	// ��Ϸ��δ����������false
				// ����˳���Ϸ
				if (contain(&quitRect, &point))
					return true;	// ��Ϸ����������true
			}
		}
		delay_ms(10);	// ƽ���ӳ�10����
	}
	return false;
}

// ��ͼ����Դ�ɵ���rc�ļ���
void render(bool onlyInfo)
{
	if (onlyInfo)	// ֻ������Ϸ��ʾ��Ϣ
		getimage(imageBuffer, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);	// ��ȡ����ͼ����ͼ�񻺳���
	settarget(imageBuffer);	// ����ͼ�񻺳���Ϊ��ͼĿ��
	if (!onlyInfo)	// ��Ⱦ��������
		putimage(0, 0, canvas);	// ����ͼ����
	
	setFont();
	putimage_transparent(NULL, scoreZone, 25, 30, BLACK);	// ����������
	rectprintf(47, 60, 54, 40, "%3d", score);	// ��ʾ����
	putimage_transparent(NULL, timeZone, 25, 190, BLACK);	// ��ʱ�䱳��
	rectprintf(58, 220, 36, 40, "%2.0lf", leftTime);	// ��ʾʱ��

	if (!onlyInfo)
	{
		for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
			// ��͸����ʽ����ָ��ͼ�����map[i][j]==0��û�ж�Ӧͼ�񣬲�����������
			putimage_transparent(NULL, img[map[i][j]], COLOR_BLOCK_PIXELS * i + MAP_X, COLOR_BLOCK_PIXELS * j + MAP_Y, BLACK);
	}

	settarget(NULL);	// ���ô���Ϊ��ͼĿ��
	putimage(0, 0, imageBuffer);	// ��ͼ�񻺳�����ͼ���������
}

// ���ɫ��
void fill()
{
	// �ӵ�ͼ�����濪ʼ���������μ��һ��
	for (int j = MAP_SIZE - 1; j >= 0; --j)
	{
		for (int i = MAP_SIZE - 1; i >= 0; --i)
		{
			while (map[i][j] == 0)	// �жϵ�ǰλ���Ƿ�Ϊ��
			{
				delay_jfps(10);	// ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡������fps��10
				// ɫ���������µ�
				for (int k = j; k > 0; --k)
					map[i][k] = map[i][k - 1];
				map[i][0] = random(COLOR_BLOCKS_NUMBER) + 1;	// ������һ�еĿ�λ���������ɫ��
				render(false);	// ÿ��ɫ��仯�󣬻��Ƴ���ǰ��ͼ
			}
		}
	}
}

// �����λ��ת��Ϊɫ������±�
inline int posToIndex(int pos, int startPos)
{
	return (pos - startPos) / COLOR_BLOCK_PIXELS;
}

// ��ҿ��Ʋ���
int play()
{
	static Point point1, point2;
	// �������Ϣ������������������
	if (mousemsg() == 0 || msg1.x != -COLOR_BLOCK_PIXELS && msg2.x != -COLOR_BLOCK_PIXELS)
		return 0;
	msg = getmouse();	// ��ȡ�����Ϣ
	if (!(msg.is_down() && msg.is_left()))	// δ����������
		return 0;
	point1.x = msg.x;
	point1.y = msg.y;
	if (contain(&leaveRect, &point1))	// ����˳���Ϸ����
		return ESC;	// �����˳���Ϸ��Ϣ
	if (!contain(&mapRect, &point1))	// ����ɫ��������
		return -1;
	++clicksNumber;	// �����������һ
	if (clicksNumber == 1)	// ��һ�ε��
	{
		// �����������ɫ���Ľǵ�����
		point1.x = msg.x - (msg.x - MAP_X) % COLOR_BLOCK_PIXELS;
		point1.y = msg.y - (msg.y - MAP_Y) % COLOR_BLOCK_PIXELS;
		point2.x = point1.x + COLOR_BLOCK_PIXELS;
		point2.y = point1.y + COLOR_BLOCK_PIXELS;
		setcolor(LIGHTRED);	// ����ǰ��ɫ
		rectangle(point1.x, point1.y, point2.x, point2.y);	// ���ƿ��ľ��Σ���ʾ��һ�ε����ɫ��
		delay_jfps(240);
		msg1 = msg;
		return 1;	// ���أ��ȴ��û���һ�ε��ɫ��
	}
	else	// �ڶ��ε��
	{
		msg2 = msg;
		clicksNumber = 0;	// �����������ε���������ָ���ʼ״̬
		render(false);	// ���ƽ���
	}
	// ��������ӳ���ɫ������±�
	point1.x = posToIndex(msg1.x, MAP_X);
	point1.y = posToIndex(msg1.y, MAP_Y);
	point2.x = posToIndex(msg2.x, MAP_X);
	point2.y = posToIndex(msg2.y, MAP_Y);
	initMouseMsg();	// �ﵽһ�ν����������������Ϣλ�ûָ���ԭʼ״̬
	// �������Ҿ������ڣ�ֱ���˳�
	if (!adjacent(&point1, &point2))
		return -1;
	swap(&map[point1.x][point1.y], &map[point2.x][point2.y]);	// ������ɫ��λ��
	render(false);	// ����ɫ��λ�ý�����ĵ�ͼ
	if (count() == 0)	// ����֮�󣬷���0˵��û������ɫ��
	{
		delay_ms(100);
		swap(&map[point1.x][point1.y], &map[point2.x][point2.y]);	// �ٴν�����ȥ
		render(false);	// ���Ƴ�������ĵ�ͼ
	}
	return 2;
}