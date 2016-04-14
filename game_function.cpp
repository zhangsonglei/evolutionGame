#include "game_function.h"
//extern vector<vector<int> >  vLadjmx;
int GAgame::ruleupdate(double dg, double dr)//策略更新接口
{
	if (nSyncType == SYNC)      //SYNC=40 同步更新
	{
		switch (nImitType)
		{
		//case 30: imitmaxs(); break;       //无条件学习unconditional imitation
		//case 31: imitrands(); break;      // 随机学习
		//case 32: imitdyns(dg, dr); break; // replicator dynamic 动态学习
		//case 33: imitgas(); break;        //遗传算法
		case 34: imitsecs(); break;       //学习次大者
		default: exit(331);
		}
	}
	else                        //SYNC=41 异步更新
	{
		switch (nImitType)
		{
		//case 30: imitmaxas(); break;    //无条件学习unconditional imitation
		//case 31: imitrandas(); break;   // 随机学习
		//case 32: imitdynas(dg); break;	// replicator dynamic 动态学习
		//case 33: imitgaas(); break;	    //遗传算法
		case 34: imitsecas(); break;    //学习次大者
		default: exit(332);
		}
	}
	return 0;
}


/*
	同步更新 学习次大者
	by 张松磊 201226010427
	2016/04/10
*/
int GAgame::imitsecs()
{
	int i, j, k, max, sec;
	//printf("  imitmaxs  ");
	//Gsize=1000
	for (i = 0; i < Gsize; i++)//For each NODE,we need to do SELECTION,CROSSOVER,AND MUTATION
	{
		max = sec = i;
		if (vLadj[i].size() == 0)
		{
			for (j = 0; j < MEMLEN; j++)//memlength
				MidStrategy[i][j] = Strategy[i][j];
			continue;
		}
		//		if(vLadj[i].size()==0) continue;
		for (j = 0; j < (int)vLadj[i].size(); j++)
		{
			k = vLadj[i][j];
			if (Fitness[max] < Fitness[k])
			{
				sec = max;
				max = k;
			}
			else if (Fitness[sec] < Fitness[k] && Fitness[max] != Fitness[k])
				sec = k;	
		}

		for (j = 0; j < MEMLEN; j++)//memlength
			MidStrategy[i][j] = Strategy[sec][j];
	}//END OF for(i<Gsize)
	//printf("\nstrategt ga \n");
	for (i = 0; i < Gsize; i++)
	{
		for (j = 0; j < MEMLEN; j++)
		{
			Strategy[i][j] = MidStrategy[i][j];

			if (rand() % 100<1)
				Strategy[i][j] = 1 - Strategy[i][j];

			if (Strategy[i][j] > 1)
			{
				printf("strategy wrong at geneticalgrithm %d %d\n", i, j);
				exit(0);
			}
		}
		//	printf("%d%d%d%d\t",Strategy[i][0],Strategy[i][1],Strategy[i][2],Strategy[i][3] );
	}

	return 0;
}

/*
	异步更新 学习次大者
	by 张松磊 201226010427
	2016/04/12
*/
int GAgame::imitsecas()// now not ga  2011 2 11
{
	int i, j, k, max, sec;
	i = Randnode;
	sec = max = i;
	//			if(vLadj[i].size()==0) continue;
	for (j = 0; j < (int)vLadj[i].size(); j++)
	{
		k = vLadj[i][j];
		if (Fitness[max] < Fitness[k])
		{
			sec = max;
			max = k;
		}
		else if (Fitness[sec] < Fitness[k] && Fitness[max] != Fitness[k])
			sec = k;
	}
	for (j = 0; j < MEMLEN; j++)//memlength
	{
		Strategy[i][j] = Strategy[sec][j];
		if (rand() % 100 < 1)
			Strategy[i][j] = 1 - Strategy[i][j];

	}

	//	printf(" imitmaxs over  ");
	return 0;
}

