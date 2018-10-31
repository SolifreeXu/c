#include "performance.h"

PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	// ͼƬָ�����飬��ż���ͼƬ�ĵ�ַ
PIMAGE background;	// ��Ϸ����ͼƬָ��
PIMAGE scoreZone;	// ��������ͼƬָ��
PIMAGE timeZone;	// ʱ�䱳��ͼƬָ��
PIMAGE leaveChannel;	// �˳�ͨ��ͼƬָ��
char szBuffer[BUFSIZ];	// ��Ϸ�ַ���ר�û�����
static mouse_msg msg, msg1, msg2;	// mouse_msg�����Ϣ�ṹ�����
static int clicksNumber;	// ���������������ɫ�����������

// ��ʼ�����Ϣλ��
inline void initMouseMsg()
{
	// �����Ϣλ�ó�ʼΪ-COLOR_BLOCK_PIXELS�������ж��Ƿ������Ч�����Ϣ
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;
}

// ��ʼ����Ϸ����
void initEnviron()
{
	randomize();	// ��ʼ�����������
	score = 0;	// �÷�����
	leftTime = INITIAL_TIME;	// ��ʼʣ��ʱ��
	// �����鸳ֵ����ʼ����ͼ״̬
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		map[i][j] = random(COLOR_BLOCKS_NUMBER) + 1;	// random��ȡ���������
	initMouseMsg();	// ��ʼ�����Ϣλ��
	clicksNumber = 0;	// ��ʼ���������
}

// ��Ϸ��ֻ���
void displayEnd()
{
	if (score < SCORE_UPPER_LIMIT)
		sprintf(szBuffer, "The final score is %d", score);	// �����յ÷���ʾд�뻺����
	else
		strcpy(szBuffer, "Congratulations!");
	int len = strlen(szBuffer) * 18;	// ������ʾ��Ϣ����
	setfillcolor(WHITE);	// ���õ�ǰ�����ɫ
	bar(0, (WINDOW_HEIGHT - 50) / 2, WINDOW_WIDTH, (WINDOW_HEIGHT - 50) / 2 + 50);	// ���ƾ���
	rectprintf((WINDOW_WIDTH - len) / 2, (WINDOW_HEIGHT - 36) / 2, len, 36, szBuffer);	// ������յ÷���ʾ
	delay_jfps(300);	// ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡������fps��300
}

// ��ʾ��Ϸ��Ϣ
void displayInfo()
{
	// ��ӡ����
	putimage_transparent(NULL, scoreZone, 25, 30, BLACK);
	rectprintf(47, 60, 54, 40, "%3d", score);
	// ��ӡʱ��
	putimage_transparent(NULL, timeZone, 25, 190, BLACK);
	rectprintf(58, 220, 36, 40, "%2.0lf", leftTime);
	delay_jfps(300);
}

// ��ͼ����Դ�ɵ���rc�ļ���
void render()
{
	cleardevice();	// ����
	putimage(0, 0, background);	// ����ͼ����
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		// ����Ļ����͸����ʽ����ָ��ͼ�����map[i][j]==0��û�ж�Ӧ��ͼ��������������
		putimage_transparent(NULL, img[map[i][j]], COLOR_BLOCK_PIXELS * i + MAP_X, COLOR_BLOCK_PIXELS * j + MAP_Y, BLACK);
	putimage_transparent(NULL, leaveChannel, 25, 360, BLACK);	// ��ʾ�˳���Ϸͨ��
	displayInfo();	// ��ʾ��Ϸ��Ϣ
}

// ���������ɫ��
void fill()
{
	// �ӵ�ͼ�����濪ʼ���������μ��һ��
	for (int j = MAP_SIZE - 1; j >= 0; --j)
	{
		for (int i = MAP_SIZE - 1; i >= 0; --i)
		{
			while (map[i][j] == 0)	// �жϵ�ǰλ���Ƿ�Ϊ��
			{
				delay_jfps(10);
				// ɫ�����µ�
				for (int k = j; k > 0; --k)
					map[i][k] = map[i][k - 1];
				map[i][0] = random(COLOR_BLOCKS_NUMBER) + 1;	// ������һ�еĿ�λ���������ɫ��
				render();	// ÿ��ɫ��仯�󣬻��Ƴ���ǰ��ͼ
			}
		}
	}
}

// �������Ƿ�λ�ھ��ο���
inline int contain(int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY)
{
	return dstX > srcX && dstX < srcX + srcWidth && dstY > srcY && dstY < srcY + srcHeight;
}

// �����λ��ת��Ϊɫ������±�
inline int positionToIndex(int position, int startingPosition)
{
	return (position - startingPosition) / COLOR_BLOCK_PIXELS;
}

// ��ҿ��Ʋ���
int play()
{
	static int x1, y1, x2, y2;
	// δ���������������������Ϣ
	if (!mousemsg() || msg1.x != -COLOR_BLOCK_PIXELS && msg2.x != -COLOR_BLOCK_PIXELS)
		return 0;
	msg = getmouse();	// ��ȡ�����Ϣ
	// �������
	if (!(msg.is_down() && msg.is_left()))	// ����������
		return 0;
	if (contain(LEAVE_X, LEAVE_Y, LEAVE_PIXELS, LEAVE_PIXELS, msg.x, msg.y))	// ����˳���Ϸ����
		return ESC;	// �����˳���Ϸ��Ϣ
	if (!contain(MAP_X, MAP_Y, MAP_PIXELS, MAP_PIXELS, msg.x, msg.y))	// ����ɫ��������
		return -1;
	++clicksNumber;	// �����������һ
	if (clicksNumber == 1)	// ��һ�ε��
	{
		// �����������ɫ���Ľǵ�����
		x1 = msg.x - (msg.x - MAP_X) % COLOR_BLOCK_PIXELS;
		y1 = msg.y - (msg.y - MAP_Y) % COLOR_BLOCK_PIXELS;
		x2 = x1 + COLOR_BLOCK_PIXELS;
		y2 = y1 + COLOR_BLOCK_PIXELS;
		rectangle(x1, y1, x2, y2);	// ���ƿ��ľ��Σ���ʾ�û���һ�ε����ɫ��
		delay_jfps(300);
		msg1 = msg;
		return 1;	// ���أ��ȴ��û���һ�ε��ɫ��
	}
	else	// �ڶ��ε��
	{
		msg2 = msg;
		clicksNumber = 0;	// �����������ε���������ָ���ʼ״̬
		render();	// ���ƽ���
	}
	// ��������ӳ���ɫ������±�
	x1 = positionToIndex(msg1.x, MAP_X);
	y1 = positionToIndex(msg1.y, MAP_Y);
	x2 = positionToIndex(msg2.x, MAP_X);
	y2 = positionToIndex(msg2.y, MAP_Y);
	initMouseMsg();	// �ﵽһ�ν����������������Ϣλ�ûָ���ԭʼ״̬
	// �������Ҿ������ڣ�ֱ���˳�
	if (!adjacent(x1, y1, x2, y2))
		return -1;
	swap(&map[x1][y1], &map[x2][y2]);	// ������ɫ��λ��
	render();	// ����ɫ��λ�ý�����ĵ�ͼ
	if (count() == 0)	// ����֮�󣬷���0˵��û������ɫ��
	{
		delay_ms(100);
		swap(&map[x1][y1], &map[x2][y2]);	// �ٴν�����ȥ
		render();	// ���һ��Ƴ�������ĵ�ͼ
	}
	return 2;
}