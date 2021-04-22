#include <iostream>
#include <vector>
#include <set>
#include <stack>
using namespace std;

typedef struct{
	int x;   // 0~39
	int y;		// 0~49
	bool exist;  // ��ˮ���Ƿ�����ڵ�ǰͼ��
} Position;  // ˮ��λ��

typedef enum {
	Horizontal,
	Vertical,
	RightDown,  // ���ϵ�����б
	LeftDown	// ���ϵ�����б
} CurDirection;   // ��������

typedef struct{
	int value;  // �Բ�ͬ���������ֶ��в�ͬ���ͣ���CutCrossPoint����
	CurDirection dir;
	bool exist;  // �õ����Ƿ�����ڵ�ǰ����������
} Cut;  // һ������


typedef struct _CutTree{
	Cut c;
	int nCut;  // �����������ȥ�������ܵ���
	struct _CutTree* branchs[4];
}CutTree;   // �������ɵ���

CutTree cTree;

// ����Position��CurDirection����Cut
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

// �жϵ���c�Ƿ����p
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

// �жϹ�p1�ĵ���dir�Ƿ�Ҳ����p2
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


// ���ڵ�����set
class myless_than
{
public:
	bool operator()(const Cut& c1, const Cut& c2) const//Ҳ����const��
	{
		if (c1.dir == c2.dir)
			return c1.value < c2.value;
		else
			return c1.dir < c2.dir;
	}
};

// ����һ���������е�������
int solu(vector<Position> &positions, vector<Cut> &cuts)
{
	vector<bool> hasBeenCut(positions.size(), 0);  // ����ÿ�ֵ�����ʱ�򣬸���ˮ���Ƿ���
	int minCuts = positions.size();   // ��С����

	// �Ƿ�����ˮ���������� 
	bool finish = true;  
	for (int i = 0; i < positions.size(); i++)
		if (positions[i].exist)
		{
			finish = false;
			break;
		}
	if (finish)
		return 0;

	// ����ÿ������
	for (int i = 0; i < cuts.size(); i++)
	{
		if (!cuts[i].exist)   // ������ǰ��������
			continue; 

		//for (int i = 0; i < hasBeenCut.size(); i++)
		//	hasBeenCut[i] = 0;
		// ȥ���õ�����������ˮ��
		for (int j = 0; j < positions.size(); j++)
		{
			if (positions[j].exist && CutCrossPoint(positions[j], cuts[i]))
			{
				positions[j].exist = false;
				hasBeenCut[j] = 1;
			}
		}
		// ȥ���õ���
		cuts[i].exist = 0;  

		// �ݹ�
		int nSub = solu(positions, cuts);
		// ������С����
		if (nSub + 1 < minCuts)
			minCuts = nSub + 1;
		
		// �ѵ�����ˮ����ԭ
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

// �������������׸�λ�õĵ�������
int solu2(vector<Position> positions, CutTree* curRoot)
{
	int minCuts = positions.size();   // ��С����

	curRoot->branchs[0] = NULL;				// ·����ֹ�б�����
	if (positions.empty())
		return 0;

	
	
	// ������һ���ڵ��4������
	Position p = positions[0];
	for (int i = 0; i < 4; i++)
	{
		CurDirection cd = (CurDirection)i;
		vector<Position> tmp_positions = positions;

		// ȥ���õ�����������ˮ��
		for (int j = 0; j < tmp_positions.size(); j++)
		{
			if (CutCrossPoint(p, tmp_positions[j], cd))
			{
				tmp_positions.erase(tmp_positions.begin() + j);
				j--; 
			}
		}

		// ����һ����֧
		CutTree *ct = (CutTree*)malloc(sizeof(CutTree));
		ct->c = cutFromPosDir(p, cd);
		curRoot->branchs[i] = ct;

		// �ݹ�
		int nSub = solu2(tmp_positions, ct);
		ct->nCut = nSub + 1;

		// ������С����
		if (nSub + 1 < minCuts)
			minCuts = nSub + 1;


	}
	return minCuts;
}

int main()
{
	// ����
	int N;
	cin >> N;
	vector<Position> positions;
	for (int i = 0; i < N; i++)
	{
		int x, y;
		cin >> x >> y;
		positions.push_back({ x, y, true });
	}
	
	/*********** ����1 ***********/
	// Ѱ�����е�����ÿ��ˮ�������ֵ������ϣ�
	//set<Cut, myless_than> cutSet;
	//for (int i = 0; i < positions.size(); i++)
	//{
	//	Cut c;
	//	c.exist = true;
	//	// ���ֵ���
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
	//vector<Cut> cutVec;   // ��vector����Ȼ���治�ò���
	//set<Cut>::iterator it;
	//for (it = cutSet.begin(); it != cutSet.end(); it++)
	//	cutVec.push_back(*it);
	//// ���
	//cout << solu(positions, cutVec) << endl;

	/*********** ����2 ***********/
	cout << solu2(positions, &cTree) << endl;
	// ��ӡ�������
	CutTree*c = &cTree;
	while (c->branchs[0])
	{
		// ����С�����ĵ���
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
		// ��ӡ
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

		// ��һ��֧
		c = c->branchs[minInd];

	}

	system("pause");


}