//////////////////////////////////////////////////////////////


double GAgame::getpayoff()// 收益 计算，取各种平均，博弈次数，群体个数
{
	int i;
	double payoff = 0;

	if (Gsize>100)
	{
		for (i = 0; i<Gsize; i++)
		{
			if (vLadj[i].size()>0)
				payoff = payoff + Fitness[i];
		}
		payoff = payoff / (Gsize * 200 * JLOOP);
	}
	else
	{
		for (i = 0; i<Gsize; i++)
		{
			if (vLadj[i].size()>0)
				payoff = payoff + Fitness[i] / vLadj[i].size();

		}
		payoff = payoff / Gsize;
	}
	return payoff;
}

int GAgame::Strtinit()//个体策略初始化
{
	int i, k;

	for (i = 0; i<Gsize; i++)
		for (k = 0; k<MEMLEN; k++)////////////////////////////////////////memlength///////////////
		{
			Strategy[i][k] = (unsigned short)(rand() % 2);
		//	cout<<Strategy[i][k]<<"\t";
		//	if(Strategy[i][k]>1) {printf("strategy wrong at Strategy init i %d k %d\n",i,k); exit (0);}
		}
	return 0;
}

int GAgame::Infoprint()//打印一些信息
{
	int i, j, k;
	for (i = 0; i<Gsize; i++)
	{
		printf("\t node %d ", i);
		for (k = 0; k<MEMLEN; k++)////////////////////////////////////////memlength///////////////
		{
			printf(" %d ", Strategy[i][k]);
			//	if(Strategy[i][k]>1) {printf("strategy wrong at Strategy init i %d k %d\n",i,k); exit (0);}
		}
	}
	printf("\n");
	for (i = 0; i<Gsize; i++)
	{
		printf("\t %d  %lf ", i, Fitness[i]);
	}
	/*	for(i=0;i<Gsize;i++)
	{
	printf("\n %d ",i);
	for(j=0;j<vLadj[i].size();j++)
	printf("%lf ",Pfmx[i][vLadj[i][j]]);
	}
	*/
	printf("history\n");
	for (i = 0; i<Gsize; i++)
		for (j = 0; j<vLadj[i].size(); j++)
			printf("%d %d %d\t", i, vLadj[i][j], (GameHistory[i][vLadj[i][j]]) % 2);

	return 0;
}

int GAgame::Hstinit(double rt)//博弈历史初始化，主要对有记忆情况
{
	int i, j, k, ans1, ans2;
	for (i = 0; i<Gsize; i++)
		for (k = i + 1; k<Gsize; k++)//Some values are meaningless
		{
			for (j = 0; j<MEM; j++)//memlength
			{
				if (rand() % 100 / 100.00<rt)
					ans1 = 1;
				else
					ans1 = 0;

				if (rand() % 100 / 100.00<rt)
					ans2 = 1;
				else
					ans2 = 0;

				GameHistory[i][k] = (GameHistory[i][k] << 2);    //左移操作符
				GameHistory[i][k] = GameHistory[i][k] + ans1 * 2 + ans2;

				GameHistory[k][i] = (GameHistory[k][i] << 2);
				GameHistory[k][i] = GameHistory[k][i] + ans2 * 2 + ans1;
			}
		}
	return 0;
}

int GAgame::Fitinit()//个体适应度水平初始化
{
	Fitness.assign(Gsize, 0.0);
	Pfmx.assign(Gsize, vector<double>(Gsize, 0.0));
	GameCountmx.assign(Gsize, vector<int>(Gsize, 0));
	return 0;
}

