#pragma once
#include "bPOINT.h"
#include "POINT.h"


// part 1 :
class FACE {
public:
	POINT * P[4];
	bool IsOnBound = 0;
	bool IsOnInterface = 0;
	bool FaceIsLabl = 0;
	bool FaceIsDel = 0;
	int FacePointSize = 0;
	int FType = 3;
	int region = 0;

	// part 2 :
	FACE() {
		for (int i = 0; i < 4; i++) {
			P[i] = nullptr;
		}

	}

	// part 3 :
	FACE(POINT* P0, POINT* P1, POINT* P2) {
		P[0] = P0;
		P[1] = P1;
		P[2] = P2;
		P[3] = nullptr;
		FacePointSize = 3;
	}

	// part 4 :
	FACE(POINT* P0, POINT* P1, POINT* P2, POINT* P3) {
		P[0] = P0;
		P[1] = P1;
		P[2] = P2;
		P[3] = P3;
		FType = 4;
		FacePointSize = 4;
	}

	// part 5 :
	void FaceAddPoint(POINT* P_new) {
		P[FacePointSize++] = P_new;
		if (FacePointSize > 4)
			throw;
	}

	// part 6 :
	void FaceRemPoint() {
		for (int i = 0; i < 4; i++) {
			P[i] = nullptr;
		}
		FacePointSize = 0;
	}

	// part 7 :
	double perimeter();
	double area();
	bPOINT center();
	bool operator == (FACE);
	bool is_in(POINT*);
	int state();
	int is_joined(FACE*);
	int join(FACE*,POINT**);
	void push();
	void pool();

};