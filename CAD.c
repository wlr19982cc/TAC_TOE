#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include "linkedlist.h"
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#define TYPE          4    //共需要绘制四种图形 
#define LINE          0    // 代表直线 
#define RECT          1    //代表椭圆 
#define ELLIPSE       2    //代表椭圆 
#define TEXTB         3    //代表文本 
#define TEXTLEN       100  //文本长度 
#define DRAWCOLOR    "BLACK"   //绘图的默认颜色 
#define CURSOR        "_"      //光标 
#define TIMER_BLINK   5         //500ms定时器事件标志号 
#define m       GetWindowWidth()    //窗口宽度 
#define n       GetWindowHeight()    //窗口高度 
#define PI    3.1415926
typedef struct {
	double x1, y1;
    double x2, y2;
    int PenSize; 
    string pencolor;
    bool isSelected;  
} *LineA;   //直线结构，结构成员包括 首末点横纵坐标，画笔尺寸，画笔颜色，是否选中 

typedef struct {
	double x1, y1;
    double x2, y2;
    int PenSize; 
    string pencolor; 
    bool isSelected;  
} *RectA;        //矩形结构，结构成员包括 左下、右上的横纵坐标，画笔尺寸，画笔颜色，是否选中 

typedef struct {
    double cx, cy;
    double rx, ry;
    int PenSize; 
    string pencolor; 
    bool isSelected; 
} *EllipseA;   //椭圆结构，结构成员包括 圆心横纵坐标，长短半轴大小，画笔尺寸，画笔颜色，是否选中 

typedef struct {
	string text;
	double x, y;
    int cursor_Pos; 
    int PenSize; 
    string Pointcolor; 
    bool isSelected;  
    bool isBlink; 
    
} *TextA; //文本结构，结构成员包括 文本字符串，文本开始处的横纵坐标，光标位置，画笔尺寸，画笔颜色，是否选中，光标是都闪烁 

void Clear(void);  //清空整个界面 
void DrawBox(double x, double y, double width, double height);  //画矩形 
void DrawTriangle(double x, double y, double base, double height);   //画三角形 
void Drawhouse1();   //画第一个房子 
void Drawhouse2();   //画第二个房子 
void KeyboardEventProcess(int key,int event);   //键盘消息回调函数
void CharEventProcess(char c);  //字符消息回调函数
void MouseEventProcess(int x, int y, int button, int event);   //鼠标消息回调函数
void TimerEventProcess(int timerID);   //定时器消息回调函数
void DrawLineA(void *line);   //画一条直线 
bool LineEqual(void *line1, void *line2);  //比较两条直线是否相同 
double distance_Line(double x, double y, LineA line);  //计算鼠标所在位置与直线中心的距离 
LineA SelectNearestLine(linkedlistADT list, double mx, double my);   //求出与鼠标所在位置距离最近的直线，并返回其指针    
void DrawRectT(void *rect);  //画一个矩形 
bool RectEqual(void *rect1, void *rect2);//比较两个矩形是否相同 
double distance_Rect(double x, double y, RectA rect);//计算鼠标所在位置与矩形中心的距离 
RectA SelectNearestRect(linkedlistADT list, double mx, double my); //求出与鼠标所在位置距离最近的矩形，并返回其指针   
void DrawEllipseA(void *ellipse); //画一个椭圆 
bool EllipseEqual(void *ellipse1, void *ellipse2);//比较两个椭圆是否相同 
double distance_Ellipse(double x, double y, EllipseA ellipse);//计算鼠标所在位置与椭圆中心的距离 
EllipseA SelectNearestEllipse(linkedlistADT list, double mx, double my); //求出与鼠标所在位置距离最近的椭圆，并返回其指针   
void DrawTextT(void *text); //显示文本 
void DrawCursorA(string str, int cursor_Pos, double startx, double starty); //显示光标 
bool TextEqual(void *text1, void *text2);//比较两个文本内容、位置是否相同 
double distance_Text(double x, double y, TextA text);//计算鼠标所在位置与文本中心的距离 
TextA SelectNearestText(linkedlistADT list, double mx, double my);//求出与鼠标所在位置距离最近的文本，并返回其指针   
void PickNearest(linkedlistADT list[], double mx, double my); //求出与鼠标所在位置距离最近的图形，并返回其指针   
void Paint_List();   //根据存储的指针画出所有图形 
void start(); //说明使用方法的初始界面 
void InsertCharToString(string str, int pos, char c); //插入一个字符到字符串末尾 
void DrawSnowLine(void *angle);   //绘制雪花图形的边长 
void Divide_Line(linkedlistADT linkedlist);  //按照角度将雪花的每条边分成四条 
void KochSnow(linkedlistADT linkedlist, int abc);   //利用递归的方法分形 
void Draw_snow(void);


