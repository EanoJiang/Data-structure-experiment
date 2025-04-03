#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<time.h>
#define ERROR -1
#define OK 0
#define size 1000

typedef struct{
	int ID;
	float key;
	float item1;
	float item2;
	float item3;
}RecType;

//定义链表结点类型
typedef struct Node{
	RecType data;
	struct Node* next;
}Node, *LinkedList;

//插入元素
int listAppend(LinkedList& l,float e[],int ID,float key)
{
	LinkedList p = l,s;
	if(p==NULL){
		return ERROR;
	}else{
		while(p->next!=NULL){
			p = p->next;
		}
		s = (LinkedList)malloc(sizeof(Node));
		s->data.ID = ID;
		s->data.key = key;
		s->data.item1 = e[0];
		s->data.item2 = e[1];
		s->data.item3 = e[2];
		s->next = NULL;
		p->next = s;
		return OK;
	}
}

//读取文件
int read(char* name,LinkedList& l)
{
	FILE *fp = fopen(name,"r");
	float e[3],key=0;
	int ID;
	if (fp==NULL) 
	{
        puts("不能打开文件！\n");
        return ERROR;
    }
    while(!feof(fp))
    {
    	fscanf(fp,"%d,\t",&ID, sizeof(int));
		fscanf(fp,"%f,\t",&key, sizeof(float));
		fscanf(fp,"%f,\t",&e[0], sizeof(float));
		fscanf(fp,"%f,\t",&e[1], sizeof(float));
		fscanf(fp,"%f\n",&e[2], sizeof(float));
		listAppend(l,e,ID,key);
    }
    fclose(fp);
    return OK;
}

//初始化链表
void createList(LinkedList& l)
{
	l=(LinkedList)malloc(sizeof(Node));
	l->next=NULL;
}

//考虑到方便对距离的各个参数增删改查，所以读取时先放在链表里
//欧几里得距离
void Euclideandistance(LinkedList l,float AnchorPoint[3])
{
	Node* s = l;
	while(l!=NULL)
	{	
//		s = (LinkedList)malloc(sizeof(Node));
		float tmpD=((AnchorPoint[0]-l->data.item1)*(AnchorPoint[0]-l->data.item1)+(AnchorPoint[1]-l->data.item2)*(AnchorPoint[1]-l->data.item2)+ (AnchorPoint[2]-l->data.item3)*(AnchorPoint[2]-l->data.item3));
		l->data.key = sqrt(tmpD);
		l=l->next;	
	}
	l = s;
	delete s;
}

//街区距离
void blockdistance(LinkedList l,float AnchorPoint[3])
{
	Node* s = l;
	while(l!=NULL)
	{	
		float tmpD=(fabs(AnchorPoint[0]-l->data.item1)+fabs(AnchorPoint[1]-l->data.item2)+fabs(AnchorPoint[2]-l->data.item3));
		l->data.key = tmpD;
		l=l->next;	
	}
	l = s;
	delete s;
}

//余弦距离
void cosdiatance(LinkedList l)
{
	Node* s=l;
	float AnchorPoint[3];
	AnchorPoint[0] = 1;
	AnchorPoint[1] = 0;
	AnchorPoint[2] = 0;
	while(l!=NULL)
	{	
		float tmpD1=(AnchorPoint[0]*l->data.item1 + AnchorPoint[1]*l->data.item2 + AnchorPoint[2]*l->data.item3);
		float tmpD2=(AnchorPoint[0]*AnchorPoint[0]+AnchorPoint[1]*AnchorPoint[1]+AnchorPoint[2]*AnchorPoint[2])*(l->data.item1*l->data.item1+l->data.item2*l->data.item2+l->data.item3*l->data.item3);
		float tmpD=tmpD1/sqrt(tmpD2);
		l->data.key = tmpD;
		l=l->next;	
	}
	l = s;
	delete s;
}

//复制链表
LinkedList clone(LinkedList l) {
	LinkedList RD;
	RD = (LinkedList)malloc(sizeof(Node));
	Node *s, *r = RD, *p = l->next;
	while (p != NULL) {
		s = (LinkedList)malloc(sizeof(Node));
		s->data = p->data;
		r->next = s;
		r = s;
		p = p->next;
	}
	delete s,r,p;
	r->next = NULL;
	return RD;
}

//考虑到在排序是不需要进行增删改查，所以用顺序表比较方便
//链表转化为数组
void CreateList(LinkedList RD,RecType R[])	//创建顺序表,主要用于单个数值比较，ID值默认为序号，数据项即为key值
{
	Node* s = RD;
	for (int i = 0; i <size; i++)			//R[0..n-1]存放排序记录
	{
		R[i].key = RD->next->data.key;
		R[i].ID = RD->next->data.ID;
		R[i].item1 = RD->next->data.item1;
		R[i].item2 = RD->next->data.item2; 
		R[i].item3 = RD->next->data.item3;	
		RD->next = RD->next->next;
	}
	RD = s;
	delete s;	
}

void DispList(RecType R[])	//输出顺序表
{

	for (int i = 0; i < size; i++)
		printf("key=%f,ID=%d,item1=%f,item2=%f,item3=%f \n",R[i].key,R[i].ID,  R[i].item1, R[i].item2, R[i].item3);
}

//希尔排序
void ShellSort(RecType R[],int n)
{
	//分组：以数组长度/2为起始分组点
	//循环结束执行delta/2：再一次分组
	RecType temp;
	int i,j,d;
	for(d=n/2;d>0;d/=2)
	{
		//组内排序：排序方式为插入方式
		//i++:就到了下一组
		for(i=d;i<n;i++)
		{
			temp=R[i];
			for(j=i-d;j>=0;j-=d)
			{
				if(temp.key<R[j].key)
					R[j+d]=R[j];
				else
					break;
			}
			R[j+d]=temp;
		}
		printf("d=%d: \n", d);
//		DispList(R);
	}
}

