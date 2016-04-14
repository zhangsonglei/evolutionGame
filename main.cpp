#include "evolution_game.h"
#include <iostream>
using namespace std;

int main()
{
	//MIX = 10全连接网络 LAT = 11网格网络
	//EG2X2 = 24
	//IGA = 33
	//SYNC = 40同步更新
	//M0 = 0, M1 = 1, M2 = 2// the mean changed in mobile game   the number games in a meet .

	Evgame evgame0(MIX, SD, ISD, SYNC, M0); //全连接网络 囚徒博弈 学习次大者 同步更新 移动步数
	Evgame evgame1(LAT, SD, ISD, SYNC, M0); //网格网络 囚徒博弈 学习次大者 同步更新 移动步数


	system("pause>>NULL");
	return 0;
}