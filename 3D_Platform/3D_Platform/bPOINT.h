#pragma once
#include <iostream>

class bPOINT {
public:
	double X[3];
	bool operator==(bPOINT bP) {
		if (X[0] == bP.X[0] && X[1] == bP.X[1] && X[2] == bP.X[2])
			return true;
		else
			return false;
	}
	void show() {
		std::cout << std::endl << X[0] << " " << X[1] << " " << X[2] << std::endl;
	}
};