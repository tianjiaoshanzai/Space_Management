#pragma once
#include "stdafx.h"


#define MAXGROUP 10			//定义组的大小
#define MAXJOB 100			//定义一个作业最大能申请的块数

//结构体定义

typedef struct node			//空闲盘块号管理节点
{
	int lock;				//空闲锁，1空闲，0,busy
	int quantity;
	int cell[MAXGROUP];		//组数
	struct node *next;
} group;

typedef struct node1		//进程
{
	char name[20];
	int quantity;
	int cell[MAXJOB];
	struct node1 *next;
} job;

void InitDisk(FILE *fp, int a);//初始化模拟DISK
group *initial();//初始化组函数
job *initial_job();	 //初始化进程函数

//读入空闲块流文件
void readData(FILE *fp, char fname[], int INIT_NUM, int *CAN_OPEN, int NumTemp[], int *NumTotal, int *i);//INIT_NUM 初始化-1  .CAN_OPEN初始化为1
																										  
void view(int *i, int STO[], int *FREEBLO, int *TOTALNUM);  //查看专用块函数

void bid(char jobname[], int number, int GteBlo[], int *i, int *CANAPPLY);		//新申请函数
void finish(char jobname[], int *FIndJob);		//撤消作业
void SimulateTime(char jobname[], int GteBlo[], int *CANAPPLY, int *FIndJob, int OccupyBlo[], int *num, int *CountT);	//模拟时间函数
void Start();

