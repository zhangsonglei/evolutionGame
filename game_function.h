//#include "network.h"
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;
//#include"globev.h"
#include "evolution_game.h"

class GAgame
{
	//DATA  OF GAGAME
public:

	GAgame(int gamet, int imitt, int synct, int mem, const vector<vector<int> > &vTemp);
	~GAgame();
	int Strtinit();
	int Hstinit(double rt = 0.5);
	int Fitinit();

	double Game(double dg, double dr);//
	int Gameas(double dg);
	double Gamemob(double dg, double dr);
	int Gameasmob(double dg);
	int Pfpmob();
	int ruleupdate(double dg, double dr);  //���¹���
	int imitdyns(double dg, double dr);
	int imitmaxs();
	int imitrands();
	int imitgas();
	int imitsecs();//���� ͬ������ ѧϰ�δ���
	int imitdynas(double b);
	int imitmaxas();
	int imitrandas();                     //
	int imitgaas();
	int imitsecas();//���� �첽���� ѧϰ�δ���

	int imitrandomdys(double b);
	double getpayoff();
	int Infoprint();
	double getCratioas();

	vector<vector<unsigned> > GameHistory;//������ʷ
	vector<vector<double> > Pfmx;         //������棿������
	vector<vector<int> > Strategy;        //��ǰ����
	vector<vector<int> > MidStrategy;     //�����µĲ��ԣ�����
	vector<double> Fitness;               //��Ӧ�ȣ�����
	vector<vector<int> > GameCountmx;     //����������������

	const vector<vector<int> > &vLadj;
	int Randnode;
	int Gsize, GAMETYPE, MEM, MEMLEN, GLOOP, nImitType, nSyncType;//game loops in one generation
	//int ILOOP;// I loop times in main  rerun the whole sytem repeat times
	//int JLOOP;//k loop times in main ,Gene algrithm generations
private:
};