// Display.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Core.h"
#include "Display.h"
#pragma comment(lib,"Core.lib")

#define MAXBOL 20500


int main()
{
int f = 1;
int chioce;

Start();
printf(" ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
printf(" ┃　　             　制作信息                   ┃\n");
printf(" ┠──────────────────────────────────────────────┨\n");
printf(" ┃　 　       制作人：袁啸山 彭宇飞　　 　      ┃\n");
printf(" ┃　　　　　　指导老师：田卫东    　　　　    　┃\n");
printf(" ┃　　　　　                       　　　　　　 ┃\n");
printf(" ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
D_readData();

while (f == 1)
{

printf("=====================================================\n" );
printf("*              模拟UNIX的成组链接法                 *\n");
printf("=====================================================\n" );
printf("1.创建进程\n");
printf("2.撤消进程 \n");
printf("3.查看空闲盘块管理节点 \n");
printf("4.模拟时间变化动态\n");
printf("0.退出\n");
printf("=====================================================\n");
printf("请选择:");
scanf("%d", &chioce);
//cin >> chioce;


switch (chioce)
{
case 1:
D_bid();
break;
case 2:
D_finish();
break;
case 3:
D_view();
break;
case 4:
D_SimulateTime();
break;
case 0:
f = 0;
break;
default:
printf("选择错误!\n");
}
}

}



void D_readData()
{

	int INIT_NUM = -1;
	int CAN_OPEN = 1;
	int NumTemp[MAXBOL];//20480为10M，再大可以改
	int NumTotal;
	int i;

	FILE *fp;
	char fname[20];

	printf("请输入初始空闲块数据文件名:");
	scanf("%s", &fname);
	getchar();
	if ((fp = fopen(fname, "rt")) == NULL)
	{
		
		printf("文件无法打开,请检查文件名!");
		
	}
	
	else
	{
		char judge;
		printf("是否初始化（y/n）:");
		scanf("%c",&judge);
		if (judge == 'y')
		{
			printf("输入初始化块数:");
			scanf("%d", &INIT_NUM);
			//INIT_NUM = 1;
		}
		readData(fp, fname, INIT_NUM, &CAN_OPEN, NumTemp, &NumTotal,&i);
		if (CAN_OPEN == 0)
		{
			printf("文件无法打开,请检查文件名!");
		}
		else
		{
			printf("==================================================\n");
			printf("读入的初始空闲块号:\n");

		
			int ii;
			for (ii = 0; ii < i; ii++)
			{
				printf("%d ", NumTemp[ii]);
			}

			printf("\n总空闲块数:%d\n", NumTotal);
		
		}


	}


}

void D_view()
{
	int i;
	int STO[MAXGROUP];
	int FREEBLO;
	int TOTALNUM;

	view(&i, STO, &FREEBLO, &TOTALNUM);

	printf("\n空闲盘块管理节点数据如下:\n");
	printf("==================================================\n");
	printf("所存储的空闲块号:");
	int ii;
	for (ii= 0; ii<i; ii++)
	{

		printf("%d ",STO[ii]);
	}
	 printf("\n空闲盘块管理节点块数为:%d",FREEBLO);

	 printf("\n总空闲块数:%d\n",TOTALNUM);


}

void D_bid()	//CANAPPL初始化为1
{
	char jobname[20];
	int number;
	int GteBlo[20480];
	int i;
	int CANAPPLY = 1;

	
	printf("----------------------------------\n");
	printf("请输入新进程名:");
	scanf("%s", &jobname);
	//cin >> jobname;
	printf("\n所需内存块数:");
	//cout << "所需内存块数:";
	scanf("%d", &number);
	//cin >> number;

	bid(jobname, number, GteBlo, &i, &CANAPPLY);

	if (CANAPPLY==0)
	{
		printf("所需内存块数大于当前空闲块数,请稍候再试!\n");
	}
	else
	{
		printf("所申请到的空闲块号流:");

		int ii;
		for (ii = 0; ii < i; ii++)
		{
			printf("%d ", GteBlo[ii]);
		}
		printf("\n");
	}

	
	
}

//撤消作业
void D_finish()//FIndJob初始化为1
{
	char jobname[20];
	int FIndJob = 1;

	printf("请输入要撤消的进程名:");
	scanf("%s", jobname);

	finish(jobname, &FIndJob);

	if (FIndJob == 0)
		printf("没有该进程");

}



void D_SimulateTime()
{
	
	//SimulateTime(char jobname[], int GteBlo[], int *CANAPPLY, int *FIndJob, int OccupyBlo[], int *num, int *CountT)
		
	char jobname[20];
	int GetBlo[MAXBOL];
	int CANAPPLY = 1;
	int FIndJob = 1;
	int OccupyBlo[MAXBOL];
	int num;
	int CountT;

	printf("----------------------------------\n");
	printf("请输入模拟对象的进程名:");
	scanf("%s", &jobname);

	printf("开始计时，按回车停止\n");
	SimulateTime(jobname, GetBlo, &CANAPPLY, &FIndJob, OccupyBlo, &num, &CountT);
	if (FIndJob==0)
	{
		printf("没有创建该进程！\n");

		//cout << "Sorry,没有该进程" << endl;
		return;
	}
	/*else
	{
		printf("开始计时，按空格停止");
	}*/
	printf("计时结束，经过了%d秒，该进程请求多占用%d个盘块\n",CountT,CountT);
	
	if (CANAPPLY == 0)
	{
		printf("所需内存块数大于当前空闲块数,请稍候再试\n");
		return;
	}
	else
	{
		printf("新申请到的空闲块号流:\n");

		int i;
		for (i = 0; i <CountT ; i++)
		{
			printf("%d ", GetBlo[i]);
		}
		printf("\n");

		printf("进程目前占用的所有块\n");

		//int i;
		for (i = 0; i <CountT; i++)
		{
			printf("%d ", OccupyBlo[i]);
		}
		printf("\n");
		//system("pause");
	}

	

}








