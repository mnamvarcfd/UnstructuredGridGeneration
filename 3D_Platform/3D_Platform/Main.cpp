#include "stdafx.h"
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
#include "globals.h"
#include "MESH.h"







using namespace std;










// Global Variables and functions




int main() {

	mesh.readmesh("meshout.gid");
	mesh.Cell_order();
	CELL* C = nullptr;
	for (int i = 0; i < mesh.NC; i++)
		if (mesh.CellStd[i].CType == CELL::prism)
			C = &mesh.CellStd[i];
	vector<CELL*> layer;
	mesh.extend(C, layer);
	mesh.Cell_order(layer);
	mesh.add(layer);
	mesh.add(layer);

	mesh.writeplt("out.plt");
	_getch();
	return 0;


}






