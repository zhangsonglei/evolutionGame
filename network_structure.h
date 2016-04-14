//#include <iostream>
#include "evolution_game.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
using namespace std;

//	static const int SIZE=100;///important!! range of rand is 0-32767,size can not be too large!!! number of nodes?
//	static const int NMAX=SIZE-1;               //degree of nodes
//	static const int tmplen=SIZE*NMAX;               //=totalvector array

class NodeAdj
{
private:
public:

	int Size, iSide;///important!! range of rand is 0-32767,size can not be too large!!! number of nodes?
	double dSide;

	//NodeAdj(int nettype, double movr);
	NodeAdj(int nettype);
	~NodeAdj();//	VNode *p;

	double Nratio;// node ratio  密度

	int mixbuld();//全连接网络
	int latbuld();//网格
	//vector<vector<int> > vLmatrix;
	vector<vector<double> > vLaxis;
	vector<vector<int> > vLadjmx;
	int AdjPrint();
};