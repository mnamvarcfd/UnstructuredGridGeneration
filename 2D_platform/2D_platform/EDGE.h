#pragma once
#include "bPOINT.h"
#include "POINT.h"


// Definition Of EDGE Class
class EDGE {
public:
	POINT * P[2];
	int EdgType = 2;
	int region = 0;
	bool EdgIsLabl = 0;
	bool is_interface = 0;
	bool EdgIsDel = 0;


	EDGE() = default;
	double length();
	bPOINT mid();
	bool operator == (EDGE);
	int state();
	bool is_joined(EDGE*);
	
};

