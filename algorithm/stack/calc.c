#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#define DEAD(mess){perror(mess);exit(-1);}
#define DEATH(mess)(printf(mess);return 1;)
#define MAX 1024

typedef struct node{
	union{
		char * opd;
		char  opt[2];
		float d;
	};
	int flag;//0 replace degital,1 express symbol,2 express float
}Node;
typedef enum{
	add='+',//1
	sub='-',//1
	multi='*',//2
	divi='/',//2
	left='(',//3,0
	right=')'//0
}symbols;

typedef struct list_node{
	struct list_node *fre;
	Node * n;
	struct list_node *next;
}Nodel;
typedef Nodel *list;
void calculator(list l);
char priority(symbols  sym){
	char  ret;
	switch(sym){
		case add:
		case sub:
			ret='1';
			break;
		case multi:
		case divi:
			ret='2';
			break;
		case left:
			ret= '3';
			break;
		case right:
			ret='0';
			break;
		default:
			ret='4';
			break;		
	}
	return ret;
}
int main(){

	int i=0;//record the pointer of express string
	char express[MAX]={0};//record middle order expree string
	char tmp;//judge whether add to express and how to add to express
	Node exp[MAX];//record express
	int exp_i=-1;//record the top of exp;
	int flag=1;
	Node sym[MAX];
	int sym_i=-1;
	char prio;
	while((tmp=getchar())==' ');
	//express[i]=tmp;
	while(tmp!='='){
		if(tmp<'9'&&tmp>'0')//digital
			if(flag)//the first time to read degital or fre type is symbol
				flag=0,express[i]=tmp,exp[++exp_i].flag=0,exp[exp_i].opd=express+(i++);//push degital to express stack(exp)
			else
				express[i++]=tmp;
		if((symbols)tmp==add||(symbols)tmp==sub||(symbols)tmp==multi||(symbols)tmp==divi||(symbols)tmp==left||(symbols)tmp==right){//symbol
			if(!flag)//digital over
				express[i++]='\0';
			prio=priority(tmp);//translate th symbol`s priority
			if(sym_i<0||sym[sym_i].opt[1]<prio){//push to sym
				if(prio=='3')
					prio='0';
				//push
				sym[++sym_i].flag=1,sym[sym_i].opt[0]=tmp,sym[sym_i].opt[1]=prio;
			}else{//sym.top.proi>=proi
				if(prio!='0'){
					while(sym[sym_i].opt[1]!='0')
						exp[++exp_i].flag=1,exp[exp_i].opt[0]=sym[sym_i--].opt[0],exp[exp_i].opt[1]='\0';
					//push
					sym[++sym_i].flag=1,sym[sym_i].opt[0]=tmp,sym[sym_i].opt[1]=prio;
				}
				else//')'
					while(sym_i>=0)
						if(sym[sym_i].opt[1]!='0')
							exp[++exp_i].flag=1,exp[exp_i].opt[0]=sym[sym_i--].opt[0],exp[exp_i].opt[1]='\0';
						else
							sym_i--;

			}	
			express[i++]=tmp;		
			flag=1;
		}
		if(tmp==' ')
			continue;
		tmp=getchar();			
	}
	if(tmp=='=')
		if(sym_i>=0)
			while(sym_i>=0)
				if(sym[sym_i].opt[1]!='0')
					exp[++exp_i].flag=1,exp[exp_i].opt[0]=sym[sym_i--].opt[0],exp[exp_i].opt[1]='\0';
				else
					sym_i--;
	int j;
	/*for(j=0;j<i;j++){
		printf("%c",express[j]);
	}	
	printf("\n");*/

	list l=NULL;
	//conver exp to linked exp
	for(j=exp_i;j>=0;j--){
		Nodel *nl=calloc(sizeof(Nodel),1);
		if(!exp[j].flag)//digital convert float
			exp[j].flag=2,exp[j].d=strtod(exp[j].opd,NULL);
		nl->n=&exp[j];
		nl->next=l;
		if(l)
			l->fre=nl;
		nl->fre=NULL;			
		l=nl;
	}

	calculator(l);

	while(l){
		if(l->n->flag==1)
			printf("%s\n",l->n->opt);
		else
			printf("%.2f\n",l->n->d);
		l=l->next;
	}
}
void calc(Nodel * node,symbols sym){
	//printf("%.2f\n",node->n->d);
	switch(sym){
		case add:
			node->n->d=node->n->d+node->next->n->d;
			break;
		case sub:
			node->n->d=node->n->d-node->next->n->d;
			break;
		case multi:
			node->n->d=node->n->d*node->next->n->d;
			break;
		case divi:
			node->n->d=node->n->d/node->next->n->d;
			break;
	}
}

void calculator(list l){
	Nodel *tmp;
	Nodel *_tmp;
	while(l){
		if(l->n->flag==1){//symbol
			tmp=l->fre->fre,calc(tmp,(symbols)l->n->opt[0]),_tmp=tmp->next,tmp->next=l->next;
			if(l->next)
				l->next->fre=tmp;
			free(_tmp->next),free(_tmp),l=tmp->next;
		}
		else//float
			l=l->next;
	}

}
