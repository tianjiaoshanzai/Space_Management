// TT.cpp: 定义控制台应用程序的入口点。
//
//#define _CRT_SECURE_NO_DEPRECATE；

#include "stdafx.h"
#include "Core.h"


group *head;
int total;		//空闲总块数

job *jhead;

//初始化模拟DISK
void InitDisk(FILE *fp, int a)
{
	for (int i = 0; i<a; i++)
	{
		fprintf(fp, "%d ", i);
	}
}

//初始化空闲盘块管路节点函数
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

//初始化进程函数
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

//读入空闲块流文件
void readData(FILE *fp,char fname[], int INIT_NUM, int *CAN_OPEN,int NumTemp[],int *NumTotal,int *i)//INIT_NUM 初始化-1  .CAN_OPEN初始化为1
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
				//cout << "错误,文件打不开,请检查文件名:)" << endl;

			}
			else
			{
				//int INIT_NUM;//初始化块数
				//cout << "初始化块数:";
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
					//输出初始数据

					//cout << temp << " ";
					NumTemp[*i] = temp;
					(*i)++;
				}

				//cout << endl << "总空闲块数:" << total << endl;
				*NumTotal = total;
			}

		}

		
}

//查看专用块函数
void view(int *i, int STO[],int *FREEBLO,int *TOTALNUM)
{
	//专用块数据如下
	//所存储的空闲块号
	
	for (*i = 0; *i<head->quantity;(*i)++)
	{
		
		//cout << head->cell[i] << " ";
		STO[*i] = head->cell[*i];
	}
	//cout << endl << "专用块空闲块数为:" << head->quantity;
	*FREEBLO = head->quantity;
	//cout << endl << "总空闲块数:" << total << endl;
	*TOTALNUM = total;
}


//新申请函数
void bid(char jobname[], int number,int GteBlo[], int *i,int *CANAPPLY)	//CANAPPL初始化为1
{
	
	job *p;

	

	if (number>total)
	{
		//cout << "所需内存块数大于当前空闲块数,请稍候再试:)" << endl;
		*CANAPPLY = 0;
	}
	else
	{
		p = initial_job();
		strcpy(p->name, jobname);
		p->next = jhead->next;
		jhead->next = p;
		//p->quantity = number;
		
		//cout << "所申请到的空闲块号流:";
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
			else      //组空时转下一组
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

//撤消进程
void finish(char jobname[],int *FIndJob)//FIndJob初始化为1
{
	
	int i;
	job *p, *q;
	group *r;

	//cout << "请输入要撤消的进程名:";
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
		//cout << "Sorry,没有该进程" << endl;
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
			else   //新建一组
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
		p = p->next;	//找到模拟进程
	}

		if (p == NULL)
		{
			//cout << "Sorry,没有该进程" << endl;
			*FIndJob = 0;
			return;
		}

	//job *p;
	

	//计时部分------------------------------------------------------
	time_t start, end;
	start = time(NULL);
	system("pause");	//可以放RunProcess
	end = time(NULL);
	

	int runtime = difftime(end, start);
	//计时部分结束----------------------------------------------------
	
	*CountT = runtime;

	//输出runtime作为内存，提醒计时结束

	if (runtime>total)		//runtime多少就动态申请多少内存，可以改对应函数关系
	{
		//cout << "所需内存块数大于当前空闲块数,请稍候再试:)" << endl;
		*CANAPPLY = 0;
	}
	else
	{
		

		//cout << "所申请到的空闲块号流:";
		int old = p->quantity;//记录原占有内存块数

		int i;//计数器
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
			else      //组空时转下一组
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

		//输出该进程目前占用的所有块
		*num = p->quantity;
		for (i = 0; i < *num; i++)
		{
			OccupyBlo[i] = p->cell[i];
		}
	}

}

//初始化内核数据
void Start()
{
	head = initial();
	total = 0;
	jhead = initial_job();
}





