#ifndef EVG
#define EVG
#define egX 20
#define egY 20

//#include "macro.h"
//#include "mpi.h"
const int ILOOP = 100; // I loop times in main  rerun the whole sytem repeat times
const int JLOOP = 1000; //k loop times in main ,Gene algrithm generations

#include <omp.h>

//网络拓扑
#define MIX 10//全连接网络
#define LAT 11//网格网络

//博弈类型
#define G2X2 20
#define PD 21     //囚徒博弈
#define SD 22     //雪堆博弈
#define SH 23     //猎鹿博弈
#define EG2X2 24
#define GEN 25

//学习类型
#define IMX 30	//学习最大者 
#define IRD 31	//随机学习
#define IDY 32	// replicator dynamic 动态学习
#define IGA 33	//遗传算法
#define ISD 34	//学习次大者

//策略更新规则
#define SYNC 40//同步更新
#define ASYNC 41//异步更新

#define HSKP 50
#define HS25 51
#define HS50 52
#define HS75 53

//移动网络移动步数
#define M0 0/// the mean changed in mobile game   the number games in a meet .
#define M1 1
#define M2 2
#define M3 3
#define M4 4
#define M5 5
#define M6 6
#define M7 7
#define M8 8
#define M9 9
#define M10 10

#ifndef LINUX
#include "direct.h"
#include "io.h"
#endif
#include <fstream>

#include <vector>
using namespace std;

#ifndef TTTT
#define TTTT 10

#endif
class Evgame
{

	//	double mcr[ILOOP][egX][egY][JLOOP];
	double mst[ILOOP][egX][egY][16];//存放策略
	//	double stdcr[egX][egY][JLOOP];
	//	double mfg[ILOOP][egX][egY][4096];
	double tpf[ILOOP][egX][egY];//存放收益

	//	double smcr[egX][egY][JLOOP];
	double smst[egX][egY][16];//存放某一轮策略
	//	double smfg[egX][egY][4096];
	double stpf[egX][egY];//存放某一轮收益

	vector<vector<vector<vector<double> > > > mcr;//存放合作历史
	vector<vector<vector<double> > > smcr;//某一轮合作历史
	vector<vector<vector<vector<double> > > > mfg;//存放策略的分布？？
	vector<vector<vector<double> > > smfg;//存放某一轮策略分布
	vector<vector<vector<double> > > stdcr;//统计偏差

	/*
	double mpicr[ILOOP][JLOOP];
	int mpimst[16];
	double mpifg[64];
	double mpipf[ILOOP];
	*/
	int tsize, ttmplen;
	char fhead[30];
	int nettype, gametype, imittype, synctype, mem, netsize;

	int dataproc();
	int  crhp();
	int fchmp();
	int histp();
	int pfp();
public:
	Evgame(int nett, int gamet, int imitt, int synct, int meml);
	//~eVgame();
	int evgm();

	int writefile();


};

#endif