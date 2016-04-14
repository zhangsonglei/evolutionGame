#include "evolution_game.h"
//#include "time.h"
#include "game_function.h"
#include "network_structure.h"
//extern vector<vector<int> >  vLadjmx;
Evgame::Evgame(int nett, int gamet, int imitt, int synct, int meml)
{
	nettype = nett;
	gametype = gamet;
	imittype = imitt;
	synctype = synct;
	mem = meml;

	char *nets, *games, *imits, *syncs; //存放存盘文件名
	switch (nett)
	{
	case 10: nets = "mix"; break;//全连接网络
	case 11: nets = "lat"; break;//网格网络
	default: exit(21);
	}

	switch (gamet)
	{
	case 20:games = "2x2"; break;
	case 21:games = "pd"; break;//囚徒博弈
	case 22:games = "sd"; break;//雪堆博弈
	case 23:games = "sh"; break;//猎鹿博弈
	case 24:games = "eg2x2"; break;
	case 25:games = "gen"; break;

	default: exit(21);
	}

	switch (imitt)
	{
	case 30: imits = "Imx"; break;//学习最大的
	case 31: imits = "Ird"; break;//随机学习
	case 32: imits = "Idy"; break;//动态学习
	case 33: imits = "Iga"; break;//遗传算法
	case 34: imits = "Isd"; break;//学习次大者
	default: exit(21);
	}

	switch (synct)
	{
	case 40: syncs = "Syn"; break;//同步更新
	case 41: syncs = "Asyn"; break;//异步更新
	default: exit(21);
	}

	srand((unsigned)time(0));
	sprintf_s(fhead, 30, "%s%s%s%s%d", nets, games, imits, syncs, mem);//写磁盘文件
	_mkdir(fhead);
	memset(mst, 0, sizeof(mst));
	memset(tpf, 0, sizeof(tpf));
	printf("construct egame ok\n");

	memset(smst, 0, sizeof(smst));
	memset(stpf, 0, sizeof(stpf));
	//ILOOP=100 JLOOP=1000 egX=egY=15
	mcr.resize(ILOOP, vector<vector<vector<double> > >(egX, vector<vector<double> >(egY, vector<double>(JLOOP, 0.0))));
	mfg.resize(ILOOP, vector<vector<vector<double> > >(egX, vector<vector<double> >(egY, vector<double>(4096, 0.0))));
	smcr.resize(egX, vector<vector<double> >(egY, vector<double>(JLOOP, 0.0)));
	smfg.resize(egX, vector<vector<double> >(egY, vector<double>(4096, 0.0)));
	stdcr.resize(egX, vector<vector<double> >(egY, vector<double>(JLOOP, 0.0)));

	switch (nett)
	{
	case 10:
	case 11: evgm();
		dataproc();
		break;
	default: exit(21);
	}

}