GAgame::GAgame(int gamet, int imitt, int synct, int mem, const vector<vector<int> > &vTemp) :vLadj(vTemp)//构造函数，将网络结构通过vTemp传过来
{
	//	vLadj=vTemp;
	Gsize = (int)vLadj.size();
	GAMETYPE = gamet;
	nImitType = imitt;
	nSyncType = synct;
	MEM = mem;     //  printf("ga ok0");

	switch (mem)
	{
	case 0: MEMLEN = 1, GLOOP = 1; break;
	case 1: MEMLEN = 4, GLOOP = 20; break;//if(synctype==41)GLOOP=8;
	case 2: MEMLEN = 16, GLOOP = 40; break;//if(synctype==41)GLOOP=32;
	case 3: MEMLEN = 64, GLOOP = 80; break;//if(synctype==41)GLOOP=32;
	case 4: MEMLEN = 256, GLOOP = 100; break;//if(synctype==41)GLOOP=32;
	case 5: MEMLEN = 1024, GLOOP = 200; break;//if(synctype==41)GLOOP=32;
	case 6: MEMLEN = 4096, GLOOP = 200; break;//if(synctype==41)GLOOP=32;

	default:exit(24);
	}
	//      printf("ga ok");

	Strategy.resize(Gsize, vector<int>(MEMLEN));
	MidStrategy.resize(Gsize, vector<int>(MEMLEN, 0));
	GameHistory.resize(Gsize, vector<unsigned>(Gsize));
	Pfmx.resize(Gsize, vector<double>(Gsize, 0.0));
	Fitness.resize(Gsize, 0);
	//   cout<<Strategy[1][1]<<" ";
	GameCountmx.resize(Gsize, vector<int>(Gsize, 0));
	Strtinit();

	Hstinit(0.5);//初始化合作比0.5
}

GAgame::~GAgame()
{
}

