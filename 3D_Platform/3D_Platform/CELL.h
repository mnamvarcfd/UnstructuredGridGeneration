#pragma once
#include "FACE.h"

// part 1 :
class CELL {
public:
	FACE * F[6];
	enum TYPE { tetra, pyramid, prism, cube };
	TYPE CType = tetra;
	bool CellIsDel = 0;
	bool CellIsLabl = 0;
	int CellFaceSize = 0;
	int CellZNum = 0;
	
	// part 2:
	CELL() {
		for (int i = 0; i < 6; i++)
			F[i] = nullptr;
	}

	// part 3 :
	void CellAddFace(FACE* F_new) {
		F[CellFaceSize++] = F_new;
		if (CellFaceSize > 6) 
			throw;
		
	}

	// part 4:
	void CellRemFace() {
		for (int i = 0; i < 6; i++)
			F[i] = nullptr;
		CellFaceSize = 0;
	}

	// part 5 :
	void RemoveCellFromPoints();
	void AddCellToPoints();
	void check_p();
	double volume();
	double area();
	bPOINT center();
	bool is_in(FACE*);
	FACE* across_F(FACE*);
	POINT* across_P(FACE*);

};