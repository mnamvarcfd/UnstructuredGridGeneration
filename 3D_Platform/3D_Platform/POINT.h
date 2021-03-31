#pragma once
#include <algorithm>
#include <vector>
#include "macros.h"



using namespace std;

// part 1:
class CELL;

// part 2 :
class POINT {
public:
	CELL * PointCells[MAX_NEIB];
	int PointCellSize = 0;
	double X[3];
	bool IsOnBound = 0;
	bool IsOnInterface = 0;
	bool PointIsDel = 0;
	vector<int> reg_vec;

	// part 3:
	POINT() {
		for (int i = 0; i < MAX_NEIB; i++)
			PointCells[i] = nullptr;
	}

	// part 4:
	void PointAddCell(CELL* C_new) {
		/*for (int i = 0; i < PointCellSize; i++)
			if (C_new == PointCells[i])
				throw;*/
		PointCells[PointCellSize++] = C_new;
		if (PointCellSize == MAX_NEIB)
			throw;
	}

	// part 5 :
	bool PointRemCell(CELL* C_new) {
		for (int i = 0; i<PointCellSize; i++)
			if (PointCells[i] == C_new) {
				swap(PointCells[i], PointCells[PointCellSize - 1]);
				PointCells[PointCellSize - 1] = nullptr;
				PointCellSize--;
				return true;
			}
		return false;
	}
	bPOINT tobPOINT() {
		bPOINT bP;
		memcpy(bP.X, X, 3 * sizeof(double));
		return bP;
	}
};