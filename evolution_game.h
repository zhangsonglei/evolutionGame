#ifndef EVG
#define EVG
#define egX 20
#define egY 20

//#include "macro.h"
//#include "mpi.h"
const int ILOOP = 100; // I loop times in main  rerun the whole sytem repeat times
const int JLOOP = 1000; //k loop times in main ,Gene algrithm generations

#include <omp.h>

//��������
#define MIX 10//ȫ��������
#define LAT 11//��������

//��������
#define G2X2 20
#define PD 21     //��ͽ����
#define SD 22     //ѩ�Ѳ���
#define SH 23     //��¹����
#define EG2X2 24
#define GEN 25

//ѧϰ����
#define IMX 30	//ѧϰ����� 
#define IRD 31	//���ѧϰ
#define IDY 32	// replicator dynamic ��̬ѧϰ
#define IGA 33	//�Ŵ��㷨
#define ISD 34	//ѧϰ�δ���

//���Ը��¹���
#define SYNC 40//ͬ������
#define ASYNC 41//�첽����

#define HSKP 50
#define HS25 51
#define HS50 52
#define HS75 53

//�ƶ������ƶ�����
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
	double mst[ILOOP][egX][egY][16];//��Ų���
	//	double stdcr[egX][egY][JLOOP];
	//	double mfg[ILOOP][egX][egY][4096];
	double tpf[ILOOP][egX][egY];//�������

	//	double smcr[egX][egY][JLOOP];
	double smst[egX][egY][16];//���ĳһ�ֲ���
	//	double smfg[egX][egY][4096];
	double stpf[egX][egY];//���ĳһ������

	vector<vector<vector<vector<double> > > > mcr;//��ź�����ʷ
	vector<vector<vector<double> > > smcr;//ĳһ�ֺ�����ʷ
	vector<vector<vector<vector<double> > > > mfg;//��Ų��Եķֲ�����
	vector<vector<vector<double> > > smfg;//���ĳһ�ֲ��Էֲ�
	vector<vector<vector<double> > > stdcr;//ͳ��ƫ��

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