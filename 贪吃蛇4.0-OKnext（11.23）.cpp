#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

//˵��:��������dev c++����������,��֪��������ƽ̨���л᲻���ʲô��� 

/*��Ϸ����: 
1.��������,�������Լ����� 
2.�� fruit->@ ��ʹ����߳� 
3.w,d,s,a �ֱ���� ��,��,��,�� 
*/
//��ʼ��Ϸǰ,�������л�ΪӢ�����뷨,��������ʱ���������������뷨�� 

/*
����:������fruit 
�Ľ�:ʹfruit�������߳��� 
�Ľ�:һ��ʼ��ֹ����,��Ϸ������� 
�Ľ���ʹ���ݱ߿�ȱ�
�Ľ�:����#define,ʵ��һ����λ��������,�������޸ı���ʱ�����ٸ������� 
4.0����:1.���� fruit_book 2.����һ�����淽��ı���ߵ�ͷ 
debug��¼ 
	1.����������׷ּҵ�����(����ԭ��:print()������book[j][i]�㷴�� 
	2.������������Լ�������Ϸ��������bug(����ԭ��: check()������book[x][y]�㷴�� 
	3.����� ���fruitʱֻ��һ�����������õ�fruit��bug 
	4.�����fruit����ֻ��20����bug
��δ�����bug: 
  4.0���� fruit_book��,��֪Ϊ��������Ϸ����fruit������Խ��Խ�� 
*/

//----------------���±����������и��� -----------

//---�����С---------------------
#define wide 25 // ��Ӧ x���� 0~24  //�ײ�100Ϊ����(��Ҫȫ��) 
#define high 25 // ��Ӧ y���� 0~24 	 //�ײ�50Ϊ����(��Ҫȫ��) 
//--------------------------------
int len=3;//��ʼ���� 
int x=10,y=10;//��ʼλ��  
int add_len=2;//ÿ��һ��fruit���ӵĳ���
int speed=100;//�����ƶ��ٶȣ�ֵԽС�ٶ�Խ�죨��׼ֵ��100�� 
#define nfruit 5//fruit����   //�����int nfruit=5��������fx[nfruit]��[]���޷�Ϊ nfruit��ֻ������ֵ 
char snakehead0='Q';//ѡ����ϲ�����ַ����ߵ�ͷ 

//4.0��������:һ��ʱ�̳����˶������ͷ
char snakehead1[4]={'^','>','v','<'};
bool type_of_snakehead=0; //����ʹ������Ϊ1,��ʹ������Ϊ0

//----------------���ϱ����������и��� -----------

int book[wide][high] = {0};
int fruit_book[wide][high] = {0};
int fx[nfruit],fy[nfruit]; //����fruit��λ�ô洢����  //���nfruit>fx[ ]��������������������鷢�� 
int dir;//��¼��ǰ���� 
int up=1,right=2,down=3,left=4;//������������Ӧ 
int tailx[100]={0},taily[100]={0};

void bfruit(){ //fruit��ʼ�� 
    srand((unsigned)time(NULL));
	for(int i=0;i<nfruit;i++){
		fx[i]=rand()%(wide-5)+2;//
		fy[i]=rand()%(high-5)+2;//ȡģ(��ʱ������-1):0~24 ��2:2~26  ��Ҫ:2~22  ���,ȡģ:0~20,������-5  ��ʱ0~19.  2~21
	}
}

void cfruit(int i){  //�ĸ�fruit�����˾������ĸ� 
	srand((unsigned)time(NULL));//!!!��Ϊ���������10��fruit����һ��λ�ö�û�иı�!!! 
	fx[i]=rand()%(wide-5)+2;
	fy[i]=rand()%(high-5)+2;     //�ŷ�����ͬ����������,�ֲ��øİ��컹��û���fruit���ߵ����� 
}

void init_fruit_book(){//4.0����,��ʼ��fruit_book 
	for(int i=0;i<nfruit;i++){
		fruit_book[fx[i]][fy[i]]=1;
	}
} 
void reset_fruit_book(int i){//fruit����,book��ԭΪ0 
	fruit_book[fx[i]][fy[i]]=0;
}

void newchange_fruit_book(int i){//���Ե�fruit���������Ӧbook��Ϊ1 
	fruit_book[fx[i]][fy[i]]=1;
}

int gameover_check(){
	if(x<1||x>=wide-1||y<1||y>=high-1||book[x][y]==1) return 1; //�������Լ�������� 
	else return 0;		 
}

