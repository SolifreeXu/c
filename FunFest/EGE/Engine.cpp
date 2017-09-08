#include<stdio.h>
#include<graphics.h>	//EGEͼ�ο����ͷ�ļ�
#include"FunFest.h"

PIMAGE img[COLOR_BLOCKS_NUMBER + 1];	//ͼƬָ�����飬��ż���ͼƬ�ĵ�ַ
PIMAGE background;	//��Ϸ����ͼƬָ��
PIMAGE scoreZone;	//��������ͼƬָ��
PIMAGE timeZone;	//ʱ�䱳��ͼƬָ��
PIMAGE leave;	//�˳�ͨ��ͼƬָ��
char strBuffer[_MAX_PATH];	//��Ϸ�ַ���ר�û�����
static mouse_msg msg, msg1, msg2;	//mouse_msg�����Ϣ�ṹ�����
static int clicksNumber;	//���������������ɫ�����������

//��ʼ����Ϸ����
void initEngine()
{
	srand((unsigned)time(NULL));	//�������������
	score = 0;	//�÷�����
	timeLeft = INITIAL_TIME;	//��ʼʣ��ʱ��
	//�����鸳ֵ����ʼ����ͼ״̬
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		map[i][j] = rand() % COLOR_BLOCKS_NUMBER + 1;	//rand��ȡ�������ȡ�õ�����int��
	//�����Ϣλ�ó�ʼ��Ϊ-COLOR_BLOCK_PIXELS�������ж��Ƿ������Ч�����Ϣ
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;
	clicksNumber = 0;	//�����������ʼ��
}

//��ʾ��Ϸ��Ϣ
void displayInfo()
{
	//��ӡ����
	putimage_transparent(NULL, scoreZone, 25, 30, BLACK);
	rectprintf(47, 60, 54, 40, "%3u", score);
	//��ӡʱ��
	putimage_transparent(NULL, timeZone, 25, 190, BLACK);
	rectprintf(58, 220, 36, 40, "%2u", timeLeft);
}

//��ͼ����Դ�ɵ���rc�ļ���
void draw()
{
	cleardevice();	//����
	putimage(0, 0, background);	//����ͼ����
	for (int i = 0; i < MAP_SIZE; ++i)
	for (int j = 0; j < MAP_SIZE; ++j)
		//����Ļ����͸����ʽ����ָ��ͼ�����map[i][j]==0��û�ж�Ӧ��ͼ��������������
		putimage_transparent(NULL, img[map[i][j]], COLOR_BLOCK_PIXELS * i + MAP_X, COLOR_BLOCK_PIXELS * j + MAP_Y, BLACK);
	putimage_transparent(NULL, leave, 25, 360, BLACK);	//��ʾ�˳���Ϸͨ��
	displayInfo();	//��ʾ��Ϸ��Ϣ
	delay_jfps(10);	//ƽ���ӳ�1000/fps���룬�����ȶ��߼�֡�ʿ��ƣ���ͼ����֡������fps��10
}

//���������ɫ��
void fill()
{
	//�ӵ�ͼ�����濪ʼ���������μ��һ��
	for (int j = MAP_SIZE - 1; j >= 0; --j)
	{
		for (int i = MAP_SIZE - 1; i >= 0; --i)
		{
			while (map[i][j] == 0)	//�жϵ�ǰλ���Ƿ�Ϊ��
			{
				//ɫ�����µ�
				for (int k = j; k > 0; --k)
					map[i][k] = map[i][k - 1];
				map[i][0] = rand() % COLOR_BLOCKS_NUMBER + 1;	//������һ�еĿ�λ���������ɫ��
				draw();	//ÿ��ɫ��仯�󣬻��Ƴ���ǰ��ͼ
			}
		}
	}
}

