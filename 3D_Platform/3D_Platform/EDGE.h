#pragma once
#include "bPOINT.h"
#include "POINT.h"


// part 1:
class EDGE {
public:
	POINT * P[2];
	double length();
	bPOINT mid();
	bool operator==(EDGE);
	bool is_joined(EDGE*);

	// part 2 :
	EDGE(POINT* P1, POINT* P2) {
		P[0] = P1;
		P[1] = P2;
	}

	// part 3:
	EDGE() {
		P[0] = nullptr;
		P[1] = nullptr;
	}

	// part 4 :
	bool IsOnBound() {
		int t = 0;
		for (int i = 0; i < P[0]->reg_vec.size(); i++)
			for (int j = 0; j < P[1]->reg_vec.size(); j++)
				if (P[0]->reg_vec[i] == P[1]->reg_vec[j])
					t = 1;

		if (t == 1)
			return true;
		else
			return false;
	}
};