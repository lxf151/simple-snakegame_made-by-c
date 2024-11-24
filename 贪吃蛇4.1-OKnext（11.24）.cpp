#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>

//说明:本程序由dev c++制作并测试,不知道在其他平台运行会不会出什么差错 

/*游戏规则: 
1.不能碰壁,不能碰自己身体 
2.吃 fruit->@ 会使身体边长 
3.w,d,s,a 分别控制 上,右,下,左 
*/
//开始游戏前,建议先切换为英文输入法,避免允许时总是跳出中文输入法框 

/*
新增:允许多个fruit 
改进:使fruit不会贴边出现 
改进:一开始静止不动,游戏不会结束 
改进：使横纵边框等比
改进:引入#define/const,实现一步到位创建数组,在自行修改变量时无需再更改数组 
4.0新增:1.引入 fruit_book 2.加入一种能随方向改变的蛇的头 
debug记录 
	1.解决了蛇身首分家的问题(错误原因:print()函数中book[j][i]搞反了 
	2.解决了蛇碰到自己身体游戏不结束的bug(错误原因: check()函数中book[x][y]搞反了 
	3.解决了 多个fruit时只有一个是真正能用的fruit的bug 
	4.解决了fruit上限只能20个的bug
仍未解决的bug: 
  4.0引入 fruit_book后,不知为何随着游戏进行fruit数量会越来越少 
*/

//----------------以下变量可以自行更改 -----------

//---界面大小---------------------
#define wide 25 // 对应 x坐标 0~24  //亲测100为极限(需要全屏) 
const int high=25; // 对应 y坐标 0~24 	 //亲测50为极限(需要全屏)   //4.1更新:用#define 或 const 都可以 
//--------------------------------
int len=3;//初始长度 
int x=10,y=10;//初始位置  
int add_len=2;//每吃一个fruit增加的长度
int speed=100;//调整移动速度，值越小速度越快（标准值：100） 
const int nfruit=5;//fruit数量   //如果用int nfruit=5；则下面fx[nfruit]的[]中无法为 nfruit，只能是数值 
char snakehead0='Q';//选择你喜欢的字符当蛇的头 

//4.0新增内容:一种时刻朝着运动方向的头
char snakehead1[4]={'^','>','v','<'};
bool type_of_snakehead=0; //若想使用请设为1,不使用请设为0

//----------------以上变量可以自行更改 -----------

int book[wide][high] = {0};
int fruit_book[wide][high] = {0};
int fx[nfruit],fy[nfruit]; //创建fruit的位置存储数组  //如果nfruit>fx[ ]的容量，会有神奇的事情发生 
int dir;//记录当前方向 
int up=1,right=2,down=3,left=4;//将方向与数对应 
int tailx[200]={0},taily[200]={0};

void bfruit(){ //fruit初始化 
    srand((unsigned)time(NULL));
	for(int i=0;i<nfruit;i++){
		fx[i]=rand()%(wide-5)+2;//
		fy[i]=rand()%(high-5)+2;//取模(此时括号内-1):0~24 加2:2~26  需要:2~22  因此,取模:0~20,括号内-5  此时0~19.  2~21
	}
}

void cfruit(int i){  //哪个fruit被吃了就重置哪个 
	srand((unsigned)time(NULL));//!!!因为差这句所以10个fruit碰哪一个位置都没有改变!!! 
	fx[i]=rand()%(wide-5)+2;
	fy[i]=rand()%(high-5)+2;     //才发现忘同步改这里了,怪不得改半天还是没解决fruit贴边的问题 
}

void init_fruit_book(){//4.0内容,初始化fruit_book 
	for(int i=0;i<nfruit;i++){
		fruit_book[fx[i]][fy[i]]=1;
	}
} 
void reset_fruit_book(int i){//fruit被吃,book还原为0 
	fruit_book[fx[i]][fy[i]]=0;
}

void newchange_fruit_book(int i){//被吃的fruit的新坐标对应book变为1 
	fruit_book[fx[i]][fy[i]]=1;
}

int gameover_check(){
	if(x<1||x>=wide-1||y<1||y>=high-1||book[x][y]==1) return 1; //蛇碰到自己身体会死 
	else return 0;		 
}

void fruit_check(){
	for(int i=0;i<nfruit;i++){
		if(x==fx[i]&&y==fy[i]){    //y==fy写错成了x==fy
		reset_fruit_book(i);
		cfruit(i);
		newchange_fruit_book(i);
		len+=add_len;
		}     //错这了!!!if外的return 0 不能在循环里,得等循环结束再return 0 
	}
}

void gotoxy(int x, int y) {
    COORD pos = {x, y};
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}

//int tfruit(int i,int j){//自己加的:判断多个fruit //4.0 已废用:(4.1更新)原因:本身为循环,拖累运行速度,而使用fruit_book数组储存每个坐标对应状态,直接定位查找,无需再遍历,空间换时间,继而允许界面大小和fruit数量能去更大的值 
//	for(int g=0;g<nfruit;g++){
//		if(fx[g]==j&&fy[g]==i)return 1;	//不能用之前那种写法，使return在else里，导致fruit最多20个					
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
				if(dir!=right)dir=left;break;  //加入if,使反方向移动无效 
		}
	}
}

void print(){
	Sleep(speed);
	gotoxy(0,0);
	for(int i=0;i<high;i++){
		for(int j=0;j<wide;j++){
			if(i==0||i==high-1||j==0||j==wide-1)printf(" #");		
			//-----------------------------------------------------------------------------------------------------------若要切换为跳舞蛇,从这里开始以下全部注释掉(全选中后ctrl+/),到下一条分界线 	
			else if(i==y&&j==x&&!type_of_snakehead)printf("%2c",snakehead0);
			else if(i==y&&j==x&&type_of_snakehead)printf("%2c",snakehead1[dir-1]);
			else if(book[j][i]==1)printf(" o");//!!!问题出在这里!!!,book[][]第一个[]表示x值,第二个[]表示y值,而此处的双重for循环,i表示y,j表示x。 
			            //else if(tfruit(i,j))printf(" @");//此处tfruit(i,j)自己加的  //把这条else if放在（" o"）的后面就不会出现@掩盖o的问题了 //4.0 本条语句已废用 
			else if(fruit_book[j][i]==1)printf(" @");//4.0内容,加入 fruit_book[j][i]==1
			
			//------------------------------------------------------------------------------------------------------------------------------------!!!!!!!!!换成下面这段代码会变成跳舞蛇!!!!!!!!
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
			taily[i]=taily[i-1];//刚才写错成了tailx 
	}
	tailx[0]=x;
	taily[0]=y;
	book[tailx[0]][taily[0]]=1;//这里也同样写错了 
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
		if(count>3)flag=gameover_check(); //我也不知道为什么这里数值为大于3就行,1，2就不行 
		if(flag)break;
		fruit_check();
		print();
		control();
		move();
		if(count<=3)count++;//没必要让count一直加下去占用空间 
	}
	printf("游戏结束!\n输入任意值退出......");
	getchar(); 
	return 1;
}

//制作于 2024/11/22/早晨->晚上  by JTZ 跟着b站up“沙猪教写程序”的视频做出来的,除了gotoxy()函数查百度复制的外其余照着教程纯手敲，折腾了一天，写代码 2小时，debug 1整天，好在问题全解决了，完美做出来了，nice! I get it!!! 
//收尾 2024//11/23/夜里1:57
//4.0收尾 2024//11/23/22:05




























