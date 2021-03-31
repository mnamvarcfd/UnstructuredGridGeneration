#include "stdafx.h"
#include "bPOINT.h"
#include "EDGE.h"
#include <math.h>



bPOINT EDGE::mid() {
	bPOINT mP;
	mP.X[0] = (P[0]->X[0] + P[1]->X[0]) / 2;
	mP.X[1] = (P[0]->X[1] + P[1]->X[1]) / 2;
	mP.X[2] = (P[0]->X[2] + P[1]->X[2]) / 2;
	return mP;
}

double EDGE::length() {

	return sqrt((P[0]->X[0] - P[1]->X[0])*(P[0]->X[0] - P[1]->X[0]) + (P[0]->X[1] - P[1]->X[1])*(P[0]->X[1] - P[1]->X[1]) + (P[0]->X[2] - P[1]->X[2])*(P[0]->X[2] - P[1]->X[2]));
}