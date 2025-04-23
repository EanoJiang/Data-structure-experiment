#include <stdio.h>
#include "time.h"
#include <stdlib.h>

int main()
{
	int size = 1000;
	int ID[size];
    float data[size][4];
    int i,j;
    int m=1000;
    for(i=0;i<size;i++){
    	ID[i]=m;
    	m--;
	}
    srand(time(0));
    for(i=0; i<size; i++){
    	data[i][0] = (float)i+1;
        data[i][1] = 1.0+1000.0*(rand()%RAND_MAX)/RAND_MAX *(1-0);	//设为RAND_MAX,随机效果更好
        data[i][2] = 1.0+1000.0*(rand()%RAND_MAX)/RAND_MAX *(1-0);	//设为RAND_MAX,随机效果更好
		data[i][3] = 1.0+1000.0*(rand()%RAND_MAX)/RAND_MAX *(1-0);	//设为RAND_MAX,随机效果更好
    }
    FILE*fp=fopen("data.txt","w");
	for(i=0; i<size; i++){
    	printf("%d,",ID[i]);
		fprintf(fp,"%d,\t",ID[i]);				
    	for(int j=0;j<4;j++){
        	printf("%.6f, ", data[i][j]);					       	
		}
		fprintf(fp,"%f,\t",data[i][0]); 
		fprintf(fp,"%f,\t",data[i][1]); 
		fprintf(fp,"%f,\t",data[i][2]); 
		fprintf(fp,"%f\t",data[i][3]); 				
		printf("\n");
		fprintf(fp,"\n");		
	}
	fclose(fp);
    return 0;
}


