#pragma once
#include "SURFACE.h"
#include "EDGE.h"

class ZONE {
public:
	vector<POINT*> ZPoint;
	vector<FACE*> ZFace;
	vector<CELL*> ZCells;
	vector<FACE*> interface;
	vector<FACE*> boundary;
	enum TYPE { simple, hybrid };
	TYPE ZType = simple;
	bool extend(CELL*, FACE*, vector<CELL*>&);
	CELL* C_stack[1000000];
	FACE* F_stack[1000000];
	
};