int Evgame::evgm()//非移动模式下的演化博弈过程
{
#pragma omp parallel for //private(i,k,m,n,x,y,ni,id,dr,dg,CRatio,node1,node2)

	for (int i = 0; i<ILOOP; i++)
	{
		int id = omp_get_thread_num();
		int k, m, n, x, y=0, ni;
		double dr=0.0, dg, cr;
		double CRatio;
		int     node1;//=TotalVectorArray[i];
		int     node2;//=TotalVectorArray[i+1];

		for (x = 0; x<egX; x++)
		{
			//for (y = 0; y<egY; y++)
			//{
				if (gametype == 25)//遗传算法规则 
				{
					dg = 3;
				//	dr = 1;
				}
				else
				{
					dg = (x + 0.0) / 10.0;
				//  dr = (y + 0.0) / 10.0;
				//				if (x==0) {dg=0.6;dr=1.2;}
				//				if (x==1) {dg=1.0;dr=1.2;}
				//				if (x==2) {dg=1.0;dr=0.8;}
				}

			//  NodeAdj banet(nettype, dr);//dr是移动速度 ,BA网络 
				NodeAdj banet(nettype);
				GAgame gagm(gametype, imittype, synctype, mem, banet.vLadjmx);
				netsize = banet.vLadjmx.size();//网络大小
				printf("run egame");
			//  tsize=vLadjmx.size;ttmplen=gagm.tmplen;
			//  SYN  同步演化博弈

				printf("\nl134");
				gagm.Hstinit(0.5);//初始化历史 ，dr=0.5 
				printf("\nl135\n");
				for (k = 0; k<JLOOP; k++) //game JLOOP times represent the history of cooperation.
				{
				//			gagm.Hstinit(cr);
					gagm.Fitinit();
					CRatio = gagm.Game(dg, dr); // game gloop times
					gagm.ruleupdate(dg, dr);
					if (k % 50 == 0)
					{
						printf("v %lf  b %lf  i %d    k %d    %lf id %d \n", dr, dg, i, k, CRatio, id);
					//							banet.AdjPrint();
					}
					mcr[i][x][y][k] = CRatio;//mpicrh[k]=CRatio; //vmcr.push_back(CRatio);//				
				}//END OF FOR(k=0)

			//			nets=netsize;
				int ttp, ti, tj;
				printf("\n");

				tpf[i][x][y] = gagm.getpayoff();// only use in mixnet!!!!!!!!!!!!!!!2012618
				if (mem != 0) //		if (mem==1)   //mem meaning have changed ,in moving plane game 20121108 
				for (m = 0; m<(int)banet.vLadjmx.size(); m++)
				{//ttp表示4bit的策略用十进制表示 ，共16种，在2X2中 
					ttp = gagm.Strategy[m][0] + gagm.Strategy[m][1] * 2 + gagm.Strategy[m][2] * 4 + gagm.Strategy[m][3] * 8;
					mst[i][x][y][ttp] += 1;//mpist[ttp]+=1;   ////表示该种策略数量加1，统计策略存在量 
					printf("%d%d%d%d\t", gagm.Strategy[m][0], gagm.Strategy[m][1], gagm.Strategy[m][2], gagm.Strategy[m][3]);
				}
			//				getchar();

				int amountcount = 0;
				for (m = 0; m<banet.vLadjmx.size(); m++)//size
					for (n = 0; n<banet.vLadjmx[m].size(); n++)//size=size-1
					{
						node1 = m;
						node2 = banet.vLadjmx[m][n];//m的邻接点 
						amountcount++;
						ti = gagm.GameHistory[node1][node2] % 4096;////2^16,2X2的博弈，存6次博弈历史，记忆长度为6 
						tj = gagm.GameHistory[node2][node1] % 4096;
						mfg[i][x][y][ti] = mfg[i][x][y][ti]++;
						mfg[i][x][y][tj] = mfg[i][x][y][tj]++;

					}

					for (m = 0; m<4096; m++)
						mfg[i][x][y][m] = mfg[i][x][y][m] * 1.0 / (amountcount * 2);

			//}//y end
		}//x end
	}//prarallare end;
	return 0;
}

int Evgame::dataproc()//数据存盘处理
{
	int ii, jj, kk, xx, yy;
	printf("\nmst\n");
	for (ii = 0; ii < ILOOP; ii++)
	{
		//	acrh[ii-1]=mcr;
		for (int xx = 0; xx < egX; xx++)
		{
			int yy = 0;
			//for (int yy = 0; yy<egY; yy++)
			//{
				for (jj = 0; jj < JLOOP; jj++)
				{
					smcr[xx][yy][jj] += mcr[ii][xx][yy][jj];
				}

				for (jj = 0; jj < 16; jj++)
				{
					smst[xx][yy][jj] += mst[ii][xx][yy][jj];
				}

				stpf[xx][yy] += tpf[ii][xx][yy];
				for (jj = 0; jj < 4096; jj++)
					smfg[xx][yy][jj] += mfg[ii][xx][yy][jj];
			//}
		}
	}

	printf("\nsmstILOOP %d,netsize %d \n", ILOOP, netsize);
	for (int xx = 0; xx < egX; xx++)
	{
		int yy = 0;
		//for (int yy = 0; yy < egY; yy++)
		//{
			for (jj = 0; jj < JLOOP; jj++)
			{
				smcr[xx][yy][jj] = smcr[xx][yy][jj] * 1.0 / ILOOP;
			}

			for (jj = 0; jj < 16; jj++)
			{
				//	printf("%lf\t",smst[xx][yy][jj]);
				smst[xx][yy][jj] = smst[xx][yy][jj] * 1.0 / (ILOOP*netsize);
				//	printf("n%lf\t",smst[xx][yy][jj]);
			}

			for (jj = 0; jj < 4096; jj++)
			{
				//	printf("fg%lf\t",smfg[xx][yy][jj]);
				smfg[xx][yy][jj] = smfg[xx][yy][jj] * 1.0 / (ILOOP);
				//	printf("nfg%lf\t",smfg[xx][yy][jj]);
			}

			stpf[xx][yy] = stpf[xx][yy] * 1.0 / ILOOP;
		//}
	}

	writefile();

	return 0;
}

