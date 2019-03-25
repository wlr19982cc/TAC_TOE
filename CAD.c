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
#define TYPE          4    //����Ҫ��������ͼ�� 
#define LINE          0    // ����ֱ�� 
#define RECT          1    //������Բ 
#define ELLIPSE       2    //������Բ 
#define TEXTB         3    //�����ı� 
#define TEXTLEN       100  //�ı����� 
#define DRAWCOLOR    "BLACK"   //��ͼ��Ĭ����ɫ 
#define CURSOR        "_"      //��� 
#define TIMER_BLINK   5         //500ms��ʱ���¼���־�� 
#define m       GetWindowWidth()    //���ڿ�� 
#define n       GetWindowHeight()    //���ڸ߶� 
#define PI    3.1415926
typedef struct {
	double x1, y1;
    double x2, y2;
    int PenSize; 
    string pencolor;
    bool isSelected;  
} *LineA;   //ֱ�߽ṹ���ṹ��Ա���� ��ĩ��������꣬���ʳߴ磬������ɫ���Ƿ�ѡ�� 

typedef struct {
	double x1, y1;
    double x2, y2;
    int PenSize; 
    string pencolor; 
    bool isSelected;  
} *RectA;        //���νṹ���ṹ��Ա���� ���¡����ϵĺ������꣬���ʳߴ磬������ɫ���Ƿ�ѡ�� 

typedef struct {
    double cx, cy;
    double rx, ry;
    int PenSize; 
    string pencolor; 
    bool isSelected; 
} *EllipseA;   //��Բ�ṹ���ṹ��Ա���� Բ�ĺ������꣬���̰����С�����ʳߴ磬������ɫ���Ƿ�ѡ�� 

typedef struct {
	string text;
	double x, y;
    int cursor_Pos; 
    int PenSize; 
    string Pointcolor; 
    bool isSelected;  
    bool isBlink; 
    
} *TextA; //�ı��ṹ���ṹ��Ա���� �ı��ַ������ı���ʼ���ĺ������꣬���λ�ã����ʳߴ磬������ɫ���Ƿ�ѡ�У�����Ƕ���˸ 

void Clear(void);  //����������� 
void DrawBox(double x, double y, double width, double height);  //������ 
void DrawTriangle(double x, double y, double base, double height);   //�������� 
void Drawhouse1();   //����һ������ 
void Drawhouse2();   //���ڶ������� 
void KeyboardEventProcess(int key,int event);   //������Ϣ�ص�����
void CharEventProcess(char c);  //�ַ���Ϣ�ص�����
void MouseEventProcess(int x, int y, int button, int event);   //�����Ϣ�ص�����
void TimerEventProcess(int timerID);   //��ʱ����Ϣ�ص�����
void DrawLineA(void *line);   //��һ��ֱ�� 
bool LineEqual(void *line1, void *line2);  //�Ƚ�����ֱ���Ƿ���ͬ 
double distance_Line(double x, double y, LineA line);  //�����������λ����ֱ�����ĵľ��� 
LineA SelectNearestLine(linkedlistADT list, double mx, double my);   //������������λ�þ��������ֱ�ߣ���������ָ��    
void DrawRectT(void *rect);  //��һ������ 
bool RectEqual(void *rect1, void *rect2);//�Ƚ����������Ƿ���ͬ 
double distance_Rect(double x, double y, RectA rect);//�����������λ����������ĵľ��� 
RectA SelectNearestRect(linkedlistADT list, double mx, double my); //������������λ�þ�������ľ��Σ���������ָ��   
void DrawEllipseA(void *ellipse); //��һ����Բ 
bool EllipseEqual(void *ellipse1, void *ellipse2);//�Ƚ�������Բ�Ƿ���ͬ 
double distance_Ellipse(double x, double y, EllipseA ellipse);//�����������λ������Բ���ĵľ��� 
EllipseA SelectNearestEllipse(linkedlistADT list, double mx, double my); //������������λ�þ����������Բ����������ָ��   
void DrawTextT(void *text); //��ʾ�ı� 
void DrawCursorA(string str, int cursor_Pos, double startx, double starty); //��ʾ��� 
bool TextEqual(void *text1, void *text2);//�Ƚ������ı����ݡ�λ���Ƿ���ͬ 
double distance_Text(double x, double y, TextA text);//�����������λ�����ı����ĵľ��� 
TextA SelectNearestText(linkedlistADT list, double mx, double my);//������������λ�þ���������ı�����������ָ��   
void PickNearest(linkedlistADT list[], double mx, double my); //������������λ�þ��������ͼ�Σ���������ָ��   
void Paint_List();   //���ݴ洢��ָ�뻭������ͼ�� 
void start(); //˵��ʹ�÷����ĳ�ʼ���� 
void InsertCharToString(string str, int pos, char c); //����һ���ַ����ַ���ĩβ 
void DrawSnowLine(void *angle);   //����ѩ��ͼ�εı߳� 
void Divide_Line(linkedlistADT linkedlist);  //���սǶȽ�ѩ����ÿ���߷ֳ����� 
void KochSnow(linkedlistADT linkedlist, int abc);   //���õݹ�ķ������� 
void Draw_snow(void);


