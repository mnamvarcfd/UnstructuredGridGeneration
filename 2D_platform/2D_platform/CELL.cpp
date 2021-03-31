#include "stdafx.h"
#include "MESH.h"

// part 1 :
extern CELL* Cell[100];
extern EDGE* Edge[100];
extern POINT* Point[100];
extern MESH mesh;
extern vector<void*> D;


// Definition of area Method in CELL Class

// part 2 :
double CELL::area() {
	if (CellEdgSize == 3) {
		double a;
		double p = perimeter() / 2;
		double l0 = E[0]->length();
		double l1 = E[1]->length();
		double l2 = E[2]->length();
		a = p * (p - l0)*(p - l1)*(p - l2);
		if (isfinite(a) == 0)
			throw;
		return sqrt(a);
	}
	if (CellEdgSize == 4) {
		if (E[0]->P[0] != E[2]->P[0])
			swap(E[2]->P[0], E[2]->P[1]);
		if (E[0]->P[0] != E[2]->P[0]) {
			swap(E[2]->P[0], E[2]->P[1]);
			swap(E[0]->P[0], E[0]->P[1]);
		}
		if (E[0]->P[0] != E[2]->P[0])
			swap(E[2]->P[0], E[2]->P[1]);

		EDGE E_new;
		E_new.P[0] = E[0]->P[1];
		E_new.P[1] = E[2]->P[1];
		CELL C1{ E[0],E[2],&E_new };
		CELL C2{ E[1],E[3],&E_new };
		double a = C1.area() + C2.area();
		if (isfinite(a) == 0)
			throw;
		return a;
	}
}

// Definition of angle Method in CELL Class : return angle of specified POINT

// part 3 :
double CELL::cosin(POINT* P) {
	POINT* P1;
	POINT* P2;
	int n = 0; //counter of ni
	int edge_index[2] = { -1,-1 };
	int point_index[2] = { -1,-1 };
	double PP1[2];
	double PP2[2];
	for (int i = 0; i<CellEdgSize; i++)
		for (int j = 0; j<2; j++)
			if (E[i]->P[j] == P) {
				edge_index[n]= i;
				point_index[n++] = !j;
			}

	P1 = E[edge_index[0]]->P[point_index[0]];
	P2 = E[edge_index[1]]->P[point_index[1]];
	PP1[0] = P1->x - P->x;
	PP1[1] = P1->y - P->y;
	PP2[0] = P2->x - P->x;
	PP2[1] = P2->y - P->y;
	return (PP1[0] * PP2[0] + PP1[1] * PP2[1]) / (sqrt(PP1[0] * PP1[0] + PP1[1] * PP1[1])*sqrt(PP2[0] * PP2[0] + PP2[1] * PP2[1]));
}


// Definition of perimeter Method in CELL Class 

// part 4 :
double CELL::perimeter() {
	double a = 0;
	for (int i = 0; i < CellEdgSize; i++) {
		a = a + E[i]->length();
	}
	return a;
}

// Definition of across Method of Hybrid CELL : return across edge number of specified edge number of hybrid cell

// part 5 :
int CELL::across(int i) {
	if (this->CellEdgSize == 4) {
		if (i == 0)
			return 1;
		if (i == 1)
			return 0;
		if (i == 2)
			return 3;
		if (i == 3)
			return 2;
	}
	else
		return -1;
}

// Definition of across Method of Hybrid CELL : return across edge number of specified edge 

// part 6 :
int CELL::across(EDGE* Ep) {
	int i;
	if (this->CellEdgSize == 4) {
		for (i = 0; i < 4; i++)
			if (this->E[i] == Ep)
				break;
		if (i == 0)
			return 1;
		if (i == 1)
			return 0;
		if (i == 2)
			return 3;
		if (i == 3)
			return 2;
	}
	else
		return -1;
}

// Definition of is_in Method in CELL Class : return true if specified edge is in the specified cell , otherwise return false

// part 7 :
bool CELL::is_in(EDGE* Ep) {
	for (int i = 0; i < CellEdgSize; i++)
		if (Ep == E[i])
			return true;
	return false;
}

// part 8 :
bPOINT CELL::center() {
	bPOINT P;
	int n = mesh.find(this, MESH::point);
	for (int i = 0; i < n; i++) {
		P.x = Point[i]->x + P.x;
		P.y = Point[i]->y + P.y;
	}
	P.x = P.x / n;
	P.y = P.y / n;
	return P;
}