int Evgame::writefile()//存盘
{
	crhp();//合作水平变化历史信息存盘
	//  if (mem==1)fchmp();
	fchmp();//有记忆状态下的最终博弈策略状态存盘
	histp();//最终r，st，p 互惠状态统计，最终博弈的历史状态存盘
	pfp();

	return 0;
}

int Evgame::crhp()//合作水平变化历史信息存盘
{
	char filename[80];

	int x, y=0, i, k;

	sprintf_s(filename, 80, ".//%s//%s", fhead, "crh.txt");
	ofstream fccrh(filename);
	if (!fccrh)
		exit(331);

	for (x = 0; x<egX; x++)
	//	for (y = 0; y<egY; y++)
			for (k = 0; k<JLOOP; k++)
				fccrh << smcr[x][y][k] << " ";
	fccrh.close();
	/*
	sprintf_s(filename,80,".//%s//%s",fhead,"crh00.txt");
	ofstream fccrh00(filename);
	if (!fccrh00)
	exit(331);

	for(i=0;i<ILOOP;i++)
	for(x=0;x<egX;x++)
	for(y=0;y<egY;y++)
	for(k=0;k<JLOOP;k++)
	fccrh00<<mcr[i][x][y][k]<<" ";
	fccrh00.close();
	*/
	sprintf_s(filename, 80, ".//%s//%s", fhead, "stdcrh.txt");
	ofstream fdcrh(filename);
	if (!fdcrh)
		exit(331);

	for (x = 0; x<egX; x++)
		//for (y = 0; y<egY; y++)
			for (k = 0; k<JLOOP; k++)
				fdcrh << stdcr[x][y][k] << " ";
	fdcrh.close();
	return 0;
}

int Evgame::fchmp()//有记忆状态下的最终博弈策略状态存盘
{
	char filename[80];
	int x, y=0, i;
	sprintf_s(filename, 80, ".//%s//%s", fhead, "genest.txt");
	ofstream fcgst(filename);
	if (!fcgst)
		exit(331);
	for (x = 0; x<egX; x++)
		//for (y = 0; y<egY; y++)
			for (i = 0; i<16; i++)
				fcgst << smst[x][y][i] << " ";
	fcgst.close();
	return 0;
}

int Evgame::histp()//最终r，st，p 互惠状态统计，最终博弈的历史状态存盘
{
	char filename[80];
	int x, y=0, i;

	sprintf_s(filename, 80, ".//%s//%s", fhead, "genefr.txt");
	ofstream fcgfr(filename);
	if (!fcgfr)
		exit(331);


	for (x = 0; x<egX; x++)
		//for (y = 0; y<egY; y++)
			for (i = 0; i<4096; i++)
			{
				if (smfg[x][y][i]<0)
				exit(24);//cout<<"hty error";
			//	mfg[x][y][i]=(float)mfg[x][y][i]/(ttmplen*ILOOP);
			fcgfr << smfg[x][y][i] << " ";
			}
	fcgfr.close();

	double hrstp[egX][egY][4];
	memset(hrstp, 0, sizeof(hrstp));
	sprintf_s(filename, 80, ".//%s//%s", fhead, "rstp.txt");
	ofstream fcrstp(filename);
	if (!fcrstp)
		exit(331);

	for (x = 0; x<egX; x++)
	//	for (y = 0; y<egY; y++)
			for (i = 0; i<4096; i++)
				hrstp[x][y][i % 4] += smfg[x][y][i];///////

	for (x = 0; x<egX; x++)
		//for (y = 0; y<egY; y++)
			for (i = 0; i<4; i++)
				fcrstp << hrstp[x][y][i] << " ";
	fcrstp.close();
	return 0;
}

int Evgame::pfp() //最终收益水平存盘
{
	char filename[80];
	int x, y=0, i;
	double pf[egX][egY];

	memset(pf, 0, sizeof(pf));
	sprintf_s(filename, 80, ".//%s//%s", fhead, "pf.txt");
	ofstream fcpf(filename);
	if (!fcpf)
		exit(331);
	for (x = 0; x<egX; x++)
		//for (y = 0; y<egY; y++)
			fcpf << stpf[x][y] << " ";

	fcpf.close();
	return 0;
}