int ll; //一个从控制台过度到图形界面的变量（无实际作用，可以忽略） 
double length;   //雪花图形的边长 
double angle_all;
LineA Line_current = NULL;  //直线链表的当前对象指针
RectA Rect_current = NULL;  //矩形链表的当前对象指针
EllipseA Ellipse_current = NULL;  //椭圆链表的当前对象指针
TextA Text_current = NULL;  //文本链表的当前对象指针
const int mseconds500 = 500;   //光标闪烁时间间隔 
linkedlistADT shape[TYPE]={NULL, NULL, NULL, NULL};   //存储四类图形元素链表指针
double Distance_min[TYPE];  //存储四类图形最短距离的数组 
linkedlistADT SnowList; //链表各结点依次存放各线段的角度
char textbuf[TEXTLEN];    //当前文本缓冲区
TextA text_pre;
int  List_current= LINE;    //当前选中的图形链表的序号
bool isCursorBlink = FALSE;  //光标是否闪烁 
bool isSelected = FALSE; //图形元素是否选中
bool inText = FALSE; //是否处于文本编辑状态
bool isshow = FALSE;   //是否处于展示绘制的房子图形的界面 


void Main() 
{
	int i;
    InitGraphics();   //图形初始化
	Randomize();    //随机函数初始化
	registerKeyboardEvent(KeyboardEventProcess);   //注册键盘消息回调函数
	registerCharEvent(CharEventProcess);   //注册字符消息回调函数
	registerMouseEvent(MouseEventProcess);   //注册鼠标消息回调函数
	registerTimerEvent(TimerEventProcess);   //注册定时器消息回调函数
	
	InitConsole();     //控制台窗口初始化	
	start();         
	FreeConsole();    //关闭控制台窗口 
	
	SetPenColor(DRAWCOLOR); 
    SetPenSize(1);

	for (i = 0; i < TYPE; i++) shape[i] = NewLinkedList();    //新建四个链表并存储在链表指针数组中 
}


void Clear(void)
{
	MovePen(0, 0);
	SetPenColor("White");
	StartFilledRegion(1.0);
	DrawLine(0, n);
	DrawLine(m, 0);
	DrawLine(0, -n);
	DrawLine(-m, 0);
	EndFilledRegion();
	SetPenColor("Black");
}


void DrawTriangle(double x, double y, double base, double height)
{
    MovePen(x, y);
    DrawLine(base, 0);
    DrawLine(-base / 2, height);
    DrawLine(-base / 2, -height);
}


void DrawBox(double x, double y, double width, double height)
{
    MovePen(x, y);
    turn(90);
    forward(height);
    turn(-90);
    forward(width);
    turn(-90);
    forward(height);
    turn(-90);
    forward(width);
    angle_all=0;
}


void Drawhouse1()
{
    DrawBox(1,2,1,1.2);
    DrawBox(1.2,2.7,0.2,0.2);
    DrawBox(1.6,2,0.2,0.9);
    DrawTriangle(1,3.2,1,1.0);
    MovePen(1.1, 3.4);
    DrawLine(0, 0.5);
    DrawLine(0.15, 0);
    DrawLine(0, -0.2);
    DrawBox(1.07,3.9,0.21,0.12);

}


void Drawhouse2(){

	DrawBox(6,2,0.5,0.6);
    DrawBox(6.5,2,0.5,0.6);
    DrawBox(7,2,0.5,0.6);
    DrawBox(7.5,2,0.5,0.6);
    DrawBox(6,2.6,0.5,0.6);
    DrawBox(6.5,2.6,0.5,0.6);
    DrawBox(7,2.6,0.5,0.6);
    DrawBox(7.5,2.6,0.5,0.6);
	DrawTriangle(6,3.2,0.5,0.5);
	DrawTriangle(7.5,3.2,0.5,0.5);
	DrawBox(6.5,3.2,0.085,0.085);
    DrawBox(6.67,3.2,0.085,0.085);
    DrawBox(6.84,3.2,0.085,0.085);
    DrawBox(7.01,3.2,0.085,0.085);
    DrawBox(7.18,3.2,0.085,0.085);
    DrawBox(7.35,3.2,0.085,0.085);
}



void DrawSnowLine(void *angle)
{
	double radians;
	radians = *((double *)angle) / 180 * PI;
	DrawLine(length * cos(radians), length* sin(radians));
}

