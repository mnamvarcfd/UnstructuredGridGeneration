#pragma once
#include "CELL.h"

class SURFACE {
public:
	vector<FACE*> F;
	vector<CELL*> C[2];
	void order();
};

