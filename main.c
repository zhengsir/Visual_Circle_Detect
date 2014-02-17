#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <time.h>
#include <math.h>
#include <windows.h>
#define POINT_RANGE 5   //选顶点时的邻域半径
bool left_up = false;
bool right_up = false;
bool right_down = false;
bool left_down = false;
//声明长方形公共变量
CvRect box;
bool drawing_box = false;
//声明多边形公共变量
int arr[1]; 
int multi_box[5][4]; //预存5个线圈
CvPoint ** pt = new CvPoint*[1]; 
bool drawing_multi = false;
/*-------------------------------------------------------------------------
功能：画长方形函数
输入：图像指针
输出：无
date: 2014-02-11
---------------------------------------------------------------------------*/
void draw_box(IplImage *img,CvRect rect)
{
	cvRectangle(img, cvPoint(box.x, box.y), cvPoint(box.x + box.width , box.y + box.height ), cvScalar(0xff, 0x00, 0x00));  //蓝色长方形，标量顺序是BGR,  CV_RGB( r, g, b )  cvScalar( (b), (g), (r) ) 
}
/*-------------------------------------------------------------------------
功能：画多边形函数
输入：图像指针
输出：无
date: 2014-02-11
---------------------------------------------------------------------------*/
void draw_multi(IplImage *img)
{
	cvPolyLine(img, pt, arr, 1, 1, CV_RGB(250,0,0)); //红色多边形
}
/*-------------------------------------------------------------------------
功能：鼠标事件回调函数
输入：鼠标事件，坐标
输出：无
date: 2014-01-23
---------------------------------------------------------------------------*/
void my_mouse_callback(int event,int x, int y, int flags, void* param)
{
	arr[0] = 4;  // 四边形
	//pt[0] = new CvPoint[4]; 
	IplImage *image = (IplImage*) param;
	switch(event)
	{
	case CV_EVENT_MOUSEMOVE:
		{
			if(drawing_box)
			{
				box.width = x - box.x;
				box.height = y - box.y;
				printf("box.width=%d,box.height=%d\n",box.width, box.height);
			}
			else if(drawing_multi)
			{
				//跟踪当前顶点
				if (left_up)
				{
					pt[0][0] = cvPoint(x, y); 
					printf("pt[0][0].x=%d,pt[0][0].y=%d\n", pt[0][0].x, pt[0][0].y);
				}
				else if (right_up)
				{		
					pt[0][1] = cvPoint(x, y);
					printf("pt[0][1].x=%d,pt[0][1].y=%d\n", pt[0][1].x, pt[0][1].y);
				}
				else if (right_down)
				{
					pt[0][2] = cvPoint(x, y);
					printf("pt[0][2].x=%d,pt[0][2].y=%d\n", pt[0][2].x, pt[0][2].y);
				}
				else if (left_down)
				{
					pt[0][3] = cvPoint(x, y);	
					printf("pt[0][3].x=%d,pt[0][3].y=%d\n", pt[0][3].x, pt[0][3].y);
				}
			}
			else
			{
				//首先捕捉顶点,方便选中顶点
				if((multi_box[1][1] != 0) && (pt[0][0].x - POINT_RANGE <= x ) && (x <= pt[0][0].x + POINT_RANGE) && (pt[0][0].y - POINT_RANGE <= y) && (y <= pt[0][0].y + POINT_RANGE))
					cvCircle(image, cvPoint(pt[0][0].x ,pt[0][0].y), 40/5, CV_RGB(250, 0, 0), 2, 8, 0 );
				else if((multi_box[1][1] != 0) && (pt[0][1].x - POINT_RANGE <= x ) && (x <= pt[0][1].x + POINT_RANGE) && (pt[0][1].y - POINT_RANGE <= y) && (y <= pt[0][1].y + POINT_RANGE))
					cvCircle(image, cvPoint(pt[0][1].x ,pt[0][1].y), 40/5, CV_RGB(250, 0, 0), 2, 8, 0 );
				else if((multi_box[1][1] != 0) && (pt[0][2].x - POINT_RANGE <= x ) && (x <= pt[0][2].x + POINT_RANGE) && (pt[0][2].y - POINT_RANGE <= y) && (y <= pt[0][2].y + POINT_RANGE))
					cvCircle(image, cvPoint(pt[0][2].x ,pt[0][2].y), 40/5, CV_RGB(250, 0, 0), 2, 8, 0 );
				else if((multi_box[1][1] != 0) && (pt[0][3].x - POINT_RANGE <= x ) && (x <= pt[0][3].x + POINT_RANGE) && (pt[0][3].y - POINT_RANGE <= y) && (y <= pt[0][3].y + POINT_RANGE))
					cvCircle(image, cvPoint(pt[0][3].x ,pt[0][3].y), 40/5, CV_RGB(250, 0, 0), 2, 8, 0 );
				else if((multi_box[1][1] != 0))
				{
					cvCircle(image, cvPoint(pt[0][0].x ,pt[0][0].y), 3, CV_RGB(0, 0, 255), 2, 8, 0 );
					cvCircle(image, cvPoint(pt[0][1].x ,pt[0][1].y), 3, CV_RGB(0, 0, 255), 2, 8, 0 );
					cvCircle(image, cvPoint(pt[0][2].x ,pt[0][2].y), 3, CV_RGB(0, 0, 255), 2, 8, 0 );
					cvCircle(image, cvPoint(pt[0][3].x ,pt[0][3].y), 3, CV_RGB(0, 0, 255), 2, 8, 0 );
				}
			}
		}
		break;
	case CV_EVENT_LBUTTONDOWN:
		{
			//判断是调整线圈还是新画线圈，
			//确定当前顶点：左上顶点
			if((multi_box[1][1] != 0) && (pt[0][0].x - POINT_RANGE <= x ) && (x <= pt[0][0].x + POINT_RANGE) && (pt[0][0].y - POINT_RANGE <= y) && (y <= pt[0][0].y + POINT_RANGE))
			{
				left_up = true;
				right_up = false;
				right_down = false;
				left_down = false;
				drawing_multi = true;
				drawing_box = false;
				printf("Darging...1\n");
			}//右上顶点
			else if((multi_box[1][1] != 0) && (pt[0][1].x - POINT_RANGE <= x ) && (x <= pt[0][1].x + POINT_RANGE) && (pt[0][1].y - POINT_RANGE <= y) && (y <= pt[0][1].y + POINT_RANGE))
			{
				left_up = false;
				right_up = true;
				right_down = false;
				left_down = false;		
				drawing_multi = true;
				drawing_box = false;
				printf("Darging...2\n");
			}//右下顶点
			else if((multi_box[1][1] != 0) && (pt[0][2].x - POINT_RANGE <= x ) && (x <= pt[0][2].x + POINT_RANGE) && (pt[0][2].y - POINT_RANGE <= y) && (y <= pt[0][2].y + POINT_RANGE))
			{
				left_up = false;
				right_up = false;
				right_down = true;
				left_down = false;	
				drawing_multi = true;
				drawing_box = false;
				printf("Darging...3\n");
			}//左下顶点
			else if((multi_box[1][1] != 0) && (pt[0][3].x - POINT_RANGE <= x ) && (x <= pt[0][3].x + POINT_RANGE) && (pt[0][3].y - POINT_RANGE <= y) && (y <= pt[0][3].y + POINT_RANGE))
			{
				left_up = false;
				right_up = false;
				right_down = false;
				left_down = true;	
				drawing_multi = true;
				drawing_box = false;
				printf("Darging...4\n");
			}
			else
			{
				drawing_multi  = false;
				drawing_box = true;
				printf("Creating...\n");
				box = cvRect(x, y, 0, 0);
			}
		}
		break;
	case CV_EVENT_LBUTTONUP:
		{
			if (drawing_multi)
			{
				//释放当前顶点
				if (left_up)
				{
				printf("Darg-UP-1!\n");
				drawing_multi = false;
				left_up = false;
				draw_multi(image);	
				printf("(%d,%d),(%d,%d),(%d,%d),(%d,%d) \n",pt[0][0].x, pt[0][0].y, pt[0][1].x, pt[0][1].y, pt[0][2].x, pt[0][2].y, pt[0][3].x, pt[0][3].y);				
				}
				else if(right_up)
				{
				printf("Darg-UP-2!\n");
				drawing_multi = false;
				right_up = false;
				draw_multi(image);	
				printf("(%d,%d),(%d,%d),(%d,%d),(%d,%d) \n",pt[0][0].x, pt[0][0].y, pt[0][1].x, pt[0][1].y, pt[0][2].x, pt[0][2].y, pt[0][3].x, pt[0][3].y);
				}
				else if(right_down)
				{
				printf("Darg-UP-3!\n");
				drawing_multi = false;
				right_down = false;
				draw_multi(image);	
				printf("(%d,%d),(%d,%d),(%d,%d),(%d,%d) \n",pt[0][0].x, pt[0][0].y, pt[0][1].x, pt[0][1].y, pt[0][2].x, pt[0][2].y, pt[0][3].x, pt[0][3].y);
				}
				else if(left_down)
				{
				printf("Darg-UP-4!\n");
				drawing_multi = false;
				left_down = false;
				draw_multi(image);	
				printf("(%d,%d),(%d,%d),(%d,%d),(%d,%d) \n",pt[0][0].x, pt[0][0].y, pt[0][1].x, pt[0][1].y, pt[0][2].x, pt[0][2].y, pt[0][3].x, pt[0][3].y);
				}
			}
			else
			{
				printf("Create-UP!\n");
				drawing_box = false;
				if(box.width != 0 && box.height != 0)   //取消单击时的画点情形
				{
					if(box.width < 0)
					{
						box.x += box.width;
						box.width *= -1;
					}
					if(box.height < 0)
					{
						box.y += box.height;
						box.height *= -1;
					}
					draw_box(image, box);
					printf("(%d,%d),(%d,%d),(%d,%d),(%d,%d) \n",box.x, box.y, box.x + box.width, box.y,  box.x + box.width, box.y + box.height, box.x, box.y + box.height);
					//保存四边形的4个顶点到multi_box，分别是左上角和右下角的绝对坐标
					multi_box[1][1] = box.x;
					multi_box[1][2] = box.y;
					multi_box[1][3] = box.x + box.width;
					multi_box[1][4] = box.y + box.height;
					pt[0] = new CvPoint[4];  //在此新开辟顶点的内存
					pt[0][0] = cvPoint(multi_box[1][1], multi_box[1][2]); 
					pt[0][1] = cvPoint(multi_box[1][3], multi_box[1][2]); 
					pt[0][2] = cvPoint(multi_box[1][3], multi_box[1][4]); 
					pt[0][3] = cvPoint(multi_box[1][1], multi_box[1][4]); 

				}
			}
		}
		break;
	}
}
/*-------------------------------------------------------------------------
功能：主函数
输入：参数
输出：无
date: 2014-01-23
---------------------------------------------------------------------------*/
int main(int argc,char *argv[])
{
	char c[20];  //坐标点标记文本
	cvNamedWindow("第一帧图像",CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);   //原始视频帧capture
	IplImage* first_frame;
	first_frame = cvQueryFrame(capture);                 //第一帧first_frame
	cvShowImage("第一帧图像",first_frame);
	box = cvRect(-1, -1, 0, 0);
	IplImage *temp = cvCloneImage(first_frame);          //临时处理帧temp
	cvSetMouseCallback("第一帧图像", my_mouse_callback, (void *)temp);  //分析first_frame 和 temp的区别
    CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_DUPLEX,  0.5, 0.5, 0.5, 1, 8);
	while(1)
	{		
		if(drawing_box)
		{
			cvCopyImage(first_frame, temp);
			draw_box(temp, box);
			sprintf(c, "(%d, %d)", box.x, box.y );
			cvPutText(temp, c, cvPoint(box.x - 90, box.y), &font,cvScalar(0x00, 0xff, 0x00)); //左上顶点和左下顶点的x坐标位置稍作偏移
			sprintf(c, "(%d, %d)", box.x + box.width , box.y );
			cvPutText(temp, c, cvPoint(box.x + box.width, box.y), &font,cvScalar(0x00, 0xff, 0x00));
			sprintf(c, "(%d, %d)", box.x + box.width, box.y + box.height);
			cvPutText(temp, c, cvPoint(box.x + box.width, box.y + box.height), &font,cvScalar(0x00, 0xff, 0x00));
			sprintf(c, "(%d, %d)", box.x, box.y + box.height );
			cvPutText(temp, c, cvPoint(box.x - 90, box.y + box.height), &font,cvScalar(0x00, 0xff, 0x00));

		}
		if(drawing_multi)
		{
			cvCopyImage(first_frame, temp);
			//标记当前顶点
			if (left_up)
			{
				cvCircle(temp, cvPoint(pt[0][0].x ,pt[0][0].y), 4, CV_RGB(250,0,0), 2, 8, 0 );    //画圆圈标记

			}
			else if(right_up)
			{
				cvCircle(temp, cvPoint(pt[0][1].x ,pt[0][1].y), 4, CV_RGB(250,0,0), 2, 8, 0 );
			}
			else if(right_down)
			{
				cvCircle(temp, cvPoint(pt[0][2].x ,pt[0][2].y), 4, CV_RGB(250,0,0), 2, 8, 0 );
			}
			else if(left_down)
			{
				cvCircle(temp, cvPoint(pt[0][3].x ,pt[0][3].y), 4, CV_RGB(250,0,0), 2, 8, 0 ); 
			}
			draw_multi(temp);
			sprintf(c, "(x1=%d, y1=%d)", pt[0][0].x ,pt[0][0].y );
			cvPutText(temp, c, cvPoint(pt[0][0].x - 150 ,pt[0][0].y ), &font, cvScalar(0x00, 0xff, 0x00));
			sprintf(c, "(x2=%d, y2=%d)", pt[0][1].x ,pt[0][1].y );
			cvPutText(temp, c, cvPoint(pt[0][1].x ,pt[0][1].y ), &font, cvScalar(0x00, 0xff, 0x00));
			sprintf(c, "(x3=%d, y3=%d)", pt[0][2].x ,pt[0][2].y );
			cvPutText(temp, c, cvPoint(pt[0][2].x ,pt[0][2].y ), &font, cvScalar(0x00, 0xff, 0x00));
			sprintf(c, "(x4=%d, y4=%d)", pt[0][3].x ,pt[0][3].y );
			cvPutText(temp, c, cvPoint(pt[0][3].x - 150 ,pt[0][3].y ), &font, cvScalar(0x00, 0xff, 0x00));
		}		
		cvShowImage("第一帧图像",temp);
		if(cvWaitKey(40) == 27) break;
	}
	return 0;
}













	
	/* 读视频
	while(1)
	{
		
		frame = cvQueryFrame(capture);
		if(!frame) break;
		cvShowImage("实时视频",frame);
		char c = cvWaitKey(40);   //原始视频是25fps，所以40较合适
		if(c == 27) break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("AVI Playing");
	*/

	/* 画多边形
	int arr[1]; 
	arr[0] = 4;  // 四边形
	CvPoint ** pt = new CvPoint*[1]; 
	pt[0] = new CvPoint[4]; 
	pt[0][0] = cvPoint(10,10); 
	pt[0][1] = cvPoint(100,10); 
	pt[0][2] = cvPoint(100,100); 
	pt[0][3] = cvPoint(10,100);  //顶点
	IplImage* image = cvCreateImage( cvSize(200,200), 8, 3 ); 
	cvPolyLine(image, pt, arr, 1, 1, CV_RGB(250,0,0)); 
	//cvFillPoly(image,pt,arr,1,CV_RGB(250,0,0)); 
	cvShowImage(wndname,image); 
	*/