double GAgame::Game(double dg, double dr)//Determine the fitness of each strategy具体博弈  静止模式  
{
	//	printf("  Game dg ");
	double incomeMatrix[2][2], C_Ratio;//收益矩阵，合作水平
	int m, n, i, j, node1, node2;
	int strategy1, strategy2;
	int StrategyCount[2] = { 0, 0 };//StrategyCount[0]背叛者的数量	StrategyCount[1]合作者的数量

	switch (GAMETYPE)
	{
	case 20:
		//for 2x2 incomeMatrix, the subscript variable have the opposite meaning of game stratege,in game 0 means betray,1 means cooperate.
		incomeMatrix[0][0] = 1;                 //R
		incomeMatrix[0][1] = dr - 1;             //S -1~1
		incomeMatrix[1][0] = dg;              //T   0~2  dg gr 0-2
		incomeMatrix[1][1] = 0;                //P
		break;
	case 21:   // pd game
		incomeMatrix[0][0] = 1.0;                 //R             dg 0-1
		incomeMatrix[0][1] = 0.0;            //S
		incomeMatrix[1][0] = dg + 1.0;              //T
		incomeMatrix[1][1] = 0.0;                //P
		break;
	case 22:   // SD
		incomeMatrix[0][0] = 1.0;                 //R
		incomeMatrix[0][1] = 1.0 - dg;            //S
		incomeMatrix[1][0] = 1.0 + dg;              //T
		incomeMatrix[1][1] = 0.0;                //P
		break;
	case 23:   //SH
		incomeMatrix[0][0] = 1.0;                 //R
		incomeMatrix[0][1] = -dg;            //S
		incomeMatrix[1][0] = dg;              //T
		incomeMatrix[1][1] = 0.0;                //P
		break;
	case 24:   //great2x2              dg dr 0-1.5
		incomeMatrix[0][0] = 1.0;                 //R
		incomeMatrix[0][1] = (dr * 10 - 4)*1.0 / 2;            //S     -2-5  14 samples
		incomeMatrix[1][0] = (dg * 10 - 2)*1.0 / 2;              //T   -1-6  14 samples
		incomeMatrix[1][1] = 0.0;                //P
		break;
	case 25: //general
		incomeMatrix[0][0] = 1.0;
		incomeMatrix[0][1] = dg;            //S
		incomeMatrix[1][0] = dr;              //T
		incomeMatrix[1][1] = 0.0;                //P
		break;

	default: exit(29);
	}
	//	Fitinit();
	//	Hstinit();

	if (Gsize>100)// 群体大小一般是1000，小于100 单独处理 ，估计是提高计算速度
	{
		int ti;
		for (m = 0; m<Gsize; m++)
			for (n = 0; n<100; n++)
			{
				node1 = m;
				ti = rand() % (int)vLadj[m].size();      //？？？？？？？？？？？？？？
				node2 = vLadj[m][ti];

				for (i = 0; i<GLOOP; i++)
				{
					strategy1 = Strategy[node1][GameHistory[node1][node2] % MEMLEN];//memlenghth
					strategy2 = Strategy[node2][GameHistory[node2][node1] % MEMLEN];//memlenghth

					Pfmx[node1][node2] = Pfmx[node1][node2] + incomeMatrix[1 - strategy1][1 - strategy2];
					Pfmx[node2][node1] = Pfmx[node2][node1] + incomeMatrix[1 - strategy2][1 - strategy1];
					StrategyCount[strategy1]++;
					StrategyCount[strategy2]++;

					GameHistory[node1][node2] = (GameHistory[node1][node2] << 2);
					GameHistory[node1][node2] = GameHistory[node1][node2] + strategy1 * 2 + strategy2;
					GameHistory[node2][node1] = (GameHistory[node2][node1] << 2);
					GameHistory[node2][node1] = GameHistory[node2][node1] + strategy2 * 2 + strategy1;
				}
			}

		for (i = 0; i<Gsize; i++)
		{
			Fitness[i] = 0;
			for (j = 0; j<Gsize; j++)
				Fitness[i] = Fitness[i] + Pfmx[i][j];
		}
	}

	else
	{
		for (i = 0; i<GLOOP; i++)
		{
			for (m = 0; m<(int)vLadj.size(); m++)
				for (n = 0; n<(int)vLadj[m].size(); n++)
				{
					node1 = m;
					node2 = vLadj[m][n];
					strategy1 = Strategy[node1][GameHistory[node1][node2] % MEMLEN];//memlenghth
					strategy2 = Strategy[node2][GameHistory[node2][node1] % MEMLEN];//memlenghth

					Pfmx[node1][node2] = Pfmx[node1][node2] + incomeMatrix[1 - strategy1][1 - strategy2];
					Pfmx[node2][node1] = Pfmx[node2][node1] + incomeMatrix[1 - strategy2][1 - strategy1];

					StrategyCount[strategy1]++;
					StrategyCount[strategy2]++;

					GameHistory[node1][node2] = (GameHistory[node1][node2] << 2);
					GameHistory[node1][node2] = GameHistory[node1][node2] + strategy1 * 2 + strategy2;
					GameHistory[node2][node1] = (GameHistory[node2][node1] << 2);
					GameHistory[node2][node1] = GameHistory[node2][node1] + strategy2 * 2 + strategy1;
				}
		}

		for (i = 0; i<Gsize; i++)
			for (j = 0; j<vLadj[i].size(); j++)
				Pfmx[i][vLadj[i][j]] /= (2 * GLOOP);//previous' /2'  //conside 2 times count for one game in vladjmx

		for (i = 0; i<Gsize; i++)
		{
			Fitness[i] = 0;
			for (j = 0; j<vLadj[i].size(); j++)
				Fitness[i] = Fitness[i] + Pfmx[i][vLadj[i][j]];
			//		if (vLadj[i].size()>0) Fitness[i]=Fitness[i]/vLadj[i].size();   //(GLOOP*2); 130614
		}

		//	for(i=0;i<Gsize;i++)
		//	if (vLadj[i].size()>0) Fitness[i]=Fitness[i]/(GLOOP*2);////conside the degree effection
	}

	C_Ratio = (double)StrategyCount[1] / (StrategyCount[0] + StrategyCount[1]);

	return C_Ratio;
}