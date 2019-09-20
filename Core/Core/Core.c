// TT.cpp: �������̨Ӧ�ó������ڵ㡣
//
//#define _CRT_SECURE_NO_DEPRECATE��

#include "stdafx.h"
#include "Core.h"


group *head;
int total;		//�����ܿ���

job *jhead;

//��ʼ��ģ��DISK
void InitDisk(FILE *fp, int a)
{
	for (int i = 0; i<a; i++)
	{
		fprintf(fp, "%d ", i);
	}
}

//��ʼ�������̿��·�ڵ㺯��
group *initial()
{
	int i;
	group *p = (group*)malloc(sizeof(group));

	p->lock = 1;

	p->quantity = 0;
	p->next = NULL;

	for (i = 0; i<MAXGROUP; i++)
	{
		p->cell[i] = -1;
	}

	return p;
}

//��ʼ�����̺���
job *initial_job()
{
	int i;
	
	job *p = (job*)malloc(sizeof(job));


	strcpy(p->name, "");
	p->quantity = 0;
	p->next = NULL;

	for (i = 0; i<MAXGROUP; i++)
	{
		p->cell[i] = -1;
	}

	return p;
}

//������п����ļ�
void readData(FILE *fp,char fname[], int INIT_NUM, int *CAN_OPEN,int NumTemp[],int *NumTotal,int *i)//INIT_NUM ��ʼ��-1  .CAN_OPEN��ʼ��Ϊ1
{
	
	//
	//FILE *fp;
	//char fname[20];
	int temp;
	group *p;

		if (INIT_NUM!=-1)
		{
			
			FILE *Rfp;
			if ((Rfp = fopen(fname, "wt")) == NULL)
			{
				*CAN_OPEN = 0;
				//cout << "����,�ļ��򲻿�,�����ļ���:)" << endl;

			}
			else
			{
				//int INIT_NUM;//��ʼ������
				//cout << "��ʼ������:";
				//cin >> INIT_NUM;
				InitDisk(Rfp, INIT_NUM);
				fclose(Rfp);


				*i = 0;
				while (!feof(fp))
				{
					fscanf(fp, "%d ", &temp);
					if (head->quantity<MAXGROUP)
					{
						head->cell[head->quantity] = temp;
						head->quantity++;
					}
					else
					{
						p = initial();
						p->next = head;
						head = p;
						p->cell[p->quantity] = temp;
						p->quantity++;
					}
					total++;
					//�����ʼ����

					//cout << temp << " ";
					NumTemp[*i] = temp;
					(*i)++;
				}

				//cout << endl << "�ܿ��п���:" << total << endl;
				*NumTotal = total;
			}

		}

		
}

//�鿴ר�ÿ麯��
void view(int *i, int STO[],int *FREEBLO,int *TOTALNUM)
{
	//ר�ÿ���������
	//���洢�Ŀ��п��
	
	for (*i = 0; *i<head->quantity;(*i)++)
	{
		
		//cout << head->cell[i] << " ";
		STO[*i] = head->cell[*i];
	}
	//cout << endl << "ר�ÿ���п���Ϊ:" << head->quantity;
	*FREEBLO = head->quantity;
	//cout << endl << "�ܿ��п���:" << total << endl;
	*TOTALNUM = total;
}


//�����뺯��
void bid(char jobname[], int number,int GteBlo[], int *i,int *CANAPPLY)	//CANAPPL��ʼ��Ϊ1
{
	
	job *p;

	

	if (number>total)
	{
		//cout << "�����ڴ�������ڵ�ǰ���п���,���Ժ�����:)" << endl;
		*CANAPPLY = 0;
	}
	else
	{
		p = initial_job();
		strcpy(p->name, jobname);
		p->next = jhead->next;
		jhead->next = p;
		//p->quantity = number;
		
		//cout << "�����뵽�Ŀ��п����:";
		for (*i = 0; *i<number; (*i)++)
		{
			if (head->quantity>1)
			{
				//cout << head->cell[head->quantity - 1] << " ";
				GteBlo[*i] = head->cell[head->quantity - 1];
				
				//head->quantity--;
				p->cell[*i] = head->cell[head->quantity - 1];
				head->quantity--;
				p->quantity++;
			}
			else      //���ʱת��һ��
			{
				//cout << head->cell[0] << " ";
				GteBlo[*i] = head->cell[0];

				p->cell[*i] = head->cell[head->quantity - 1];
				head->quantity--;
				p->quantity++;
				if (head->next != NULL)
				{
					head = head->next;
				}
			}
			total--;
		}
	}
	//cout << endl;
}

