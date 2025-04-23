#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
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

//追加元素
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

//欧几里得距离
void Euclideandistance(LinkedList RD,float AnchorPoint[3])
{
	Node* s = RD->next;
//	s = (Node*)malloc(sizeof(Node));
	while(s->next!=NULL)
	{	
		float tmpD=((AnchorPoint[0]-s->next->data.item1)*(AnchorPoint[0]-s->next->data.item1)+(AnchorPoint[1]-s->next->data.item2)*(AnchorPoint[1]-s->next->data.item2)+ (AnchorPoint[2]-s->next->data.item3)*(AnchorPoint[2]-s->next->data.item3));
		s->next->data.key = sqrt(tmpD);
//		printf("%.f\n",RD->next->data.key);
		s->next=s->next->next;	
	}
//	RD->next = s;
	printf("%d\n",RD->next->data.ID);
	delete s;
}

//街区距离
int blockdistance(LinkedList RD,float AnchorPoint[3])
{
	Node* s;
//	s = (Node*)malloc(sizeof(Node));
	s = RD->next;
	while(RD->next!=NULL)
	{	
		float tmpD=(fabs(AnchorPoint[0]-RD->next->data.item1)+fabs(AnchorPoint[1]-RD->next->data.item2)+fabs(AnchorPoint[2]-RD->next->data.item3));
		RD->next->data.key = tmpD;
//		printf("%.f\n",RD->next->data.key);
		RD->next=RD->next->next;	
	}
	RD->next = s;
	delete s;
	return OK;
}

//余弦距离
int cosdiatance(LinkedList RD)
{
	Node* s;
//	s = (Node*)malloc(sizeof(Node));
	s = RD->next;
	float AnchorPoint[3];
	AnchorPoint[0] = 1;
	AnchorPoint[1] = 0;
	AnchorPoint[2] = 0;
	while(RD->next!=NULL)
	{	
		float tmpD1=(AnchorPoint[0]*RD->next->data.item1 + AnchorPoint[1]*RD->next->data.item2 + AnchorPoint[2]*RD->next->data.item3);
		float tmpD2=(AnchorPoint[0]*AnchorPoint[0]+AnchorPoint[1]*AnchorPoint[1]+AnchorPoint[2]*AnchorPoint[2])*(RD->next->data.item1*RD->next->data.item1+RD->next->data.item2*RD->next->data.item2+RD->next->data.item3*RD->next->data.item3);
		float tmpD=tmpD1/sqrt(tmpD2);
		RD->next->data.key = tmpD;
//		printf("%.6f\n",RD->next->data.key);
		RD->next=RD->next->next;	
	}
	RD->next = s;
	delete s;
//	free(s);
	return OK;
}

void CreateList(LinkedList RD,RecType R[])	//创建顺序表,主要用于单个数值比较，ID值默认为序号，数据项即为key值
{
	Node* s;
	s = RD->next;
//	printf("%f\n",RD->next->data.key);
	for (int i = 1; i <=size; i++)			//R[0..n-1]存放排序记录
	{
		R[i].key = s->next->data.key;
			printf("%f\n",s->next->data.key);
		R[i].ID = s->next->data.ID;
		R[i].item1 = s->next->data.item1;
		R[i].item2 = s->next->data.item2; 
		R[i].item3 = s->next->data.item3;	
		s->next = s->next->next;
	}
	delete s;	
}

void DispList(RecType R[])	//输出顺序表
{

	for (int i = 1; i <= size; i++)
		printf("Key=%f\tID=%d,Item1=%f,Item2=%f,Item3=%f \n",R[i].key,R[i].ID,  R[i].item1, R[i].item2, R[i].item3);
	printf("\n");
}

//int ShellInsert(LinkedList RD, int n) 
//{
////	int dk,i,j;
////	Node* s;
////	s = (Node*)malloc(sizeof(Node));
////	s = RD->next;
////	for(dk=n/2; dk>=1; dk=dk/2) {
////		for(i=dk+1; i<=n; i++) {
////			if(RD[i].data.key<RD[i-dk].data.key) {
////				RD[0].data.key=RD[i].data.key;//哨兵
////				for(j=i-dk; j>0&&RD[0].data.key<RD[j].data.key; j-=dk) {
////					RD[j+dk].data.key=RD[j].data.key;
////				}
////				RD[j+dk].data.key=RD[0].data.key;
////			}
////		}
////	}
//    
//    Node* r = RD->next;
//    Node a,b,c;
//    int gap = n / 2;
//	while (gap > 0) { //每完成一次while循环，代表完成了一次分组后的插入排序
//		for (int i = 0; i <= gap; i++) {
//			for (int j = i+gap; j < n; j += gap) { //对间隔为gap的数组成员进行插入排序
//				if (r[j].data.key < r[j-gap].data.key) {       //对于这一段代码如果没办法很好的理解
//					a=r[j-gap+1];
//					*r[j-gap].next=r[j+1];
//					c=r[j-gap];
//					*r[j-gap-1].next=r[j];
//					b=r[j];
//					*r[j-1].next=c;
//					*b.next=a;
//				}
//			}
//		}
//		gap /= 2;
//	}
//	
//	
////	RD->next = s;
////	delete s;
//	return OK;
//}






//测试主函数
int main()
{	
	LinkedList l;
	createList(l);
	read("data.txt",l);
//	//显示读取文件
//	LinkedList newL = l->next;
//	while(newL!=NULL){
//		printf("%d\t",newL->data.ID);
//		printf("%f\t",newL->data.key);
//		printf("%f\t",newL->data.item1);
//		printf("%f\t",newL->data.item2);
//		printf("%f\n",newL->data.item3);
//		newL = newL->next;
//	}

	//复制链表
	LinkedList RD;
	RD=clone(l);

	//参考点(欧式距离、街区距离的)
	float AnchorPoint[3];
	AnchorPoint[0]=0;
	AnchorPoint[1]=0;
	AnchorPoint[2]=0;
	
	//距离关键字选择
	int nflag;
	printf("请输入比较的关键字类型（0读取文件；1欧式距离；2街区距离；3余弦距离）：");	
	scanf("%d",&nflag);
	if(nflag==1)     	Euclideandistance(RD,AnchorPoint);//欧式距离
	else if(nflag==2)	blockdistance(RD,AnchorPoint);	  //街区距离
	else if(nflag==3)	cosdiatance(RD);	              //余弦距离
	printf("%f\n",RD->next->data.key);
//	//希尔排序
//	ShellInsert(RD,size);
	
//	//显示当前待排序链表
	while(RD->next!=NULL){
		printf("%d\t",RD->next->data.ID);
		printf("%f\t",RD->next->data.key);
		printf("%f\t",RD->next->data.item1);
		printf("%f\t",RD->next->data.item2);
		printf("%f\n",RD->next->data.item3);
		RD->next = RD->next->next;
	}
	//希尔排序
//	ShellInsert(RD,size);
//	printf("%d\n",RD->next);
//	while(RD->next!=NULL){
//		printf("%d\t",RD->next->data.ID);
//		printf("%f\t",RD->next->data.key);
//		printf("%f\t",RD->next->data.item1);
//		printf("%f\t",RD->next->data.item2);
//		printf("%f\n",RD->next->data.item3);
//		RD->next = RD->next->next;
//	}

	//转换为数组
//	RecType R[size+1]={0};
//	CreateList(RD,R);

//	DispList(R);
	
	return OK;
}
