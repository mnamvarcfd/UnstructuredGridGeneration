#pragma once
#include "EDGE.h"


// Definition of CELL Class
class CELL {
public:
	EDGE * E[4];
	int CellEdgSize = 0;
	int CellZnum = 0;
	bool CellIsDel = 0;

	CELL() {
		for (int i = 0; i < 4; i++)
			E[i] = nullptr;
	}
	CELL(EDGE* E1, EDGE* E2, EDGE* E3) {
		E[0] = E1;
		E[1] = E2;
		E[2] = E3;
		CellEdgSize = 3;
	}
	CELL(EDGE* E1, EDGE* E2, EDGE* E3, EDGE* E4) {
		E[0] = E1;
		E[1] = E2;
		E[2] = E3;
		E[3] = E4;
		CellEdgSize = 4;
	}
	
	double area();
	double cosin(POINT*);
	bPOINT center();
	double perimeter();
	void CellAddEdg(EDGE* new_E) {
		E[CellEdgSize++] = new_E;

	}
	int across(int);
	int across(EDGE*);
	bool is_in(EDGE*);
	
};

