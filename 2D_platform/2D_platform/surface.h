#pragma once
#include "CELL.h"
#include "EDGE.h"
#include <vector>




// Definition of Surface Class 
class SURFACE {
public:
	vector<EDGE*> E;
	vector<CELL*> C[2];
	void order();
	void flip();
	void clear();
};