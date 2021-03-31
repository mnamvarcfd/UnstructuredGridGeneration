#pragma once
#include <algorithm>
#include "macros.h"


using namespace std;


// Forward Declaration of EDGE & CELL Classes
// part 1 :
class EDGE;
class CELL;



// Definition of POINT Class
// part 2 :
class POINT {
public:

	double x;
	double y;
	EDGE* PointEdgs[MAX_NEIB];
	CELL* PointCells[MAX_NEIB];
	int PointEdgSize = 0;
	int PointCellSize = 0;
	bool is_boundary = 0;
	bool is_on_interface = 0;
	bool PointIsDel = 0;

	// part 3 :
	POINT() {
		for (int i = 0; i < MAX_NEIB; i++) {
			PointEdgs[i] = nullptr;
			PointCells[i] = nullptr;
		}
	}
	
	// part 4 :
	void PointAddEdg(EDGE* new_E) {
		PointEdgs[PointEdgSize++] = new_E;
		if (PointEdgSize == MAX_NEIB)
			throw;
	}

	// part 5 :
	void PointAddCell(CELL* new_C) {
		PointCells[PointCellSize++] = new_C;
		if (PointCellSize == MAX_NEIB)
			throw;
	}

	// part 6 :
	void PointRemEdg(int i) {
		swap(PointEdgs[i], PointEdgs[PointEdgSize - 1]);
		PointEdgs[PointEdgSize - 1] = nullptr;
		PointEdgSize--;
	}

	// part 7 :
	void PointRemCell(int i) {
		swap(PointCells[i], PointCells[PointCellSize - 1]);
		PointCells[PointCellSize - 1] = nullptr;
		PointCellSize--;
	}
	
};