void Divide_Line(linkedlistADT SnowList)
{
	linkedlistADT angle_list;
	double *angle, preangle;

	angle_list = SnowList;  
    angle_list = NextNode(SnowList, angle_list);
	while (angle_list != NULL) {
		preangle = *((double *)NodeObj(SnowList, angle_list));    //该节点中原来存放的角度 
		angle = GetBlock(sizeof(double));
		*angle = preangle - 60;
        InsertNode(SnowList, angle_list, (void *)angle);     //将分形产生新边的角度存放于下一节点 
		angle_list = NextNode(SnowList, angle_list);         //跳到下一节点 
		angle = GetBlock(sizeof(double));
		*angle = preangle + 60;                       
        InsertNode(SnowList, angle_list, (void *)angle);    //将分形产生新边的角度存放于下一节点 
		angle_list = NextNode(SnowList, angle_list);         //跳到下一节点    
		angle = GetBlock(sizeof(double));
		*angle = preangle;
        InsertNode(SnowList, angle_list, (void *)angle);    //将分形产生新边的角度存放于下一节点 
		angle_list = NextNode(SnowList, angle_list);        //跳到下一节点   
		angle_list = NextNode(SnowList, angle_list);       //跳到下一节点      
	}
}


void KochSnow(linkedlistADT SnowList, int abc)
{
	double *angle;

	if (abc == 0) { //1阶雪花三条边的角度（从底边开始） 
		angle = GetBlock(sizeof(double));
		*angle = 0;
        InsertNode(SnowList, NULL, (void *)angle);
		angle = GetBlock(sizeof(double));
		*angle = 120;
        InsertNode(SnowList, NULL, (void *)angle);
		angle = GetBlock(sizeof(double));
		*angle = 240;
        InsertNode(SnowList, NULL, (void *)angle);
	} else {
		KochSnow(SnowList, abc-1);  //递归分形 
        Divide_Line(SnowList);   //将每一条边分别变成四条边 
	}
}


void Draw_snow() 
{
	int abc;
	SetPenColor(DRAWCOLOR); 
    SetPenSize(1);      
	InitConsole();
	printf("Enter n: "); //输入阶数
	abc = GetInteger();
	printf("Enter length: ");  //输入初始边长
	length = GetReal();
	FreeConsole();
	MovePen(m/2.0 - length/2.0,n/2 - length/2.0*tan(30/180.0*PI));  //为了使图形处于正中间更加美观，故设置初始画笔在图形左下角处 
	length = length / pow(3.0, (double)abc); 
	SnowList = NewLinkedList(); 
	KochSnow(SnowList, abc); 
    TraverseLinkedList(SnowList, DrawSnowLine); //便利整个链表，绘制出每一条边 
    FreeLinkedList(SnowList);

}


void DrawLineA(void *line)
{
	LineA line_pre = (LineA)line;
	int pensize = GetPenSize();    
	string pencolor = GetPenColor();
	SetPenSize(line_pre->PenSize);
	SetPenColor(line_pre->pencolor);
	MovePen(line_pre->x1, line_pre->y1);
	DrawLine(line_pre->x2-line_pre->x1,line_pre->y2-line_pre->y1);
	SetPenSize(pensize); 
	SetPenColor(pencolor);
}


void DrawRectA(void *rect)
{
	RectA rect_pre = (RectA)rect;
	int pensize = GetPenSize();
	string pencolor = GetPenColor();
	SetPenSize(rect_pre->PenSize);
	SetPenColor(rect_pre->pencolor);
	MovePen(rect_pre->x1, rect_pre->y1);
	DrawLine(rect_pre->x2-rect_pre->x1, 0);
	DrawLine(0, rect_pre->y2-rect_pre->y1);
	DrawLine(rect_pre->x1-rect_pre->x2, 0);
	DrawLine(0, rect_pre->y1-rect_pre->y2);
	SetPenSize(pensize); 
	SetPenColor(pencolor);
}


void DrawTextT(void *text)
{
	TextA text_pre = (TextA)text;
	string pencolor = GetPenColor();
	int pointsize = GetPointSize();
	MovePen(text_pre->x, text_pre->y);
	SetPenColor(text_pre->Pointcolor);
	SetPointSize(text_pre->PenSize);
    DrawTextString(text_pre->text);
    SetPenColor(pencolor);
    SetPointSize(pointsize);
}


void DrawCursorA(string str, int cursor_Pos, double startx, double starty)
{
	MovePen(startx+TextStringWidth(SubString(str, 0, cursor_Pos-1)), starty);
	DrawTextString(CURSOR);
	return;
}


void DrawEllipseA(void *ellipse)
{
	EllipseA ellipse_pre = (EllipseA)ellipse;
	int pensize = GetPenSize();
	string pencolor = GetPenColor();
	SetPenSize(ellipse_pre->PenSize);
	SetPenColor(ellipse_pre->pencolor);
	MovePen(ellipse_pre->cx + ellipse_pre->rx, ellipse_pre->cy);
    DrawEllipticalArc(ellipse_pre->rx, ellipse_pre->ry, 0.0, 360.0);
    SetPenSize(pensize);
    SetPenColor(pencolor);
}


