#pragma once
#include <vector>
#include "surface.h"

// Definition of ZONE Class
class ZONE {
public:
	vector<POINT*> ZPoint;
	vector<EDGE*> ZEdge;
	vector<CELL*> ZCell;
	vector<EDGE*> CurveInterface;
	vector<EDGE*> CurveBound;
	void writeplt(const char*);
	void writecurve(const char*);
	enum TYPE { simple, hybrid };
	TYPE ZType = simple;
	
	
	void order();
};