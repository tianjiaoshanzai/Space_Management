#pragma once
#include "stdafx.h"


#define MAXGROUP 10			//������Ĵ�С
#define MAXJOB 100			//����һ����ҵ���������Ŀ���

//�ṹ�嶨��

typedef struct node			//�����̿�Ź���ڵ�
{
	int lock;				//��������1���У�0,busy
	int quantity;
	int cell[MAXGROUP];		//����
	struct node *next;
} group;

typedef struct node1		//����
{
	char name[20];
	int quantity;
	int cell[MAXJOB];
	struct node1 *next;
} job;

void InitDisk(FILE *fp, int a);//��ʼ��ģ��DISK
group *initial();//��ʼ���麯��
job *initial_job();	 //��ʼ�����̺���

//������п����ļ�
void readData(FILE *fp, char fname[], int INIT_NUM, int *CAN_OPEN, int NumTemp[], int *NumTotal, int *i);//INIT_NUM ��ʼ��-1  .CAN_OPEN��ʼ��Ϊ1
																										  
void view(int *i, int STO[], int *FREEBLO, int *TOTALNUM);  //�鿴ר�ÿ麯��

void bid(char jobname[], int number, int GteBlo[], int *i, int *CANAPPLY);		//�����뺯��
void finish(char jobname[], int *FIndJob);		//������ҵ
void SimulateTime(char jobname[], int GteBlo[], int *CANAPPLY, int *FIndJob, int OccupyBlo[], int *num, int *CountT);	//ģ��ʱ�亯��
void Start();