bool LineEqual(void *line1, void *line2)
{
	LineA line_pre1 = (LineA)line1, line_pre2 = (LineA)line2;
	if(line_pre1->x1 == line_pre2->x1 && line_pre1->y1 == line_pre2->y1 &&line_pre1->x2 == line_pre2->x2 && line_pre1->y2 == line_pre2->y2)
	   return 1;
	return 0;
}


bool RectEqual(void *rect1, void *rect2)
{
	RectA rect_pre1, rect_pre2;
	rect_pre1 = (RectA)rect1;
	rect_pre2 = (RectA)rect2;
	if(rect_pre1->x1 == rect_pre2->x1 && rect_pre1->y1 == rect_pre2->y1 &&rect_pre1->x2 == rect_pre2->x2 && rect_pre1->y2 == rect_pre2->y2)
	   return 1;
	return 0;
}


bool EllipseEqual(void *ellipse1, void *ellipse2)
{
	EllipseA ellipse_pre1 = (EllipseA)ellipse1, ellipse_pre2 = (EllipseA)ellipse2;
	return ellipse_pre1->cx == ellipse_pre2->cx && ellipse_pre1->cy == ellipse_pre2->cy &&ellipse_pre1->rx == ellipse_pre2->rx && ellipse_pre1->ry == ellipse_pre2->ry;

}


bool TextEqual(void *text1, void *text2)
{
	TextA text_pre1 = (TextA)text1, text_pre2 = (TextA)text2;
	if(StringEqual(text_pre1->text, text_pre2->text) &&text_pre1->x == text_pre2->x && text_pre1->y == text_pre2->y)
        return 1;
    return 0;
}


double distance_Line(double x, double y, LineA line)
{
	double x0, y0;
	x0 = (line->x1 + line->x2)/2;
	y0 = (line->y1 + line->y2)/2;
	return fabs(x-x0) + fabs(y-y0);
}


double distance_Rect(double x, double y, RectA rect)
{
	double x0, y0;
	x0 = (rect->x1 + rect->x2)/2;
	y0 = (rect->y1 + rect->y2)/2;
	return fabs(x-x0)+fabs(y-y0);
}


double distance_Ellipse(double x, double y, EllipseA ellipse)
{
	return fabs(x-ellipse->cx) + fabs(y-ellipse->cy);
}


double distance_Text(double x, double y, TextA text)
{
	return fabs(x - (text->x)/2) + fabs(y - (text->y)/2);
}


LineA SelectNearestLine(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearest = NULL, ptr;
	double distance_min, distance;
	ptr = NextNode(list, list);
	if (ptr == NULL) 
	    return NULL;
    nearest = ptr;
  	distance_min = distance_Line(mx, my, (LineA)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
  	    distance = distance_Line(mx, my, (LineA)NodeObj(list, ptr));
		if (distance < distance_min) {
			nearest = ptr;
			distance_min = distance;
		}
	}
	Distance_min[LINE] = distance_min;   //将最短距离存储在数组中 

	return (LineA)NodeObj(list, nearest);    //返回距离鼠标位置最近的直线的指针 
}


RectA SelectNearestRect(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearest = NULL, ptr;
	double distance_min, distance;
	ptr = NextNode(list, list);
	if (ptr == NULL) 
	   return NULL;
    nearest = ptr;
  	distance_min = distance_Rect(mx, my, (RectA)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
  	    distance = distance_Rect(mx, my, (RectA)NodeObj(list, ptr));
		if (distance < distance_min) {
			nearest = ptr;
			distance_min = distance;
		}
	}
	Distance_min[RECT] = distance_min;    //将最短距离存储在数组中 
	return (RectA)NodeObj(list, nearest);  //返回距离鼠标位置最近的矩形的指针 
}


EllipseA SelectNearestEllipse(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearest = NULL, ptr;
	double distance_min, distance;
	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
    nearest = ptr;
  	distance_min = distance_Ellipse(mx, my, (EllipseA)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
  	    distance = distance_Ellipse(mx, my, (EllipseA)NodeObj(list, ptr));
		if (distance < distance_min) {
			nearest = ptr;
			distance_min = distance;
		}
	}
	Distance_min[ELLIPSE] = distance_min;    //将最短距离存储在数组中 
	return (EllipseA)NodeObj(list, nearest);    //返回距离鼠标位置最近的椭圆的指针 
}


