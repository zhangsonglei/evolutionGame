#include "network_structure.h"

vector<vector<int> >  vLadjmx;

//NodeAdj::NodeAdj(int nettype, double movr)
NodeAdj::NodeAdj(int nettype)
{

	switch (nettype)
	{
	case 10:mixbuld(); break;//全连接网络
	case 11:latbuld(); break;//网格
	default: exit(110);
	}

	printf("net ok\n");//网络初始化设置;
}

NodeAdj::~NodeAdj()
{

}

int NodeAdj::mixbuld() //全连接网络
{                        //void resize (size_type n); void resize (size_type n, value_type val);
	//Size = 1000;           
	Size = 400;//改动20160411张松磊
	vLadjmx.resize(Size);//初始化向量
	int i, j;             //
	double tpa, tpb, tpc;
	for (i = 0; i < Size; i++)
	{
		vLadjmx[i].clear();       //void reserve (size_type n)
		vLadjmx[i].reserve(Size); //reserver函数用来给vector预分配存储区大小，即capacity的值 ，但是没有给这段内存进行初始化。
	}                             //reserve 的参数n是推荐预分配内存的大小，实际分配的可能等于或大于这个值，即n大于capacity的值，就会reallocate内存 capacity的值会大于或者等于n 。

	for (i = 0; i < Size; i++)
		for (j = 0; j < Size; j++)
			if (j != i)
				vLadjmx[i].push_back(j);//在向量尾部插入
	//printf("mmat2adj over  ");
	return 0;
}

int NodeAdj::latbuld()//网格
{
	int i, j, k;
	int side = 20;
	Size = side*side;
	vLadjmx.resize(Size);
	//网格，构造首尾相连的网格， 去除边界影响

	for (i = 0; i < Size; i++)
	{
		vLadjmx[i].clear();
		vLadjmx[i].reserve(4);
		vLadjmx[i].push_back((side + i - 1) % side + (i / side)*side);
		vLadjmx[i].push_back((i + 1) % side + (i / side)*side);
		vLadjmx[i].push_back(i%side + ((side + i / side - 1) % side)*side);
		vLadjmx[i].push_back(i%side + ((i / side + 1) % side)*side);
	}

	return 0;
}