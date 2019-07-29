#include <iostream>
#include <windows.h>
#include <conio.h>
using namespace std;

char ci,ch;
int lungime_snake=4,dirx=1,diry,tailx,taily,ok,score,bonusval,BonusIsActive,i;
COORD Position={11,11};
clock_t snaketime,bonustime;

struct snake
{
	int x,y;
}; snake tail[300],mar,bonus;

void ClearBonusLine()
{
	Position.X=12;
	Position.Y=24;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
	cout<<"            ";
	bonus.x=bonus.y=0;
}

void ShowBonusCounter()
{
	Position.X=12;
	Position.Y=24;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position);
	cout<<"Bonus: "<<10-((clock()-bonustime)/(double)CLOCKS_PER_SEC);
}

bool GameIsLost()
{
	if (tail[1].y==0 || tail[1].y==23 || tail[1].x==0 || tail[1].x==79) return 1;
	for (int i=4; i<=lungime_snake; i++)
		if (tail[1].x==tail[i].x && tail[1].y==tail[i].y) return 1;
	return 0;
}

void UpdateScore()
{
	Position.X=7;
	Position.Y=24;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Position); cout<<score;
}

void GrowSnakeTail()
{
	tail[++lungime_snake].x=tail[lungime_snake-1].x;
	tail[lungime_snake].y=tail[lungime_snake-1].y;
}

void CreateNewBonus()
{
	do
	{
		ok=1;
		bonus.x=rand()%78+1;
		bonus.y=rand()%22+1;
		for (int i=1; i<=lungime_snake; i++)
			if (bonus.x==tail[i].x && bonus.y==tail[i].y) ok=0;
		if (bonus.x==mar.x && bonus.y==mar.y) ok=0;
	}while (!ok);
	Position.X=bonus.x;
	Position.Y=bonus.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position); cout<<'@';
}

void CreateNewApple()
{
	do
	{
		ok=1;
		mar.x=rand()%78+1;
		mar.y=rand()%22+1;
		for (int i=1; i<=lungime_snake; i++) 
			if (mar.x==tail[i].x && mar.y==tail[i].y) ok=0;
	}while (!ok);
	Position.X=mar.x;
	Position.Y=mar.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position); cout<<'#';
}

void CheckApple()
{
	if (tail[1].x==mar.x && tail[1].y==mar.y) 
	{
		GrowSnakeTail();
		CreateNewApple();
		score+=5;
		UpdateScore();
		bonusval++;
		if (bonusval%5==0)
		{
			CreateNewBonus();
			BonusIsActive=1;
			bonustime=clock();
		}
	}
	else if (tail[1].x==bonus.x && tail[1].y==bonus.y)
	{
		score+=(int)((10-((clock()-bonustime)/(double)CLOCKS_PER_SEC))*10);
		UpdateScore();
		BonusIsActive=0;
		ClearBonusLine();
	}
}

void MoveSnake()
{
	if (kbhit())
	{
		ch=_getch();
		if (ch==-32)
		{
			ci=_getch();
			if (ci==80 && diry!=-1)
			{
				diry=1;
				dirx=0;
			}
			else if (ci==72 && diry!=1)
			{
				diry=-1;
				dirx=0;
			}
			else if (ci==75 && dirx!=1)
			{
				diry=0;
				dirx=-1;
			}
			else if (ci==77 && dirx!=-1)
			{
				diry=0;
				dirx=1;
			}
		}
	}
	Position.X=tail[lungime_snake].x;
	Position.Y=tail[lungime_snake].y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position); cout<<' ';
	for (int i=lungime_snake; i>=2; i--)
	{
		tail[i].x=tail[i-1].x;
		tail[i].y=tail[i-1].y;
	}
	tail[1].x+=dirx;
	tail[1].y+=diry;
	Position.X=tail[1].x;
	Position.Y=tail[1].y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position); cout<<(char)178;
	snaketime=clock();
}

void SetupNewGame()
{
	cout<<(char)218<<"------------------------------------------------------------------------------"<<(char)191;
	for (i=1; i<=22; i++) cout<<"|                                                                              |";
	cout<<(char)192<<"------------------------------------------------------------------------------"<<(char)217;
	cout<<"Score: "<<score;
	for (i=1; i<=4; i++){ tail[i].x=41-i; tail[i].y=11;}
	for (i=1; i<=lungime_snake; i++)
	{
		Position.X=tail[i].x;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position); cout<<(char)178;
	}
	srand(clock());
	CreateNewApple();
	snaketime=clock();
}

int main()
{
	SetupNewGame();
	while (1)
		if ((clock()-snaketime)/(double)CLOCKS_PER_SEC>=0.1) 
		{
			MoveSnake();
			CheckApple();
			if (BonusIsActive) ShowBonusCounter();
			if (GameIsLost()){ Sleep(3000); exit(0);}
		}
		else if ((clock()-bonustime)/(double)CLOCKS_PER_SEC>=10 && BonusIsActive)
		{
			Position.X=bonus.x;
			Position.Y=bonus.y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Position); cout<<' ';
			BonusIsActive=0;
			ClearBonusLine();
		}
}