TextA SelectNearestText(linkedlistADT list, double mx, double my)
{
	linkedlistADT nearest = NULL, ptr;
	double distance_min, distance;
	ptr = NextNode(list, list);
	if (ptr == NULL) return NULL;
    nearest = ptr;
  	distance_min = distance_Text(mx, my, (TextA)NodeObj(list, ptr));
	while (NextNode(list, ptr) != NULL) {
		ptr = NextNode(list, ptr);
  	    distance = distance_Text(mx, my, (TextA)NodeObj(list, ptr));
		if (distance < distance_min) {
			nearest = ptr;
			distance_min = distance;
		}
	}
	Distance_min[TEXTB] = distance_min;  //将最短距离存储在数组中 
	return (TextA)NodeObj(list, nearest);  //返回距离鼠标位置最近的文本的指针 
}


void InsertCharToString(string str, int pos, char c)
{
	int len;
	if (pos < 0 || pos >= TEXTLEN) 
	  return;
	len = strlen(str);
	*(str+len+1) = '\0';
	*(str+len) = c;
	return;
}


void PickNearest(linkedlistADT list[], double mx, double my)
{
	int i, list_min;
	for(i=0;i<TYPE;i++){
        Distance_min[i]=1000000;
    } 
	Line_current = SelectNearestLine(list[LINE], mx, my);
	Rect_current = SelectNearestRect(list[RECT], mx, my);
    Ellipse_current = SelectNearestEllipse(list[ELLIPSE], mx, my);
	Text_current = SelectNearestText(list[TEXTB], mx, my);
	list_min = LINE;
	for (i = 1; i < TYPE; i++) {
		if (Distance_min[i] < Distance_min[list_min]) list_min = i;
	}
	List_current = list_min;   //记录距离鼠标位置最近的图形的序号  
}


void Paint_List()
{
    TraverseLinkedList(shape[LINE], DrawLineA);
    TraverseLinkedList(shape[RECT], DrawRectA);
    TraverseLinkedList(shape[ELLIPSE], DrawEllipseA);
    TraverseLinkedList(shape[TEXTB], DrawTextT);
}


void start()
{
	printf("\t╔═══════════════════════════════╗\n"); 
	printf("\t║                                                              ║\n");
	printf("\t║                                                              ║\n");
    printf("\t║                 ._______________________.                    ║\n");
    printf("\t║                 | _____________________ |                    ║\n"); 
    printf("\t║                 | I                   I |                    ║\n"); 
    printf("\t║                 | I欢迎使用CAD绘图程序I |                    ║\n"); 
    printf("\t║                 | I                   I |                    ║\n"); 
    printf("\t║                 | I   made by   DAC     |                    ║\n"); 
    printf("\t║                 | I___________________I |                    ║\n"); 
    printf("\t║                 !_______________________!                    ║\n"); 
    printf("\t║                     ._[__________]_.                         ║\n"); 
    printf("\t║                 .___|_______________|___.                    ║\n"); 
    printf("\t║                  |::: ____             |                     ║\n");  
    printf("\t║                  |    ~~~~ [CD-ROM]    |                     ║\n"); 
    printf("\t║                  !_____________________!                     ║\n");
	printf("\t║                                                              ║\n"); 
	printf("\t║                                                              ║\n");
    printf("\t║                                                              ║\n");                                    
    printf("\t║使用说明：F1-绘制直线 F2-绘制矩形 F3-绘制椭圆 F4-编辑新字符串 ║\n"); 
    printf("\t║F5-编辑已存在的字符串  F5-显示房子图形 F6-回到CAD绘图界面     ║\n"); 
    printf("\t║Esc和return-退出字符串的编辑  delete-删除字符、字符换或图形   ║\n"); 
    printf("\t║鼠标左键单击移动-图形的移动   鼠标右键单击移动-图形的拉伸     ║\n");
	printf("\t║     输入任意数字并回车进入绘图界面："); 
    scanf("%d",&ll); 

}


void TimerEventProcess(int timerID)
{
	switch (timerID) {
		case TIMER_BLINK: 
			SetEraseMode(!text_pre->isBlink);
			DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);
			SetEraseMode(FALSE);
			text_pre->isBlink=!text_pre->isBlink;
			break;
	    default:
			break;
	}
}