int ll; //һ���ӿ���̨���ȵ�ͼ�ν���ı�������ʵ�����ã����Ժ��ԣ� 
double length;   //ѩ��ͼ�εı߳� 
double angle_all;
LineA Line_current = NULL;  //ֱ������ĵ�ǰ����ָ��
RectA Rect_current = NULL;  //��������ĵ�ǰ����ָ��
EllipseA Ellipse_current = NULL;  //��Բ����ĵ�ǰ����ָ��
TextA Text_current = NULL;  //�ı�����ĵ�ǰ����ָ��
const int mseconds500 = 500;   //�����˸ʱ���� 
linkedlistADT shape[TYPE]={NULL, NULL, NULL, NULL};   //�洢����ͼ��Ԫ������ָ��
double Distance_min[TYPE];  //�洢����ͼ����̾�������� 
linkedlistADT SnowList; //�����������δ�Ÿ��߶εĽǶ�
char textbuf[TEXTLEN];    //��ǰ�ı�������
TextA text_pre;
int  List_current= LINE;    //��ǰѡ�е�ͼ����������
bool isCursorBlink = FALSE;  //����Ƿ���˸ 
bool isSelected = FALSE; //ͼ��Ԫ���Ƿ�ѡ��
bool inText = FALSE; //�Ƿ����ı��༭״̬
bool isshow = FALSE;   //�Ƿ���չʾ���Ƶķ���ͼ�εĽ��� 


void Main() 
{
	int i;
    InitGraphics();   //ͼ�γ�ʼ��
	Randomize();    //���������ʼ��
	registerKeyboardEvent(KeyboardEventProcess);   //ע�������Ϣ�ص�����
	registerCharEvent(CharEventProcess);   //ע���ַ���Ϣ�ص�����
	registerMouseEvent(MouseEventProcess);   //ע�������Ϣ�ص�����
	registerTimerEvent(TimerEventProcess);   //ע�ᶨʱ����Ϣ�ص�����
	
	InitConsole();     //����̨���ڳ�ʼ��	
	start();         
	FreeConsole();    //�رտ���̨���� 
	
	SetPenColor(DRAWCOLOR); 
    SetPenSize(1);

	for (i = 0; i < TYPE; i++) shape[i] = NewLinkedList();    //�½��ĸ������洢������ָ�������� 
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
		preangle = *((double *)NodeObj(SnowList, angle_list));    //�ýڵ���ԭ����ŵĽǶ� 
		angle = GetBlock(sizeof(double));
		*angle = preangle - 60;
        InsertNode(SnowList, angle_list, (void *)angle);     //�����β����±ߵĽǶȴ������һ�ڵ� 
		angle_list = NextNode(SnowList, angle_list);         //������һ�ڵ� 
		angle = GetBlock(sizeof(double));
		*angle = preangle + 60;                       
        InsertNode(SnowList, angle_list, (void *)angle);    //�����β����±ߵĽǶȴ������һ�ڵ� 
		angle_list = NextNode(SnowList, angle_list);         //������һ�ڵ�    
		angle = GetBlock(sizeof(double));
		*angle = preangle;
        InsertNode(SnowList, angle_list, (void *)angle);    //�����β����±ߵĽǶȴ������һ�ڵ� 
		angle_list = NextNode(SnowList, angle_list);        //������һ�ڵ�   
		angle_list = NextNode(SnowList, angle_list);       //������һ�ڵ�      
	}
}


void KochSnow(linkedlistADT SnowList, int abc)
{
	double *angle;

	if (abc == 0) { //1��ѩ�������ߵĽǶȣ��ӵױ߿�ʼ�� 
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
		KochSnow(SnowList, abc-1);  //�ݹ���� 
        Divide_Line(SnowList);   //��ÿһ���߷ֱ��������� 
	}
}