//��������
void finish(char jobname[],int *FIndJob)//FIndJob��ʼ��Ϊ1
{
	
	int i;
	job *p, *q;
	group *r;

	//cout << "������Ҫ�����Ľ�����:";
	//cin >> jobname;

	p = initial_job();
	q = initial_job();
	//q->next = jhead;
	q = jhead;
	p = jhead->next;
	while ((p != NULL) && (strcmp(p->name, jobname)))
	{
		q = q->next;
		p = p->next;
	}

	if (p == NULL)
	{
		//cout << "Sorry,û�иý���" << endl;
		*FIndJob = 0;
	}
	else
	{
		for (i = 0; i<p->quantity; i++)
		{
			if (head->quantity<MAXGROUP)
			{
				head->cell[head->quantity] = p->cell[i];
				head->quantity++;
				//p->quantity--;
			}
			else   //�½�һ��
			{
				r = initial();
				r->next = head;
				head = r;
				head->cell[head->quantity] = p->cell[i];
				head->quantity++;
				//r->cell[r->quantity] = p->cell[i];
				//r->quantity++;
			}
		}
		total += p->quantity;

		q->next = p->next;
		free(p);
	}
}

void SimulateTime(char jobname[], int GteBlo[],int *CANAPPLY,int *FIndJob, int OccupyBlo[], int *num,int *CountT)
{
	job *p, *q;
	
	

	
	p = initial_job();
	p = jhead->next;
	
	while ((p != NULL) && (strcmp(p->name, jobname)))
	{
		//q = q->next;
		p = p->next;	//�ҵ�ģ�����
	}

		if (p == NULL)
		{
			//cout << "Sorry,û�иý���" << endl;
			*FIndJob = 0;
			return;
		}

	//job *p;
	

	//��ʱ����------------------------------------------------------
	time_t start, end;
	start = time(NULL);
	system("pause");	//���Է�RunProcess
	end = time(NULL);
	

	int runtime = difftime(end, start);
	//��ʱ���ֽ���----------------------------------------------------
	
	*CountT = runtime;

	//���runtime��Ϊ�ڴ棬���Ѽ�ʱ����

	if (runtime>total)		//runtime���پͶ�̬��������ڴ棬���ԸĶ�Ӧ������ϵ
	{
		//cout << "�����ڴ�������ڵ�ǰ���п���,���Ժ�����:)" << endl;
		*CANAPPLY = 0;
	}
	else
	{
		

		//cout << "�����뵽�Ŀ��п����:";
		int old = p->quantity;//��¼ԭռ���ڴ����

		int i;//������
		for (i = 0; i<runtime; i++)
		{
			if (head->quantity>1)
			{
				//cout << head->cell[head->quantity - 1] << " ";
				GteBlo[i] = head->cell[head->quantity - 1];

				//head->quantity--;
				p->cell[i+old] = head->cell[head->quantity - 1];
				head->quantity--;
				p->quantity++;
			}
			else      //���ʱת��һ��
			{
				//cout << head->cell[0] << " ";
				GteBlo[i] = head->cell[0];

				p->cell[i+old] = head->cell[head->quantity - 1];
				head->quantity--;
				p->quantity++;
				if (head->next != NULL)
				{
					head = head->next;
				}
			}
			total--;
		}

		//����ý���Ŀǰռ�õ����п�
		*num = p->quantity;
		for (i = 0; i < *num; i++)
		{
			OccupyBlo[i] = p->cell[i];
		}
	}

}

//��ʼ���ں�����
void Start()
{
	head = initial();
	total = 0;
	jhead = initial_job();
}





