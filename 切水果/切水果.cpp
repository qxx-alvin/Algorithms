#include <iostream>
#include <vector>
#include <set>
#include <stack>
using namespace std;

typedef struct{
	int x;   // 0~39
	int y;		// 0~49
	bool exist;  // 该水果是否存在于当前图中
} Position;  // 水果位置

typedef enum {
	Horizontal,
	Vertical,
	RightDown,  // 左上到右下斜
	LeftDown	// 右上到左下斜
} CurDirection;   // 刀法方向

typedef struct{
	int value;  // 对不同刀法，该字段有不同解释，见CutCrossPoint函数
	CurDirection dir;
	bool exist;  // 该刀法是否存在于当前刀法集合中
} Cut;  // 一个刀法


typedef struct _CutTree{
	Cut c;
	int nCut;  // 从这个刀法下去，最终总刀数
	struct _CutTree* branchs[4];
}CutTree;   // 刀法构成的树

CutTree cTree;

// 根据Position和CurDirection构造Cut
Cut cutFromPosDir(Position p, CurDirection cd)
{
	Cut c;
	c.exist = 1;
	c.dir = cd;
	switch (cd)
	{
	case Horizontal:
		c.value = p.y; break;
	case Vertical:
		c.value = p.x; break;
	case RightDown:
		c.value = p.x - p.y; break;
	case LeftDown:
		c.value = p.x + p.y; break;
	}
	return c;
}

// 判断刀法c是否过点p
bool CutCrossPoint(Position &p, Cut &c)
{
	switch (c.dir)
	{
	case Horizontal:
		return p.y == c.value;
	case Vertical:
		return p.x == c.value;
	case RightDown:
		return p.x - p.y == c.value;
	case LeftDown:
		return p.x + p.y == c.value;
	}
}

// 判断过p1的刀法dir是否也过点p2
bool CutCrossPoint(Position &p1, Position &p2, CurDirection dir)
{
	switch (dir)
	{
	case Horizontal:
		return p1.y == p2.y;
	case Vertical:
		return p1.x == p2.x;
	case RightDown:
		return p1.x - p1.y == p2.x - p2.y;
	case LeftDown:
		return p1.x + p1.y == p2.x + p2.y;
	}
}


// 用于刀法的set
class myless_than
{
public:
	bool operator()(const Cut& c1, const Cut& c2) const//也得是const的
	{
		if (c1.dir == c2.dir)
			return c1.value < c2.value;
		else
			return c1.dir < c2.dir;
	}
};

// 方法一：基于所有刀法集合
int solu(vector<Position> &positions, vector<Cut> &cuts)
{
	vector<bool> hasBeenCut(positions.size(), 0);  // 尝试每种刀法的时候，各个水果是否被切
	int minCuts = positions.size();   // 最小刀数

	// 是否所有水果都被切完 
	bool finish = true;  
	for (int i = 0; i < positions.size(); i++)
		if (positions[i].exist)
		{
			finish = false;
			break;
		}
	if (finish)
		return 0;

	// 遍历每个刀法
	for (int i = 0; i < cuts.size(); i++)
	{
		if (!cuts[i].exist)   // 仅看当前刀法集合
			continue; 

		//for (int i = 0; i < hasBeenCut.size(); i++)
		//	hasBeenCut[i] = 0;
		// 去掉该刀法所经过的水果
		for (int j = 0; j < positions.size(); j++)
		{
			if (positions[j].exist && CutCrossPoint(positions[j], cuts[i]))
			{
				positions[j].exist = false;
				hasBeenCut[j] = 1;
			}
		}
		// 去掉该刀法
		cuts[i].exist = 0;  

		// 递归
		int nSub = solu(positions, cuts);
		// 更新最小刀数
		if (nSub + 1 < minCuts)
			minCuts = nSub + 1;
		
		// 把刀法和水果复原
		cuts[i].exist = 1;
		for (int j = 0; j < hasBeenCut.size(); j++)
		{
			if (hasBeenCut[j])
			{
				hasBeenCut[j] = 0;
				positions[j].exist = true;
			}
		}
	}
	return minCuts;

}

