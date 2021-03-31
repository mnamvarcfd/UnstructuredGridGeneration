#include "stdafx.h"
#include "FACE.h"
#include "MESH.h"

extern POINT* Point[100];
extern EDGE* Edge[100];
extern FACE* Face[100];
extern CELL* Cell[100];
extern MESH mesh;


// part 1 :
bPOINT FACE::center() {
	bPOINT p = { 0,0,0 };
	for (int i = 0; i < FacePointSize; i++) {
		p.X[0] = P[i]->X[0] + p.X[0];
		p.X[1] = P[i]->X[1] + p.X[1];
		p.X[2] = P[i]->X[2] + p.X[2];
	}
	p.X[0] = p.X[0] / FacePointSize;
	p.X[1] = p.X[1] / FacePointSize;
	p.X[2] = p.X[2] / FacePointSize;
	return p;
}

// part 2 :
bool FACE::is_in(POINT* P) {
	for (int i = 0; i<this->FacePointSize; i++)
		if (P == this->P[i])
			return true;

	return false;
}

// part 3 :
int FACE::is_joined(FACE* face) {
	int n = 0;
	for (int i = 0; i < FacePointSize; i++)
		for (int j = 0; j < face->FacePointSize; j++)
			if (P[i] == face->P[j])
				n++;
	return n;
}

// part 4 :
int FACE::state() {
	int n=mesh.find(this, MESH::cell);
	if (n == 1)
		return 0;
	else if (n == 2)
		if ((Cell[0]->CType == 0 && Cell[1]->CType == 0) || (Cell[0]->CType != 0 && Cell[1]->CType != 0))
			return 1;
		else
			return 2;
	else
		throw;
	/*int m = 0;
	CELL* Ce[2];
	unsigned long long I[2];

	{



		FACE* f[6];
		int F_size;
		for (int i = 0; i < this->P[0]->PointCellSize; i++) {
			//f = (this->P[0]->cell_neib[i])->F;
			memcpy(f, this->P[0]->PointCells[i], 6 * sizeof(FACE*));
			for (int j = 0; j<6; j++)
				if (*(f + j) == this) {
					I[m] = i;
					Ce[m++] = this->P[0]->PointCells[i];
					break;
				}
			if (m == 2)
				break;
		}
		//if (m > 2 || m == 0)
		//	throw;
	}
	
	if (m == 1)
		return 0;
	else if (m == 2) {
		int type[2];
		I[0] = reinterpret_cast<unsigned long long>(Ce[0]);
		I[1] = reinterpret_cast<unsigned long long>(Ce[1]);
		I[0] = I[0] + 6 * sizeof(FACE*);
		I[1] = I[1] + 6 * sizeof(FACE*);

		memcpy(type, reinterpret_cast<void*>(I[0]), sizeof(int));
		memcpy(type+1, reinterpret_cast<void*>(I[1]), sizeof(int));
		
		if ((type[0] ==0 && type[1]==0) || (type[0] != 0 && type[1] != 0))
			return 1;
		else
			return 2;
	}
	else
		throw;*/
}

// part 5 :
double FACE::area() {
	if (FacePointSize == 3) {
		double a[3];
		double b[3];
		double ab[3];
		for (int i = 0; i < 3; i++) {
			a[i] = P[1]->X[i] - P[0]->X[i];
			b[i] = P[2]->X[i] - P[0]->X[i];
		}
		ab[0] = (a[1] * b[2] - a[2] * b[1]);
		ab[1] = -(a[0] * b[2] - a[2] * b[0]);
		ab[2] = (a[0] * b[1] - a[1] * b[0]);
		return .5*sqrt(ab[0] * ab[0] + ab[1] * ab[1] + ab[2] * ab[2]);
	}
}

// part 6 :
int FACE::join(FACE* Fp,POINT** Pp) {
	int n = 0;
	

	for (int i = 0; i < FacePointSize; i++)
		for (int j = 0; j < Fp->FacePointSize; j++)
			if (P[i] == Fp->P[j])
				Pp[n++]=P[i];
	return n;
	
	
}

// part 7 :
void FACE::push() {
	if (FacePointSize == 3) {
		POINT* Pn[3] = { P[2],P[0],P[1] };
		memcpy(this->P, Pn,3*sizeof(POINT*));
	}
	if (FacePointSize == 4) {
		POINT* Pn[4] = { P[3],P[0],P[1],P[2] };
		memcpy(this->P, Pn, 4 * sizeof(POINT*));
	}
}

// part 8 :
void FACE::pool() {
	if (FacePointSize == 3) {
		POINT* Pn[3] = { P[1],P[2],P[0] };
		memcpy(this->P, Pn, 3 * sizeof(POINT*));
	}
	if (FacePointSize == 4) {
		POINT* Pn[4] = { P[1],P[2],P[3],P[0] };
		memcpy(this->P, Pn, 4 * sizeof(POINT*));
	}
}