void CharEventProcess(char c)
{
  int len;
  if(!isshow){
	if (!inText) return;
      switch (c) {
    	case 27: 
    	case '\r':  
			inText = FALSE;   //退出当前文本编辑状态 
           	SetEraseMode(TRUE);   //开始使用橡皮擦 
	 	   	MovePen(text_pre->x, text_pre->y);
	 	   	DrawTextString(textbuf);   //擦去文本 
			DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);   //擦去光标 
			Paint_List();   //擦去所有图形 
			text_pre->isBlink = FALSE;  //光标不再闪烁 
			text_pre->text = CopyString(textbuf);   //将文本缓冲区的内容存入结构中 
           	InsertNode(shape[TEXTB], NULL, text_pre);   //将给结构存入链接指针数组中 
           	SetEraseMode(FALSE);   //关闭橡皮擦 
			Paint_List();   //重新绘制所有图形 
			cancelTimer(TIMER_BLINK);   //注销光标闪烁定时器
			isCursorBlink = FALSE;   //关闭光标闪烁 
			SetPenColor(DRAWCOLOR);   //画笔恢复默认颜色 
			break;
    	default:
    		if ((len = strlen(textbuf)) >= TEXTLEN) break; 
			SetEraseMode(TRUE);
			MovePen(text_pre->x, text_pre->y);
			DrawTextString(textbuf);   
			DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);/*擦除当前光标*/
			InsertCharToString(textbuf, text_pre->cursor_Pos, c);   //将当前字符插入到字符串末尾光标位置处 
	 	   	SetEraseMode(FALSE);
	 	   	MovePen(text_pre->x, text_pre->y);
	 	   	DrawTextString(textbuf);   //重新显示新的字符串 
	 	   	(text_pre->cursor_Pos)++;   //光标位置后移一位 
	 	   	DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);   //画出当前光标
			break;
      }
  }
}