//冒泡排序
void BubbleSort(RecType R[],int len)
{
    int i;
    int j;
    RecType  temp;
    for(i=0;i<len;i++)//比较次数
    {
        for (j = len - 1; j > i; j--)		
        {
            if(R[j-1].key>R[j].key) //比较大小
            {
                temp=R[j];
                R[j]=R[j-1];
                R[j-1]=temp;
            }
        }
		printf("i=%d: 归位元素%f\n", i, R[i].key);
//		DispList(R);
    }

}

//快速排序
int Division(RecType R[],int low,int high)  //分割；
{
	RecType base=R[low];  //基准元素；
	while(low<high)
	{
        //先从后向前找，再从前向后找
		while(low<high&&R[high].key>=base.key)
			high--;   //从右向左找第一个比基准值小的元素；
		R[low]=R[high];
		while(low<high&&R[low].key<base.key)
			low++;   //从左向右找第一个比基准值大的元素；
		R[high]=R[low];
	}
	R[low]=base;
	return low;
}

 
void QuickSort(RecType R[],int low,int high)
{
	if(low<high)
	{
		int i=Division(R,low,high);  //分割；
		QuickSort(R,low,i-1);  //将两部分分别排序；
		QuickSort(R,i+1,high);
	}
}

//简单选择排序
void SelectSort(RecType R[],int n)
{
	for(int i=0;i<n;i++)
	{
		int min=i;
		for(int j=i+1;j<n;j++){
			if(R[j].key<R[min].key){
				min=j;
			}
		}	
		if(min!=i){
			RecType t=R[i];
			R[i]=R[min];
			R[min]=t;
		}
		printf(" i=%d,选择关键字:%f\n", i, R[i].key);
	}
}

//保存文件
int saveData(RecType R[],char* name)
{
	FILE* fp = fopen(name, "w");
	if(NULL==fp)  
	{
		printf("无法打开文件\n");  
		return ERROR;
	}  
	fprintf(fp,"key\tID\titem1\titem2\titem3\t\n");
    for(int i=0;i<size;i++)
	{
		fprintf(fp,"%10f %10d %20f %20f %20f;", R[i].key,R[i].ID,R[i].item1,R[i].item2,R[i].item3);						
		fprintf(fp,"\n");
	}
	fclose(fp);
	return OK;
}

//测试主函数
int main()
{	
	int result;
	LinkedList l;
	createList(l);
	read("data.txt",l);

	//参考点(欧式距离、街区距离的)
	float AnchorPoint[3];
	AnchorPoint[0]=0;
	AnchorPoint[1]=0;
	AnchorPoint[2]=0;
	
	//距离关键字选择
	int nflag;
	printf("请输入比较的关键字类型（0读取文件；1欧式距离；2街区距离；3余弦距离）：");	
	scanf("%d",&nflag);
	if(nflag==1)     	Euclideandistance(l,AnchorPoint); //欧式距离
	else if(nflag==2)	blockdistance(l,AnchorPoint);	  //街区距离
	else if(nflag==3)	cosdiatance(l);	                  //余弦距离

	//显示读取文件（待排序的链表）
	printf("待排序数据：\nID\tkey\t\titem1\t\titem2\t\titem3\n");
	LinkedList newL = l->next;
	while(newL!=NULL){
		printf("%d\t",newL->data.ID);
		printf("%f\t",newL->data.key);
		printf("%f\t",newL->data.item1);
		printf("%f\t",newL->data.item2);
		printf("%f\n",newL->data.item3);
		newL = newL->next;
	}

	//复制链表
	LinkedList RD;
	RD=clone(l);
	
	//转换为数组
	RecType R[size];
	CreateList(RD,R);
	
	//时钟定义
	clock_t start, finish;     //定义第一次调用CPU时钟单位的实际，可以理解为定义一个计数器
	double Total_time;        //定义一个double类型的变量，用于存储时间单位
   
   	while(1){
   		printf("**********************************\n");
		printf("*    1.希尔排序                  *\n");
		printf("*    2.冒泡排序                  *\n");
		printf("*    3.快速排序                  *\n");
		printf("*    4.简单选择排序              *\n");
		printf("*    5.退出                      *\n");
		printf("**********************************\n");
		printf("初始化已完成，请输入操作：\n");
		int input;
		scanf("%d",&input); 
		switch(input){  	
			case 1:
				start = clock();
				ShellSort(R,size);
				finish = clock();
				result = saveData(R,"希尔.txt");
				break;
			case 2:
				start = clock();
				BubbleSort(R,size);
				finish = clock();
				result = saveData(R,"冒泡排序.txt");
				break;
	   		case 3:
				start = clock();	
	   			QuickSort(R,0,size-1);
	   			finish = clock();
	   			result = saveData(R,"快速排序.txt");
	   			break;
	   		case 4:	
	   			start = clock();
	   			SelectSort(R,size);
	   			finish = clock();
	   			result = saveData(R,"简单选择排序.txt");
				break;
			case 5:
				exit(0);
				break;
			default:
				printf("输入操作项有误，请重新输入\n"); 
				break;					
	    	}
	    	printf("排序后数据：\n");
	    	DispList(R);
	    	Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
			printf("插入排序后(耗时=%f秒):\n", Total_time);
			if(result==0) printf("保存完成,路径与本程序一致\n"); 
			else printf("保存有误\n");
	}	    
	return OK;
}
