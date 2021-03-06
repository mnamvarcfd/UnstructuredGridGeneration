#include "stdafx.h"
#include "targetver.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <typeinfo>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <set>
#include <algorithm>
#include "mesh.h"


CELL* Cell[100];
EDGE* Edge[100];
POINT* Point[100];
MESH mesh;
vector<void*> D;

using namespace std;

// Main Scope 
int main() {
	mesh.readmesh("Mesh.txt");

	mesh.writeplt("Mesh_Output_delaunay.plt");
	int z = 1;
	ofstream file;
	file.open("Animation.plt");
	int CellNum = mesh.NC;
	int flaq = 0;
	

		
			
	for (int i = 0; i < CellNum; i++) {
		mesh.flip13(&mesh.CellStd[i]);

		if (file.is_open() == 1) {
			file << "TITLE=" << "'title'" << endl;
			file << "Variables=" << "'X'" << "," << "'Y'" << endl;
			file << "Zone" << " " << "T= mesh " << "  C=BLUE "<<"  N=" << " " << mesh.NP << ",E=" << " " << mesh.NF << ",ET=LINESEG" << ",F=FEBLOCK" << endl;
			for (int l = 0; l < mesh.NP; l++)
				file << mesh.PointStd[l].x << endl;
			for (int l = 0; l < mesh.NP; l++)
				file << mesh.PointStd[l].y << endl;
			for (int l = 0; l < mesh.NF; l++)
				file << mesh.A2N(mesh.EdgStd[l].P[0]) << " " << mesh.A2N(mesh.EdgStd[l].P[1]) << endl;



		}

	}
	int FaceNum = mesh.NF;
	for (int j = 0; j < 5; j++) {
		for (int k = 0; k < FaceNum; k++) {
			if (mesh.delcheck(&mesh.EdgStd[k]) == 0) {
				mesh.flip22(&mesh.EdgStd[k]);
				flaq++;
				if (flaq % 10 == 0) {
					if (file.is_open() == 1) {
						file << "TITLE=" << "'title'" << endl;
						file << "Variables=" << "'X'" << "," << "'Y'" << endl;
						file << "Zone" << " " << "T= mesh " << "  C=BLUE " << "  N=" << " " << mesh.NP << ",E=" << " " << mesh.NF << ",ET=LINESEG" << ",F=FEBLOCK" << endl;
						for (int l = 0; l < mesh.NP; l++)
							file << mesh.PointStd[l].x << endl;
						for (int l = 0; l < mesh.NP; l++)
							file << mesh.PointStd[l].y << endl;
						for (int l = 0; l < mesh.NF; l++)
							file << mesh.A2N(mesh.EdgStd[l].P[0]) << " " << mesh.A2N(mesh.EdgStd[l].P[1]) << endl;



					}
				}
			}
		}
	}


	mesh.writeplt("Mesh_Output_laplacian.plt");
	cout << clock();
	_getch();
	return 0;
}