void KeyboardEventProcess(int key,int event)
{
	 LineA line_pre;
	 RectA rect_pre;
	 EllipseA ellipse_pre;
	 double x1, y1, x2, y2;

     switch (event) {
	 	case KEY_DOWN:
			 switch (key) {
			     case VK_F1:if(!isshow){ //随机生成直线
					 line_pre = GetBlock(sizeof(*line_pre));
					 line_pre->x1 = RandomReal(0.0, GetWindowWidth());
					 line_pre->y1 = RandomReal(0.0, GetWindowHeight());
					 line_pre->x2 = RandomReal(0.0, GetWindowWidth());
					 line_pre->y2 = RandomReal(0.0, GetWindowHeight());
					 line_pre->PenSize = GetPenSize();
					 line_pre->pencolor = GetPenColor();
					 line_pre->isSelected = FALSE;
					 InsertNode(shape[LINE], NULL, line_pre);
					 DrawLineA(line_pre);
				}
                     break;
			     case VK_F2:if(!isshow){//随机生成矩形
					 rect_pre = GetBlock(sizeof(*rect_pre));
					 x1 = RandomReal(0.0, GetWindowWidth());
					 y1 = RandomReal(0.0, GetWindowHeight());
					 x2 = RandomReal(0.0, GetWindowWidth());
					 y2 = RandomReal(0.0, GetWindowHeight());
					 if (x1 < x2) {
                         rect_pre->x1 = x1;
                         rect_pre->x2 = x2;
					 } 
					 else {
                         rect_pre->x1 = x2;
                         rect_pre->x2 = x1;
					 }
					 if (y1 < y2) {
                         rect_pre->y1 = y1;
                         rect_pre->y2 = y2;
					 } 
					 else {
                         rect_pre->y1 = y2;
                         rect_pre->y2 = y1;
					 }
					 rect_pre->PenSize = GetPenSize();
					 rect_pre->pencolor = GetPenColor();
					 rect_pre->isSelected = FALSE;
					 InsertNode(shape[RECT], NULL, rect_pre);
					 DrawRectA(rect_pre);
				}
                     break;
			     case VK_F3:if(!isshow){//随机生成椭圆
					 ellipse_pre = GetBlock(sizeof(*ellipse_pre));
					 ellipse_pre->cx = RandomReal(2.0, GetWindowWidth()-2.0);
					 ellipse_pre->cy = RandomReal(2.0, GetWindowHeight()-2.0);
					 ellipse_pre->rx = RandomReal(0.5, 2.0);
					 ellipse_pre->ry = RandomReal(0.5, 2.0);
					 ellipse_pre->PenSize = GetPenSize();
					 ellipse_pre->pencolor = GetPenColor();
					 ellipse_pre->isSelected = FALSE;
					 InsertNode(shape[ELLIPSE], NULL, ellipse_pre);
					 DrawEllipseA(ellipse_pre);
				}
                     break;
			     case VK_F4:if(!isshow){//随机生成空的文本对象
			         if (inText) break;//若已处于文本编辑状态，则不生成新的空文本
					 inText = TRUE; //进入新的文本行编辑状态
					 text_pre = GetBlock(sizeof(*text_pre));
					 text_pre->x = RandomReal(1.0, GetWindowWidth()-1.0);
                     text_pre->y = RandomReal(1.0, GetWindowHeight()-1.0);
					 MovePen(text_pre->x, text_pre->y);//画笔移到文本显示起始位置
	 	   			 DrawTextString(CURSOR);
                     textbuf[0] = '\0'; //成字符串
                     text_pre->PenSize = GetPointSize();
                     text_pre->Pointcolor = GetPenColor();
                     text_pre->isSelected = FALSE;
                     text_pre->cursor_Pos = 0; //设置当前光标位置
                     text_pre->isBlink = TRUE; //打开光标闪烁功能 
                     startTimer(TIMER_BLINK, mseconds500);   //光标闪烁定时器触发
                     isCursorBlink = TRUE;   //设置光标闪烁
				}
                     break;              
                 case VK_F5:if(!isshow){  //编辑已有的文本 
					if (!isSelected) break;   //当前文本未被选中 
					if (inText) break;    //当前不处于文本行编辑状态 
					inText = TRUE;  //进入文本编辑状态
					text_pre = Text_current;  //选取当前编辑的文本对象
					strcpy(textbuf, text_pre->text); 
					DeleteNode(shape[TEXTB], Text_current, TextEqual);   //从当前文本链表中删除该文本对象
					SetPenColor(text_pre->Pointcolor); 
					DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);
					text_pre->isBlink = TRUE; 
            		startTimer(TIMER_BLINK, mseconds500);
            		isCursorBlink = TRUE; 
				}
				     break;
			     case VK_DELETE:
				    if (inText) { 
				        if(text_pre->cursor_Pos>=1){//在文本编辑状态中删除字符串末尾的字符 
					    	SetEraseMode(TRUE);
					    	MovePen(text_pre->x, text_pre->y);
					    	DrawTextString(textbuf);
					    	DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);
					    	(text_pre->cursor_Pos)--;
						    *(textbuf+(text_pre->cursor_Pos)) = '\0';
				 	   	    SetEraseMode(FALSE);
				 	     	Paint_List();
				 	     	MovePen(text_pre->x, text_pre->y);
				 	    	DrawTextString(textbuf);
				 	    	DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);				    	
				      }
				 	    break;
				    }
				    if (isSelected){//删除选中的图形 
			  	    	SetEraseMode(TRUE);
               	    	Paint_List();
		         	    switch (List_current) {
		         	  	    case LINE:
					 	    	DeleteNode(shape[LINE], Line_current, LineEqual);
		         	  	    	break;
		         	      	case RECT:
		         	  	    	DeleteNode(shape[RECT], Rect_current, RectEqual);
		         	  	    	break;
		         	    	case ELLIPSE:
					 	    	DeleteNode(shape[ELLIPSE], Ellipse_current, EllipseEqual);
		         	  		    break;
		         	    	case TEXTB:
		         	  	    	DeleteNode(shape[TEXTB], Text_current, TextEqual);
		         	  	    	break;
		               	}
				        SetEraseMode(FALSE);
		            	Paint_List();
		            	isSelected = FALSE;
		            	break;
		            }
                    
                 
                case VK_ESCAPE:  //退出图形元素选中状态
                	  if (!isSelected) break;
		         	  switch (List_current) {
		         	  	case LINE:
		         	  		Line_current->isSelected = FALSE;
		         	  		Line_current->pencolor = CopyString("BLACK");
		         	  		break;
		         	  	case RECT:
		         	  		Rect_current->isSelected = FALSE;
		         	  		Rect_current->pencolor = CopyString("BLACK");
		         	  		break;
		         	  	case ELLIPSE:
		         	  		Ellipse_current->isSelected = FALSE;
		         	  		Ellipse_current->pencolor = CopyString("BLACK");
		         	  		break;
		         	  	case TEXTB:
		         	  		Text_current->isSelected = FALSE;
		         	  		Text_current->Pointcolor = CopyString("BLACK");
		         	  		break;
		         	  }
		         	  isSelected = FALSE; 
		         	  Paint_List();
                	  break;
                case VK_F6:  //进入房子展示界面 
				      Clear();
				      Drawhouse1();
				      Drawhouse2();
				      isshow=TRUE;
	                  break;
	            case VK_F7:  //进入房子展示界面 
				      Clear();
				      Draw_snow();
				      isshow=TRUE;
	                  break;
	            case VK_F8:   //返回CAD绘图界面 
				   	  Clear();
				   	  Paint_List();
				      isshow=FALSE;
	                  break;
	            	 
			 }
			 break;
		case KEY_UP:
			 break;
	 }	 
}