void fruit_check(){
	for(int i=0;i<nfruit;i++){
		if(x==fx[i]&&y==fy[i]){    //y==fyд�����x==fy
		reset_fruit_book(i);
		cfruit(i);
		newchange_fruit_book(i);
		len+=add_len;
		}     //������!!!if���return 0 ������ѭ����,�õ�ѭ��������return 0 
	}
}

void gotoxy(int x, int y) {
    COORD pos = {x, y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

//int tfruit(int i,int j){//�Լ��ӵ�:�ж϶��fruit //4.0 �ѷ��� 
//	for(int g=0;g<nfruit;g++){
//		if(fx[g]==j&&fy[g]==i)return 1;	//������֮ǰ����д����ʹreturn��else�����fruit���20��					
//	}
//	return 0;
//}

void control(){
	if(_kbhit()){
		char ch=_getch();
		switch(ch){
			case'w':
			case'W':
				if(dir!=down)dir=up;break;
			case'd':
			case'D':
				if(dir!=left)dir=right;break;
			case's':
			case'S':
				if(dir!=up)dir=down;break;
			case'a':
			case'A':
				if(dir!=right)dir=left;break;  //����if,ʹ�������ƶ���Ч 
		}
	}
}

void print(){
	Sleep(speed);
	gotoxy(0,0);
	for(int i=0;i<high;i++){
		for(int j=0;j<wide;j++){
			if(i==0||i==high-1||j==0||j==wide-1)printf(" #");		
			//-----------------------------------------------------------------------------------------------------------��Ҫ�л�Ϊ������,�����￪ʼ����ȫ��ע�͵�(ȫѡ�к�ctrl+/),����һ���ֽ��� 	
			else if(i==y&&j==x&&!type_of_snakehead)printf("%2c",snakehead0);
			else if(i==y&&j==x&&type_of_snakehead)printf("%2c",snakehead1[dir-1]);
			else if(book[j][i]==1)printf(" o");//!!!�����������!!!,book[][]��һ��[]��ʾxֵ,�ڶ���[]��ʾyֵ,���˴���˫��forѭ��,i��ʾy,j��ʾx�� 
			            //else if(tfruit(i,j))printf(" @");//�˴�tfruit(i,j)�Լ��ӵ�  //������else if���ڣ�" o"���ĺ���Ͳ������@�ڸ�o�������� //4.0 ��������ѷ��� 
			else if(fruit_book[j][i]==1)printf(" @");//4.0����,���� fruit_book[j][i]==1
			
			//------------------------------------------------------------------------------------------------------------------------------------!!!!!!!!!����������δ������������!!!!!!!!
//			else if(i==y&&j==x&&!type_of_snakehead)printf("%c",snakehead0);
//			else if(i==y&&j==x)printf("%c",snakehead0);   
//			else if(book[j][i]==1)printf("o");
//			else if(fruit_book[j][i]==1)printf("@");
		
			//---------------------------------------
			
			else printf("  ");
		}
		printf("\n");
	}
}

void move(){
	if(len>0){
		for(int i=len;i>0;i--){
			tailx[i]=tailx[i-1];
			taily[i]=taily[i-1];//�ղ�д�����tailx 
	}
	tailx[0]=x;
	taily[0]=y;
	book[tailx[0]][taily[0]]=1;//����Ҳͬ��д���� 
	book[tailx[len]][taily[len]]=0;
	}
	switch(dir){
		case 1:y--;break;
		case 3:y++;break;
		case 2:x++;break;
		case 4:x--;break;
		
	}	
}

int main(){
	bfruit();
	init_fruit_book();
	int flag=0;
	int count=0;
	while(1){
		if(count>3)flag=gameover_check(); //��Ҳ��֪��Ϊʲô������ֵΪ����3����,1��2�Ͳ��� 
		if(flag)break;
		fruit_check();
		print();
		control();
		move();
		if(count<=3)count++;//û��Ҫ��countһֱ����ȥռ�ÿռ� 
	}
	printf("��Ϸ����!\n��������ֵ�˳�......");
	getchar(); 
	return 1;
}

//������ 2024/11/22/�糿->����  by JTZ ����bվup��ɳ���д���򡱵���Ƶ��������,����gotoxy()������ٶȸ��Ƶ����������Ž̴̳����ã�������һ�죬д���� 2Сʱ��debug 1���죬��������ȫ����ˣ������������ˣ�nice! I get it!!! 
//��β 2024//11/23/ҹ��1:57
//4.0��β 2024//11/23/22:05




























