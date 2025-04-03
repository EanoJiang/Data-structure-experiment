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

//��������������
typedef struct Node{
	RecType data;
	struct Node* next;
}Node, *LinkedList;

//����Ԫ��
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

//��ȡ�ļ�
int read(char* name,LinkedList& l)
{
	FILE *fp = fopen(name,"r");
	float e[3],key=0;
	int ID;
	if (fp==NULL) 
	{
        puts("���ܴ��ļ���\n");
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

//��ʼ������
void createList(LinkedList& l)
{
	l=(LinkedList)malloc(sizeof(Node));
	l->next=NULL;
}

//���ǵ�����Ծ���ĸ���������ɾ�Ĳ飬���Զ�ȡʱ�ȷ���������
//ŷ����þ���
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

//��������
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

//���Ҿ���
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

//��������
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

//���ǵ��������ǲ���Ҫ������ɾ�Ĳ飬������˳���ȽϷ���
//����ת��Ϊ����
void CreateList(LinkedList RD,RecType R[])	//����˳���,��Ҫ���ڵ�����ֵ�Ƚϣ�IDֵĬ��Ϊ��ţ������Ϊkeyֵ
{
	Node* s = RD;
	for (int i = 0; i <size; i++)			//R[0..n-1]��������¼
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

void DispList(RecType R[])	//���˳���
{

	for (int i = 0; i < size; i++)
		printf("key=%f,ID=%d,item1=%f,item2=%f,item3=%f \n",R[i].key,R[i].ID,  R[i].item1, R[i].item2, R[i].item3);
}

//ϣ������
void ShellSort(RecType R[],int n)
{
	//���飺�����鳤��/2Ϊ��ʼ�����
	//ѭ������ִ��delta/2����һ�η���
	RecType temp;
	int i,j,d;
	for(d=n/2;d>0;d/=2)
	{
		//������������ʽΪ���뷽ʽ
		//i++:�͵�����һ��
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

//ð������
void BubbleSort(RecType R[],int len)
{
    int i;
    int j;
    RecType  temp;
    for(i=0;i<len;i++)//�Ƚϴ���
    {
        for (j = len - 1; j > i; j--)		
        {
            if(R[j-1].key>R[j].key) //�Ƚϴ�С
            {
                temp=R[j];
                R[j]=R[j-1];
                R[j-1]=temp;
            }
        }
		printf("i=%d: ��λԪ��%f\n", i, R[i].key);
//		DispList(R);
    }

}

//��������
int Division(RecType R[],int low,int high)  //�ָ
{
	RecType base=R[low];  //��׼Ԫ�أ�
	while(low<high)
	{
        //�ȴӺ���ǰ�ң��ٴ�ǰ�����
		while(low<high&&R[high].key>=base.key)
			high--;   //���������ҵ�һ���Ȼ�׼ֵС��Ԫ�أ�
		R[low]=R[high];
		while(low<high&&R[low].key<base.key)
			low++;   //���������ҵ�һ���Ȼ�׼ֵ���Ԫ�أ�
		R[high]=R[low];
	}
	R[low]=base;
	return low;
}

 
void QuickSort(RecType R[],int low,int high)
{
	if(low<high)
	{
		int i=Division(R,low,high);  //�ָ
		QuickSort(R,low,i-1);  //�������ֱַ�����
		QuickSort(R,i+1,high);
	}
}

//��ѡ������
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
		printf(" i=%d,ѡ��ؼ���:%f\n", i, R[i].key);
	}
}

//�����ļ�
int saveData(RecType R[],char* name)
{
	FILE* fp = fopen(name, "w");
	if(NULL==fp)  
	{
		printf("�޷����ļ�\n");  
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

//����������
int main()
{	
	int result;
	LinkedList l;
	createList(l);
	read("data.txt",l);

	//�ο���(ŷʽ���롢���������)
	float AnchorPoint[3];
	AnchorPoint[0]=0;
	AnchorPoint[1]=0;
	AnchorPoint[2]=0;
	
	//����ؼ���ѡ��
	int nflag;
	printf("������ȽϵĹؼ������ͣ�0��ȡ�ļ���1ŷʽ���룻2�������룻3���Ҿ��룩��");	
	scanf("%d",&nflag);
	if(nflag==1)     	Euclideandistance(l,AnchorPoint); //ŷʽ����
	else if(nflag==2)	blockdistance(l,AnchorPoint);	  //��������
	else if(nflag==3)	cosdiatance(l);	                  //���Ҿ���

	//��ʾ��ȡ�ļ��������������
	printf("���������ݣ�\nID\tkey\t\titem1\t\titem2\t\titem3\n");
	LinkedList newL = l->next;
	while(newL!=NULL){
		printf("%d\t",newL->data.ID);
		printf("%f\t",newL->data.key);
		printf("%f\t",newL->data.item1);
		printf("%f\t",newL->data.item2);
		printf("%f\n",newL->data.item3);
		newL = newL->next;
	}

	//��������
	LinkedList RD;
	RD=clone(l);
	
	//ת��Ϊ����
	RecType R[size];
	CreateList(RD,R);
	
	//ʱ�Ӷ���
	clock_t start, finish;     //�����һ�ε���CPUʱ�ӵ�λ��ʵ�ʣ��������Ϊ����һ��������
	double Total_time;        //����һ��double���͵ı��������ڴ洢ʱ�䵥λ
   
   	while(1){
   		printf("**********************************\n");
		printf("*    1.ϣ������                  *\n");
		printf("*    2.ð������                  *\n");
		printf("*    3.��������                  *\n");
		printf("*    4.��ѡ������              *\n");
		printf("*    5.�˳�                      *\n");
		printf("**********************************\n");
		printf("��ʼ������ɣ������������\n");
		int input;
		scanf("%d",&input); 
		switch(input){  	
			case 1:
				start = clock();
				ShellSort(R,size);
				finish = clock();
				result = saveData(R,"ϣ��.txt");
				break;
			case 2:
				start = clock();
				BubbleSort(R,size);
				finish = clock();
				result = saveData(R,"ð������.txt");
				break;
	   		case 3:
				start = clock();	
	   			QuickSort(R,0,size-1);
	   			finish = clock();
	   			result = saveData(R,"��������.txt");
	   			break;
	   		case 4:	
	   			start = clock();
	   			SelectSort(R,size);
	   			finish = clock();
	   			result = saveData(R,"��ѡ������.txt");
				break;
			case 5:
				exit(0);
				break;
			default:
				printf("�����������������������\n"); 
				break;					
	    	}
	    	printf("��������ݣ�\n");
	    	DispList(R);
	    	Total_time = (double)(finish - start) / CLOCKS_PER_SEC;
			printf("���������(��ʱ=%f��):\n", Total_time);
			if(result==0) printf("�������,·���뱾����һ��\n"); 
			else printf("��������\n");
	}	    
	return OK;
}
