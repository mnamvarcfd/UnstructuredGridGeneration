#include "stdafx.h"
#include "CELL.h"
#include "bPOINT.h"

// part 1 :
double CELL::volume() {

	POINT* P[8];
	FACE* f;
	
	{

		if (this->CType == CELL::tetra) {
			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2]) {
				P[3] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2]) {
				P[3] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2]) {
				P[3] = f->P[2];
			}


			double A[3] = { P[0]->X[0],P[0]->X[1],P[0]->X[2] };
			double B[3] = { P[1]->X[0],P[1]->X[1],P[1]->X[2] };
			double C[3] = { P[2]->X[0],P[2]->X[1],P[2]->X[2] };
			double D[3] = { P[3]->X[0],P[3]->X[1],P[3]->X[2] };


			double a[3] = { A[0] - D[0],A[1] - D[1],A[2] - D[2] };
			double b[3] = { B[0] - D[0],B[1] - D[1],B[2] - D[2] };
			double c[3] = { C[0] - D[0],C[1] - D[1],C[2] - D[2] };


			double b_c[3] = { b[1] * c[2] - b[2] * c[1],-(b[0] * c[2] - b[2] * c[0]),b[0] * c[1] - b[1] * c[0] };


			double ab_c = a[0] * b_c[0] + a[1] * b_c[1] + a[2] * b_c[2];


			return abs(ab_c) / 6;
		}
		else
			return -1;
	}
}

// part 2 :
bPOINT CELL::center() {
	POINT* P[8];
	int P_counter = 0;
	FACE* f;


	{
		if (this->CType == CELL::tetra) {

			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2]) {
				P[3] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2]) {
				P[3] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2]) {
				P[3] = f->P[2];
			}
			P_counter = 4;
		}
		if (this->CType == CELL::cube) {
			f = this->F[0];
			memcpy(P, f->P, 4 * sizeof(POINT*));
			f = this->F[1];
			memcpy(&P[4], f->P, 4 * sizeof(POINT*));
			P_counter = 8;
		}
		if (this->CType == CELL::prism) {
			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			memcpy(&P[3], f->P, 3 * sizeof(POINT*));
			P_counter = 6;
		}
		if (this->CType == CELL::pyramid) {
			f = this->F[0];
			memcpy(P, f->P, 4 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2] && f->P[0] != P[3]) {
				P[4] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2] && f->P[1] != P[3]) {
				P[4] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2] && f->P[2] != P[3]) {
				P[4] = f->P[2];
			}
			P_counter = 5;
		}

	}

	bPOINT p;
	p.X[0] = 0;
	p.X[1] = 0;
	p.X[2] = 0;
	for (int i = 0; i < P_counter; i++) {
		p.X[0] = P[i]->X[0] + p.X[0];
		p.X[1] = P[i]->X[1] + p.X[1];
		p.X[2] = P[i]->X[2] + p.X[2];
	}
	p.X[0] = p.X[0] / P_counter;
	p.X[1] = p.X[1] / P_counter;
	p.X[2] = p.X[2] / P_counter;
	return p;
}

// part 3 :
void CELL::RemoveCellFromPoints() {
	POINT* P[8];
	int P_counter = 0;
	FACE* f;


	{
		if (this->CType == CELL::tetra) {

			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2]) {
				P[3] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2]) {
				P[3] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2]) {
				P[3] = f->P[2];
			}
			P_counter = 4;
		}
		if (this->CType == CELL::cube) {
			f = this->F[0];
			memcpy(P, f->P, 4 * sizeof(POINT*));
			f = this->F[1];
			memcpy(&P[4], f->P, 4 * sizeof(POINT*));
			P_counter = 8;
		}
		if (this->CType == CELL::prism) {
			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			memcpy(&P[3], f->P, 3 * sizeof(POINT*));
			P_counter = 6;
		}
		if (this->CType == CELL::pyramid) {
			f = this->F[0];
			memcpy(P, f->P, 4 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2] && f->P[0] != P[3]) {
				P[4] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2] && f->P[1] != P[3]) {
				P[4] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2] && f->P[2] != P[3]) {
				P[4] = f->P[2];
			}
			P_counter = 5;
		}

	}
	for (int i = 0; i < P_counter; i++) {
		if (P[i]->PointRemCell(this) != 1)
			throw;
	}
}

// part 4 :
void CELL::AddCellToPoints() {
	POINT* P[8];
	int P_counter = 0;
	FACE* f;


	{
		if (this->CType == CELL::tetra) {

			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2]) {
				P[3] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2]) {
				P[3] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2]) {
				P[3] = f->P[2];
			}
			P_counter = 4;
		}
		if (this->CType == CELL::cube) {
			f = this->F[0];
			memcpy(P, f->P, 4 * sizeof(POINT*));
			f = this->F[1];
			memcpy(&P[4], f->P, 4 * sizeof(POINT*));
			P_counter = 8;
		}
		if (this->CType == CELL::prism) {
			f = this->F[0];
			memcpy(P, f->P, 3 * sizeof(POINT*));
			f = this->F[1];
			memcpy(&P[3], f->P, 3 * sizeof(POINT*));
			P_counter = 6;
		}
		if (this->CType == CELL::pyramid) {
			f = this->F[0];
			memcpy(P, f->P, 4 * sizeof(POINT*));
			f = this->F[1];
			if (f->P[0] != P[0] && f->P[0] != P[1] && f->P[0] != P[2] && f->P[0] != P[3]) {
				P[4] = f->P[0];
			}
			if (f->P[1] != P[0] && f->P[1] != P[1] && f->P[1] != P[2] && f->P[1] != P[3]) {
				P[4] = f->P[1];
			}
			if (f->P[2] != P[0] && f->P[2] != P[1] && f->P[2] != P[2] && f->P[2] != P[3]) {
				P[4] = f->P[2];
			}
			P_counter = 5;
		}

	}
	for (int i = 0; i < P_counter; i++)
		P[i]->PointAddCell(this);
}

// part 5 :
FACE* CELL::across_F(FACE* Face) {
	if (CType == cube) {
		for(int i=0;i<CellFaceSize;i++)
			if (F[i] == Face) {
				if (i == 0)
					return F[1];
				if (i == 1)
					return F[0];
				if (i == 2)
					return F[3];
				if (i == 3)
					return F[2];
				if (i == 4)
					return F[5];
				if (i == 5)
					return F[4];
			}
	}
	if (CType == prism) {
		if (F[0] == Face)
			return F[1];
		if (F[1] == Face)
			return F[0];
		if (Face != F[0] && Face != F[1])
			return nullptr;
	}

}

// part 6 :
bool CELL::is_in(FACE* Fadd) {
	for (int i = 0; i < CellFaceSize; i++)
		if (F[i] == Fadd)
			return true;
	return false;
}