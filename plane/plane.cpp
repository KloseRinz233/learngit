#include<iostream>
#include<cstdlib>
#include<ctime>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
using namespace std;
const int width=20;
const int height=20;
int flag=1;
int score=0;
int enemy_position[2*width][height];
int plane_position_x;
int plane_position_y;
HANDLE hMutex = NULL;
void gotoxy(int x,int y)
{
	COORD loc={x,y};
	HANDLE hOutput=GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput,loc);
}
void HideCursor(int n)
{ 
    CONSOLE_CURSOR_INFO cursor_info={1,n};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
} 
void print(char a,int x,int y)
{
    gotoxy(x,y);
	cout<<a;
	HideCursor(0);
}
void addenemy()
{
	for(int i1=0;i1<height;i1++)
	{
		for(int j1=0;j1<2*width;j1++)
		{
			if(enemy_position[j1][i1]==1) 
				print(' ',j1,i1);
		}
	}
	int x=(rand() % (2*width-2)+1);
	for(int i=height-2;i>=0;i--)
	{
		for(int j=0;j<2*width;j++)
			enemy_position[j][i+1]=enemy_position[j][i];
	}
	for(int k=0;k<2*width;k++)
		enemy_position[k][0]=0;
	enemy_position[x][0]=1;
}
void showenemy()
{
	for(int i=0;i<height;i++)
	{
		for(int j=0;j<2*width;j++)
		{
			if(enemy_position[j][i]==1) 
				print('+',j,i);
		}
	}
}
void clearenemy()
{
	for(int i=0;i<plane_position_y;i++)
	{
		if(enemy_position[plane_position_x][i]==1)
		{
			enemy_position[plane_position_x][i]=0;
			score+=10;
		}
		print(' ',plane_position_x,i);
	}	
}
void printscore()
{
	gotoxy(60,3);
	cout<<"score "<<score;
}
DWORD WINAPI Fun(LPVOID lpParamter)
{
	while(flag)
	{
		WaitForSingleObject(hMutex, INFINITE);
		printscore();
		addenemy();
		showenemy();
		clearenemy();
		if(enemy_position[plane_position_x-1][plane_position_y+1]==1||enemy_position[plane_position_x+1][plane_position_y+1]==1||enemy_position[plane_position_x][plane_position_y+1]==1||enemy_position[plane_position_x][plane_position_y]==1)
			flag=0;
		ReleaseMutex(hMutex);
		Sleep(1000);
	}
    return 0L;
}
class plane
{
	int x;
	int y;
public:
	plane(int x=width,int y=height-2);
	void showplane();
	void hideplane();
	void move(char);
	void saveposition();
};
void plane::showplane()
{
	print('*',x,y);
	print('*',x,y+1);
	print('*',x-1,y+1);
	print('*',x+1,y+1);
}
void plane::hideplane()
{
	print(' ',x,y);
	print(' ',x,y+1);
	print(' ',x-1,y+1);
	print(' ',x+1,y+1);
}
void plane::saveposition()
{
	plane_position_x=x;
	plane_position_y=y;
}
plane::plane(int a,int b)
{
	x=a;y=b;
}
void plane::move(char press)
{
	gotoxy(0,0);
	hideplane();
	switch(press)
	{
	case 'w':case'W' :if(y>=1) y--; break;
	case 's':case'S' :if(y<height-2)y++; break;
	case 'a':case'A' :if(x>1) x--; break;
	case 'd':case'D' :if(x<2*width-2) x++; break;
	}
	gotoxy(0,0);
	showplane();
    saveposition();
}
int main()
{
	HANDLE thread = CreateThread(NULL, 0, Fun, NULL, 0, NULL);
	hMutex = CreateMutex(NULL, FALSE,"screen");
    CloseHandle(thread);
	plane spaceship;
	char press;
	Sleep(1000);
	while(flag)
	{
		WaitForSingleObject(hMutex, INFINITE);
		spaceship.showplane();
		ReleaseMutex(hMutex);
		press=getch();
		if(press=='r'||press=='R')
			flag=0;
		Sleep(200);
		WaitForSingleObject(hMutex, INFINITE);
        spaceship.move(press);
		ReleaseMutex(hMutex);
	}
	system("cls");
	gotoxy(0,0);
	cout<<"you died"<<endl;
	cout<<"your score is "<<score<<endl;
	return 0;
}