#include "stdafx.h"
#include "MESH.h"

// part1 :
extern CELL* Cell[100];
extern EDGE* Edge[100];
extern POINT* Point[100];
extern MESH mesh;
extern vector<void*> D;


// Length of EDGE
// part2 :
double EDGE::length() {
	return sqrt((P[0]->x - P[1]->x)*(P[0]->x - P[1]->x) + (P[0]->y - P[1]->y)*(P[0]->y - P[1]->y));
}

// Produce bPOINT as a Middle Specified EDGE
// part 3 :
bPOINT EDGE::mid() {
	bPOINT P;
	P.x = (this->P[0]->x + this->P[1]->x) / 2;
	P.y = (this->P[0]->y + this->P[1]->y) / 2;
	return P;
}

// Definition of Operator (==) in EDGE Class
// part 4 :
bool EDGE::operator == (EDGE Edge) {
	if ((Edge.P[0] == this->P[0] && Edge.P[1] == this->P[1]) || (Edge.P[0] == this->P[1] && Edge.P[1] == this->P[0]))
		return true;
	else
		return false;
}


// Definiton of is_joined Method in EDGE Class
// part 5 :
bool EDGE::is_joined(EDGE* Edge) {
	if (Edge->P[0] == this->P[0] || Edge->P[0] == this->P[1] || Edge->P[1] == this->P[0] || Edge->P[1] == this->P[1])
		return true;
	else
		return false;
	
}


// part 6 :
int EDGE::state() {
	int n = mesh.find(this, MESH::cell);
	if (n == 1)
		return 0;
	else if (Cell[0]->CellEdgSize == Cell[1]->CellEdgSize)
		return 1;
	else
		return 2;

}



