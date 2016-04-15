#include <stdio.h>
#define N 100
typedef struct{
	char data;
	int cur;
}SList;
void init_sl(SList slist[]){//初始化成空闲静态链表，
	int i;
	for(i=0;i<N-1;i++)
	{
		slist[i].cur=i+1;
	}
	slist[N-1].cur=0;//最后一个指针域指向0
}
int malloc_sl(SList slist[]){//分配空闲节点
	int i=slist[0].cur;//总是取头结点之后的第一个空闲结点做分配，同时空闲链表非空，头结点做调整
	if (i)
	{
		slist[0].cur=slist[i].cur;//空闲链表头结点调整指针域
	}
	return i;//返回申请到的空闲节点的数组下标
}
void free_sl(SList slist[],int k){//将k节点回收
	slist[k].cur=slist[0].cur;//总是将回收的节点放在头结点之后
	slist[0].cur=k;
}
int difference_sl(SList slist[],int n){
	int i,m,q,p;
	char tmp[2];//为避免出现scanf输入字符出现接受回车符，则采用输入字符串的形式
	int start,end;//start是哨兵，end指向最后的节点
	
	init_sl(slist);//初始化
	start=malloc_sl(slist);//从空闲链表中取出第一个空闲节点生成链表的头结点
	//注意区分，现在是有一个空闲链表是slist，里面都是空闲节点，每次申请malloc_sl和free_sl都是操作的是slist
	//然后非空静态链表，即集合A的链表是由start这个下标指引一直向后走，end指向非空链表的尾节点，
	//而且你发现start基本上都是1，因为开始的时候slist都是空闲节点，而分配又都是从头结点slist[0]之后的第一个
	//节点，即slist[1]开始分配，所以start=1
	end=start;
	while (n--)
	{
		scanf("%s",tmp);
		i=malloc_sl(slist);
		slist[i].data=tmp[0];
		slist[end].cur=i;
		end=i;//end指针后移
	}
	slist[end].cur=0;//这个勿忘!尾节点的指针为空
	//至此A集合输入完毕，然后处理B集合
	scanf("%d",&m);
	while (m--)
	{
		scanf("%s",tmp);
		//从A集合中扫描，如果A中存在tmp，则在A中删除(free_sl)，即A-B，如果没有则添加入A，即B-A
		q=start;//q是p的前驱
		p=slist[start].cur;
		while (p!=slist[end].cur&&slist[p].data!=tmp[0])
		{
			q=p;
			p=slist[p].cur;
		}
		if (p!=slist[end].cur)//说明在A中找到了tmp，则删除
		{
			slist[q].cur=slist[p].cur;//跨过p节点
			free_sl(slist,p);
			if (end==p)
			{
				end=q;//如果删除的是尾节点，则修改尾节点指针
			}
		}else{
			i=malloc_sl(slist);
			slist[i].data=tmp[0];
			slist[i].cur=slist[end].cur;
			slist[end].cur=i;//插在end后，end位置不变，因为end是标志集合A的结束
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
	//该程序是求(A-B)U(B-A)集合运算
	while (scanf("%d",&n)==1)
	{
		start=difference_sl(slist,n);
		print_sl(slist,start);
	}
	fclose(stdin);
	return 0;
}