// 方法二：基于首个位置的刀法集合
int solu2(vector<Position> positions, CutTree* curRoot)
{
	int minCuts = positions.size();   // 最小刀数

	curRoot->branchs[0] = NULL;				// 路径终止判别依据
	if (positions.empty())
		return 0;

	
	
	// 遍历第一个节点的4个刀法
	Position p = positions[0];
	for (int i = 0; i < 4; i++)
	{
		CurDirection cd = (CurDirection)i;
		vector<Position> tmp_positions = positions;

		// 去掉该刀法所经过的水果
		for (int j = 0; j < tmp_positions.size(); j++)
		{
			if (CutCrossPoint(p, tmp_positions[j], cd))
			{
				tmp_positions.erase(tmp_positions.begin() + j);
				j--; 
			}
		}

		// 生成一个分支
		CutTree *ct = (CutTree*)malloc(sizeof(CutTree));
		ct->c = cutFromPosDir(p, cd);
		curRoot->branchs[i] = ct;

		// 递归
		int nSub = solu2(tmp_positions, ct);
		ct->nCut = nSub + 1;

		// 更新最小刀数
		if (nSub + 1 < minCuts)
			minCuts = nSub + 1;


	}
	return minCuts;
}

int main()
{
	// 输入
	int N;
	cin >> N;
	vector<Position> positions;
	for (int i = 0; i < N; i++)
	{
		int x, y;
		cin >> x >> y;
		positions.push_back({ x, y, true });
	}
	
	/*********** 方法1 ***********/
	// 寻找所有刀法（每个水果的四种刀法集合）
	//set<Cut, myless_than> cutSet;
	//for (int i = 0; i < positions.size(); i++)
	//{
	//	Cut c;
	//	c.exist = true;
	//	// 四种刀法
	//	c.dir = Horizontal;
	//	c.value = positions[i].y;
	//	cutSet.insert(c);

	//	c.dir = Vertical;
	//	c.value = positions[i].x;
	//	cutSet.insert(c);

	//	c.dir = RightDown;
	//	c.value = positions[i].x - positions[i].y;
	//	cutSet.insert(c);

	//	c.dir = LeftDown;
	//	c.value = positions[i].x + positions[i].y;
	//	cutSet.insert(c);
	//}
	//vector<Cut> cutVec;   // 用vector，不然后面不好操作
	//set<Cut>::iterator it;
	//for (it = cutSet.begin(); it != cutSet.end(); it++)
	//	cutVec.push_back(*it);
	//// 求解
	//cout << solu(positions, cutVec) << endl;

	/*********** 方法2 ***********/
	cout << solu2(positions, &cTree) << endl;
	// 打印刀法组合
	CutTree*c = &cTree;
	while (c->branchs[0])
	{
		// 找最小刀数的刀法
		int minNum = c->branchs[0]->nCut;
		int minInd = 0;
		for (int i = 1; i < 4; i++)
		{
			if (c->branchs[i]->nCut < minNum)
			{
				minNum = c->branchs[i]->nCut;
				minInd = i;
			}
		}
		// 打印
		switch (c->branchs[minInd]->c.dir)
		{
		case Horizontal:
			cout << "Horizontal: y=" << c->branchs[minInd]->c.value << endl;
			break;
		case Vertical:
			cout << "Vertical: x=" << c->branchs[minInd]->c.value << endl;
			break;
		case RightDown:
			cout << "RightDown: x-y=" << c->branchs[minInd]->c.value << endl;
			break;
		case LeftDown:
			cout << "LeftDown: x+y=" << c->branchs[minInd]->c.value << endl;
			break;
		}

		// 下一分支
		c = c->branchs[minInd];

	}

	system("pause");


}