void Draw_snow() 
{
	int abc;
	SetPenColor(DRAWCOLOR); 
    SetPenSize(1);      
	InitConsole();
	printf("Enter n: "); //�������
	abc = GetInteger();
	printf("Enter length: ");  //�����ʼ�߳�
	length = GetReal();
	FreeConsole();
	MovePen(m/2.0 - length/2.0,n/2 - length/2.0*tan(30/180.0*PI));  //Ϊ��ʹͼ�δ������м�������ۣ������ó�ʼ������ͼ�����½Ǵ� 
	length = length / pow(3.0, (double)abc); 
	SnowList = NewLinkedList(); 
	KochSnow(SnowList, abc); 
    TraverseLinkedList(SnowList, DrawSnowLine); //���������������Ƴ�ÿһ���� 
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
	Distance_min[LINE] = distance_min;   //����̾���洢�������� 

	return (LineA)NodeObj(list, nearest);    //���ؾ������λ�������ֱ�ߵ�ָ�� 
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
	Distance_min[RECT] = distance_min;    //����̾���洢�������� 
	return (RectA)NodeObj(list, nearest);  //���ؾ������λ������ľ��ε�ָ�� 
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
	Distance_min[ELLIPSE] = distance_min;    //����̾���洢�������� 
	return (EllipseA)NodeObj(list, nearest);    //���ؾ������λ���������Բ��ָ�� 
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
	Distance_min[TEXTB] = distance_min;  //����̾���洢�������� 
	return (TextA)NodeObj(list, nearest);  //���ؾ������λ��������ı���ָ�� 
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
	List_current = list_min;   //��¼�������λ�������ͼ�ε����  
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
	printf("\t�X�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�T�[\n"); 
	printf("\t�U                                                              �U\n");
	printf("\t�U                                                              �U\n");
    printf("\t�U                 ._______________________.                    �U\n");
    printf("\t�U                 | _____________________ |                    �U\n"); 
    printf("\t�U                 | I                   I |                    �U\n"); 
    printf("\t�U                 | I��ӭʹ��CAD��ͼ����I |                    �U\n"); 
    printf("\t�U                 | I                   I |                    �U\n"); 
    printf("\t�U                 | I   made by   DAC     |                    �U\n"); 
    printf("\t�U                 | I___________________I |                    �U\n"); 
    printf("\t�U                 !_______________________!                    �U\n"); 
    printf("\t�U                     ._[__________]_.                         �U\n"); 
    printf("\t�U                 .___|_______________|___.                    �U\n"); 
    printf("\t�U                  |::: ____             |                     �U\n");  
    printf("\t�U                  |    ~~~~ [CD-ROM]    |                     �U\n"); 
    printf("\t�U                  !_____________________!                     �U\n");
	printf("\t�U                                                              �U\n"); 
	printf("\t�U                                                              �U\n");
    printf("\t�U                                                              �U\n");                                    
    printf("\t�Uʹ��˵����F1-����ֱ�� F2-���ƾ��� F3-������Բ F4-�༭���ַ��� �U\n"); 
    printf("\t�UF5-�༭�Ѵ��ڵ��ַ���  F5-��ʾ����ͼ�� F6-�ص�CAD��ͼ����     �U\n"); 
    printf("\t�UEsc��return-�˳��ַ����ı༭  delete-ɾ���ַ����ַ�����ͼ��   �U\n"); 
    printf("\t�U�����������ƶ�-ͼ�ε��ƶ�   ����Ҽ������ƶ�-ͼ�ε�����     �U\n");
	printf("\t�U     �����������ֲ��س������ͼ���棺"); 
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
			inText = FALSE;   //�˳���ǰ�ı��༭״̬ 
           	SetEraseMode(TRUE);   //��ʼʹ����Ƥ�� 
	 	   	MovePen(text_pre->x, text_pre->y);
	 	   	DrawTextString(textbuf);   //��ȥ�ı� 
			DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);   //��ȥ��� 
			Paint_List();   //��ȥ����ͼ�� 
			text_pre->isBlink = FALSE;  //��겻����˸ 
			text_pre->text = CopyString(textbuf);   //���ı������������ݴ���ṹ�� 
           	InsertNode(shape[TEXTB], NULL, text_pre);   //�����ṹ��������ָ�������� 
           	SetEraseMode(FALSE);   //�ر���Ƥ�� 
			Paint_List();   //���»�������ͼ�� 
			cancelTimer(TIMER_BLINK);   //ע�������˸��ʱ��
			isCursorBlink = FALSE;   //�رչ����˸ 
			SetPenColor(DRAWCOLOR);   //���ʻָ�Ĭ����ɫ 
			break;
    	default:
    		if ((len = strlen(textbuf)) >= TEXTLEN) break; 
			SetEraseMode(TRUE);
			MovePen(text_pre->x, text_pre->y);
			DrawTextString(textbuf);   
			DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);/*������ǰ���*/
			InsertCharToString(textbuf, text_pre->cursor_Pos, c);   //����ǰ�ַ����뵽�ַ���ĩβ���λ�ô� 
	 	   	SetEraseMode(FALSE);
	 	   	MovePen(text_pre->x, text_pre->y);
	 	   	DrawTextString(textbuf);   //������ʾ�µ��ַ��� 
	 	   	(text_pre->cursor_Pos)++;   //���λ�ú���һλ 
	 	   	DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);   //������ǰ���
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
			     case VK_F1:if(!isshow){ //�������ֱ��
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
			     case VK_F2:if(!isshow){//������ɾ���
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
			     case VK_F3:if(!isshow){//���������Բ
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
			     case VK_F4:if(!isshow){//������ɿյ��ı�����
			         if (inText) break;//���Ѵ����ı��༭״̬���������µĿ��ı�
					 inText = TRUE; //�����µ��ı��б༭״̬
					 text_pre = GetBlock(sizeof(*text_pre));
					 text_pre->x = RandomReal(1.0, GetWindowWidth()-1.0);
                     text_pre->y = RandomReal(1.0, GetWindowHeight()-1.0);
					 MovePen(text_pre->x, text_pre->y);//�����Ƶ��ı���ʾ��ʼλ��
	 	   			 DrawTextString(CURSOR);
                     textbuf[0] = '\0'; //���ַ���
                     text_pre->PenSize = GetPointSize();
                     text_pre->Pointcolor = GetPenColor();
                     text_pre->isSelected = FALSE;
                     text_pre->cursor_Pos = 0; //���õ�ǰ���λ��
                     text_pre->isBlink = TRUE; //�򿪹����˸���� 
                     startTimer(TIMER_BLINK, mseconds500);   //�����˸��ʱ������
                     isCursorBlink = TRUE;   //���ù����˸
				}
                     break;              
                 case VK_F5:if(!isshow){  //�༭���е��ı� 
					if (!isSelected) break;   //��ǰ�ı�δ��ѡ�� 
					if (inText) break;    //��ǰ�������ı��б༭״̬ 
					inText = TRUE;  //�����ı��༭״̬
					text_pre = Text_current;  //ѡȡ��ǰ�༭���ı�����
					strcpy(textbuf, text_pre->text); 
					DeleteNode(shape[TEXTB], Text_current, TextEqual);   //�ӵ�ǰ�ı�������ɾ�����ı�����
					SetPenColor(text_pre->Pointcolor); 
					DrawCursorA(textbuf, text_pre->cursor_Pos, text_pre->x, text_pre->y);
					text_pre->isBlink = TRUE; 
            		startTimer(TIMER_BLINK, mseconds500);
            		isCursorBlink = TRUE; 
				}
				     break;
			     case VK_DELETE:
				    if (inText) { 
				        if(text_pre->cursor_Pos>=1){//���ı��༭״̬��ɾ���ַ���ĩβ���ַ� 
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
				    if (isSelected){//ɾ��ѡ�е�ͼ�� 
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
                    
                 
                case VK_ESCAPE:  //�˳�ͼ��Ԫ��ѡ��״̬
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
                case VK_F6:  //���뷿��չʾ���� 
				      Clear();
				      Drawhouse1();
				      Drawhouse2();
				      isshow=TRUE;
	                  break;
	            case VK_F7:  //���뷿��չʾ���� 
				      Clear();
				      Draw_snow();
				      isshow=TRUE;
	                  break;
	            case VK_F8:   //����CAD��ͼ���� 
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
	static bool isMove = FALSE;    //�Ƿ����ƶ�״̬ 
	static bool isChangeSize = FALSE;  //�Ƿ�������״̬/ 
 	static double x_pre = 0.0, y_pre = 0.0;  //ǰһ��״̬��ʱ�����λ��
    double mx, my;  //��ǰ��������λ�� 
	double x1, y1, x2, y2, dx, dy;
	mx = ScaleXInches(x);
	my = ScaleYInches(y);
 if(!isshow){
	switch (event) {
		case BUTTON_DOWN:
			if (isSelected) { //����ѡ��״̬
       	    	x_pre = mx;
			  	y_pre = my;
			   	if (button == LEFT_BUTTON){ //������£������ƶ�״̬
         	    	isMove = TRUE;
         	       } 
				else if (button == RIGHT_BUTTON) {  //�Ҽ������£���������״̬
         	    	isChangeSize = TRUE;
         	    }
				break;
         	} 
         	PickNearest(shape, mx, my);   //ѡ�������굱ǰλ�������ͼ�Σ�����������ɫ 
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
        	isMove = FALSE; //�˳��ƶ�״̬
         	isChangeSize = FALSE;//�˳�����״̬ 
            break;
		case MOUSEMOVE:
			if (isSelected && (isMove || isChangeSize)) { //��ѡ��״̬�£������ƶ������Ų��� 
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
			    else if (isChangeSize) { //���Ų��� 
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

