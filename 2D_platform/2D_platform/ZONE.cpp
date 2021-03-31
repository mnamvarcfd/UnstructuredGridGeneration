#include "stdafx.h"
#include"ZONE.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include "MESH.h"

// part 1 :
extern CELL* Cell[100];
extern EDGE* Edge[100];
extern POINT* Point[100];
extern MESH mesh;
extern vector<void*> D;

using namespace std;




// part 2 :
// Definition of writein Methode in ZONE Class : this Method write a .txt file of edge array data of specified zone for MATLAB Program 
void ZONE::writeplt(const char* file_name) {
	ofstream file;
	file.open(file_name);
	if (file.is_open() == 1) {
		file << "TITLE=" << "'title'" << endl;
		file << "Variables=" << "'X'" << "," << "'Y'" << endl;
		file << "Zone" << " " << "T='title'," << "N=" << " " << ZPoint.size() << ",E=" << " " << ZEdge.size() << ",ET=LINESEG" << ",F=FEBLOCK" << endl;
		for (int i = 0; i < ZPoint.size(); i++)
			file << ZPoint[i]->x << endl;
		for (int i = 0; i < ZPoint.size(); i++)
			file << ZPoint[i]->y << endl;
		for (int i = 0; i < ZEdge.size(); i++) {
			size_t s1 = find(ZPoint.data(), ZPoint.data() + ZPoint.size(), ZEdge[i]->P[0]) - ZPoint.data();
			size_t s2 = find(ZPoint.data(), ZPoint.data() + ZPoint.size(), ZEdge[i]->P[1]) - ZPoint.data();
			file << s1+1 << " " << s2+1 << endl;
		}
	}
}


// Definition of writecureve Methode in ZONE Class : this Method write a .txt file of edge array data of specified curve in the zone for MATLAB Program 
void ZONE::writecurve(const char* file_name) {
	ofstream file;
	file.open(file_name);
	if (file.is_open() == 1) {
		file << CurveBound.size() + CurveInterface.size() << endl;
		for (int i = 0; i < ZEdge.size(); i++)
			if (ZEdge[i]->state() != 1)
				file << ZEdge[i]->P[0]->x << "\t" << ZEdge[i]->P[0]->y << "\t" << ZEdge[i]->P[1]->x << "\t" << ZEdge[i]->P[1]->y << endl;

	}
}

// part 3 :
//Definition of order Methode in ZONE Class : 
void ZONE::order() {
	if (ZType == hybrid) {
		for (int i = 0; i < ZCell.size(); i++) {
			if (ZCell[i]->E[0]->is_joined(ZCell[i]->E[1]))
				swap(ZCell[i]->E[1], ZCell[i]->E[2]);
			if (ZCell[i]->E[0]->is_joined(ZCell[i]->E[1]))
				swap(ZCell[i]->E[1], ZCell[i]->E[3]);
		}
	}
}