void MouseEventProcess(int x, int y, int button, int event)
{
	static bool isMove = FALSE;    //是否处于移动状态 
	static bool isChangeSize = FALSE;  //是否处于缩放状态/ 
 	static double x_pre = 0.0, y_pre = 0.0;  //前一个状态的时候鼠标位置
    double mx, my;  //当前鼠标的坐标位置 
	double x1, y1, x2, y2, dx, dy;
	mx = ScaleXInches(x);
	my = ScaleYInches(y);
 if(!isshow){
	switch (event) {
		case BUTTON_DOWN:
			if (isSelected) { //已是选中状态
       	    	x_pre = mx;
			  	y_pre = my;
			   	if (button == LEFT_BUTTON){ //左键按下，进入移动状态
         	    	isMove = TRUE;
         	       } 
				else if (button == RIGHT_BUTTON) {  //右键键按下，进入缩放状态
         	    	isChangeSize = TRUE;
         	    }
				break;
         	} 
         	PickNearest(shape, mx, my);   //选择距离鼠标当前位置最近的图形，并将其变成绿色 
         	switch (List_current) {
         	  	case LINE:
         	  		Line_current->isSelected = TRUE;
         	  		Line_current->pencolor = CopyString("GREEN");
         	  		break;
         	  	case RECT:
         	  		Rect_current->isSelected = TRUE;
         	  		Rect_current->pencolor = CopyString("GREEN");
         	  		break;
         	  	case ELLIPSE:
         	  		Ellipse_current->isSelected = TRUE;
         	  		Ellipse_current->pencolor = CopyString("GREEN");
         	  		break;
         	  	case TEXTB:
         	  		Text_current->isSelected = TRUE;
         	  		Text_current->Pointcolor = CopyString("GREEN");
         	  		break;
         	  }
         	  isSelected = TRUE;
         	  Paint_List();
              break;
		case BUTTON_UP:
        	isMove = FALSE; //退出移动状态
         	isChangeSize = FALSE;//退出缩放状态 
            break;
		case MOUSEMOVE:
			if (isSelected && (isMove || isChangeSize)) { //在选中状态下，进行移动或缩放操作 
			  	SetEraseMode(TRUE);
               	Paint_List();
               	dx = mx - x_pre;
               	dy = my - y_pre;
				if (isMove) { 
					switch (List_current){
						case LINE:
					  		Line_current->x1 += dx;
					  		Line_current->y1 += dy;
							Line_current->x2 += dx;
							Line_current->y2 += dy;
							break;
		 		    	case RECT:
					  		x1 = Rect_current->x1 + dx;
					  		y1 = Rect_current->y1 + dy;
					  		x2 = Rect_current->x2 + dx;
					  		y2 = Rect_current->y2 + dy;
					  		if (x1 < x2) {
                         		Rect_current->x1 = x1;
                         		Rect_current->x2 = x2;
					  		} else {
                         		Rect_current->x1 = x2;
                         		Rect_current->x2 = x1;
					  		}
					  		if (y1 < y2) {
                         		Rect_current->y1 = y1;
                         		Rect_current->y2 = y2;
					  		} else {
                         		Rect_current->y1 = y2;
                         		Rect_current->y2 = y1;
					  		}
					  		break;
		 		  		case ELLIPSE:
					  		Ellipse_current->cx += dx;
					  		Ellipse_current->cy += dy;
					  		break;
		 		  		case TEXTB:
					  		Text_current->x += dx;
					  		Text_current->y += dy;
					  		break;
				  	}
			  	}
			    else if (isChangeSize) { //缩放操作 
					switch (List_current){
						case LINE:
							if (fabs(x_pre-Line_current->x1)+fabs(y_pre-Line_current->y1) <
							    fabs(x_pre-Line_current->x2)+fabs(y_pre-Line_current->y2)) {
					  			Line_current->x1 += dx;
					  			Line_current->y1 += dy;
						    } else {
   								Line_current->x2 += dx;
								Line_current->y2 += dy;
						    }
							break;
		 		    	case RECT:
		 		    		if (fabs(x_pre-Rect_current->x1) < fabs(x_pre-Rect_current->x2)) {
		 		    			Rect_current->x1 += dx;
		 		    		} else {
		 		    			Rect_current->x2 += dx;
		 		    		}
		 		    		if (fabs(y_pre-Rect_current->y1) < fabs(y_pre-Rect_current->y2)) {
		 		    			Rect_current->y1 += dy;
		 		    		} else {
		 		    			Rect_current->y2 += dy;
		 		    		}
					  		break;
		 		  		case ELLIPSE:
					  		Ellipse_current->rx += dx;
					  		Ellipse_current->ry += dy;
					  		break;
		 		  		case TEXTB:
					  		break;
				  	}
				}
				x_pre = mx;
				y_pre = my;
				SetEraseMode(FALSE);
				Paint_List();
			}
            break;
    }	
 }
}