//��ҿ��Ʋ���
int play()
{
	static int x1, y1, x2, y2;
	//δ���������ε�������������Ϣ
	if ((msg1.x == -COLOR_BLOCK_PIXELS || msg2.x == -COLOR_BLOCK_PIXELS) && mousemsg())
	{
		msg = getmouse();	//��ȡ�����Ϣ

		//�϶�����������λ�ã����������¡��������
		//if (msg.is_left() && msg.is_down())
		//{
		//	if (msg.x > 25 && msg.x<125 && msg.y>360 && msg.y < 460)	//������˳���Ϸ����
		//		return ESC;	//�����˳���Ϸ��Ϣ
		//	msg1 = msg;	//���������Ϣ��msg1
		//	return 1;
		//}
		//else if (msg.is_left() && msg.is_up())
		//	msg2 = msg;	//������𣬸�msg2

		//�������
		if (msg.is_left() && msg.is_down())	//����������
		{
			++clicksNumber;	//�����������һ
			if (msg.x > 25 && msg.x<125 && msg.y>360 && msg.y < 460)	//������˳���Ϸ����
				return ESC;	//�����˳���Ϸ��Ϣ
			if (clicksNumber == 1)	//��һ�ε��
			{
				//�����������ɫ���Ľǵ�����
				x1 = (msg.x - MAP_X) / COLOR_BLOCK_PIXELS * COLOR_BLOCK_PIXELS + MAP_X;
				y1 = (msg.y - MAP_Y) / COLOR_BLOCK_PIXELS * COLOR_BLOCK_PIXELS + MAP_Y;
				x2 = x1 + COLOR_BLOCK_PIXELS;
				y2 = y1 + COLOR_BLOCK_PIXELS;
				rectangle(x1, y1, x2, y2);	//���ƿ��ľ��Σ���ʾ�û���һ�ε����ɫ��
				msg1 = msg;
				return 1;	//���أ��ȴ��û���һ�ε��ɫ��
			}
			else	//�ڶ��ε��
			{
				msg2 = msg;
				clicksNumber = 0;	//�����������ε���������ָ���ʼ״̬
				draw();	//���ƽ���
			}
		}
		else
			return 0;
	}
	else
		return 0;
	//��ɽ���������ɫ������ӳ��
	x1 = (msg1.x - MAP_X) / COLOR_BLOCK_PIXELS;
	y1 = (msg1.y - MAP_Y) / COLOR_BLOCK_PIXELS;
	x2 = (msg2.x - MAP_X) / COLOR_BLOCK_PIXELS;
	y2 = (msg2.y - MAP_Y) / COLOR_BLOCK_PIXELS;
	msg1.x = msg1.y = msg2.x = msg2.y = -COLOR_BLOCK_PIXELS;	//�ﵽһ�ν��������������λ����Ϣ�ָ���ԭʼ״̬
	//����ɫ�������޻����������Ҿ������ڣ�ֱ���˳�
	if (x1 > 8 || y1 > 8 || x2 > 8 || y2 > 8\
		|| !(abs((x1 + y1) - (x2 + y2)) == 1 && (x1 == x2 || y1 == y2)))
		return -1;
	swap(&map[x1][y1], &map[x2][y2]);	//������ɫ��λ��
	draw();	//����ɫ��λ�ý�����ĵ�ͼ
	if (clear() == 0)	//����֮��clear����0��ʾû������
	{
		swap(&map[x1][y1], &map[x2][y2]);	//�ٴν�����ȥ
		draw();	//���һ��Ƴ�������ĵ�ͼ
	}
	else	//������0���������Ĳ���
		fill();	//�����ȥ�Ĳ���
	return 2;
}

//��Ϸ��ֻ���
void endInterface()
{
	_snprintf(strBuffer, _MAX_PATH, "The final score is %d", score);	//�����յ÷���ʾд�뻺����
	int len = strlen(strBuffer) * 18;	//������ʾ��Ϣ����
	setfillcolor(WHITE);	//���õ�ǰ�����ɫ
	bar(0, (WINDOW_HEIGHT - 50) / 2, WINDOW_WIDTH, (WINDOW_HEIGHT - 50) / 2 + 50);	//���ƾ���
	rectprintf((WINDOW_WIDTH - len) / 2, (WINDOW_HEIGHT - 36) / 2, len, 36, strBuffer);	//������յ÷���ʾ
}