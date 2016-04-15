#include <stdio.h>
#define N 100
typedef struct{
	char data;
	int cur;
}SList;
void init_sl(SList slist[]){//��ʼ���ɿ��о�̬����
	int i;
	for(i=0;i<N-1;i++)
	{
		slist[i].cur=i+1;
	}
	slist[N-1].cur=0;//���һ��ָ����ָ��0
}
int malloc_sl(SList slist[]){//������нڵ�
	int i=slist[0].cur;//����ȡͷ���֮��ĵ�һ�����н�������䣬ͬʱ��������ǿգ�ͷ���������
	if (i)
	{
		slist[0].cur=slist[i].cur;//��������ͷ������ָ����
	}
	return i;//�������뵽�Ŀ��нڵ�������±�
}
void free_sl(SList slist[],int k){//��k�ڵ����
	slist[k].cur=slist[0].cur;//���ǽ����յĽڵ����ͷ���֮��
	slist[0].cur=k;
}
int difference_sl(SList slist[],int n){
	int i,m,q,p;
	char tmp[2];//Ϊ�������scanf�����ַ����ֽ��ܻس���������������ַ�������ʽ
	int start,end;//start���ڱ���endָ�����Ľڵ�
	
	init_sl(slist);//��ʼ��
	start=malloc_sl(slist);//�ӿ���������ȡ����һ�����нڵ����������ͷ���
	//ע�����֣���������һ������������slist�����涼�ǿ��нڵ㣬ÿ������malloc_sl��free_sl���ǲ�������slist
	//Ȼ��ǿվ�̬����������A����������start����±�ָ��һֱ����ߣ�endָ��ǿ������β�ڵ㣬
	//�����㷢��start�����϶���1����Ϊ��ʼ��ʱ��slist���ǿ��нڵ㣬�������ֶ��Ǵ�ͷ���slist[0]֮��ĵ�һ��
	//�ڵ㣬��slist[1]��ʼ���䣬����start=1
	end=start;
	while (n--)
	{
		scanf("%s",tmp);
		i=malloc_sl(slist);
		slist[i].data=tmp[0];
		slist[end].cur=i;
		end=i;//endָ�����
	}
	slist[end].cur=0;//�������!β�ڵ��ָ��Ϊ��
	//����A����������ϣ�Ȼ����B����
	scanf("%d",&m);
	while (m--)
	{
		scanf("%s",tmp);
		//��A������ɨ�裬���A�д���tmp������A��ɾ��(free_sl)����A-B�����û���������A����B-A
		q=start;//q��p��ǰ��
		p=slist[start].cur;
		while (p!=slist[end].cur&&slist[p].data!=tmp[0])
		{
			q=p;
			p=slist[p].cur;
		}
		if (p!=slist[end].cur)//˵����A���ҵ���tmp����ɾ��
		{
			slist[q].cur=slist[p].cur;//���p�ڵ�
			free_sl(slist,p);
			if (end==p)
			{
				end=q;//���ɾ������β�ڵ㣬���޸�β�ڵ�ָ��
			}
		}else{
			i=malloc_sl(slist);
			slist[i].data=tmp[0];
			slist[i].cur=slist[end].cur;
			slist[end].cur=i;//����end��endλ�ò��䣬��Ϊend�Ǳ�־����A�Ľ���
		}
	}
	return start;
}
void print_sl(SList slist[],int start){
	int p=slist[start].cur;
	while (p)
	{
		printf("%c ",slist[p].data);
		p=slist[p].cur;
	}
	printf("\n");
}
int main(){
	int n,start;
	SList slist[N];
	freopen("1.txt","r",stdin);
	//�ó�������(A-B)U(B-A)��������
	while (scanf("%d",&n)==1)
	{
		start=difference_sl(slist,n);
		print_sl(slist,start);
	}
	fclose(stdin);
	return 0;
}