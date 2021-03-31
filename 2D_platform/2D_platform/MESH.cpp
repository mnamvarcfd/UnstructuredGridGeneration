#include "stdafx.h"
#include "MESH.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <set>





//Definition of readmesh Method in MESH Class 
void MESH::readmesh(const char* file_name) {
	int p1;
	int p2;
	int me;
	int ne;
	int Reg_data[30];
	cout << file_name << endl;
	ifstream file;
	file.open(file_name);
	if (file.is_open() == 1) {
		file >> mesh_dim >> NP >> NC >> NF >> NR;
		cout << "mesh dimension :" << mesh_dim << "\nnumber of points :" << NP << "\nnumber of cells :" << NC << "\nnumber of faces :" << NF << "\nnumber of regions :" << NR << endl;
	}
	PointStd.reserve(NP * SIZE);
	CellStd.reserve(NC * SIZE);
	EdgStd.reserve(NF * SIZE);


	PointStd.resize(NP);
	CellStd.resize(NC);
	EdgStd.resize(NF);

	for (int i = 0; i < 2 * NR; i++)
		file >> Reg_data[i];
	int edge_counter = 0;
	for (int j = 0; j < NR; j++) {
		for (int i = 0; i < Reg_data[2 * j]; i++) {
			file >> EdgStd[edge_counter].EdgType >> me >> ne >> p1 >> p2;
			CellStd[me - 1].CellAddEdg(&EdgStd[edge_counter]);
			EdgStd[edge_counter].region = Reg_data[2 * j + 1];
			EdgStd[edge_counter].P[0] = &PointStd[p1 - 1];
			EdgStd[edge_counter].P[1] = &PointStd[p2 - 1];
			if (ne != 0) {
				CellStd[ne - 1].CellAddEdg(&EdgStd[edge_counter]);
			}
			else {
				PointStd[p1 - 1].is_boundary = 1;
				PointStd[p2 - 1].is_boundary = 1;
			}
			edge_counter++;
		}
	}
	double q;
	for (int i = 0; i < NP; i++) {
		file >> PointStd[i].x >> PointStd[i].y;
	}


	for (int i = 0; i < NF; i++) {
		EdgStd[i].P[0]->PointAddEdg(&EdgStd[i]);
		EdgStd[i].P[1]->PointAddEdg(&EdgStd[i]);
	}
	int n;
	for (int i = 0; i < NC; i++) {
		n = find(&CellStd[i], point);
		for (int j = 0; j < n; j++) {
			Point[j]->PointAddCell(&CellStd[i]);
		}
	}
	for(int i=0;i<NC;i++)
		if (CellStd[i].CellEdgSize == 4) {
			if (CellStd[i].E[0]->is_joined(CellStd[i].E[2]) == 0)
				swap(CellStd[i].E[1], CellStd[i].E[2]);
			else if (CellStd[i].E[0]->is_joined(CellStd[i].E[3]) == 0)
				swap(CellStd[i].E[1], CellStd[i].E[3]);
		}
	
}


//

void MESH::readmeshGID(const char* filename)
{
	int pNum, k = 0, L = 0, i = 0, zoneNumber = 1, me, ne, p1, p2;
	int Reg_data[30];
	//for reading line by line in file
	string line;
	//for five line at first and convert to int 
	string a[5];
	//for iteration
	vector<int> countZone;
	//define and open file object for reading 
	ifstream myfile(filename);
	//if file open 
	if (!myfile) {
		cout << "Unable to open file";
		exit(1); // terminate with error
	}
	//read line by line file
	getline(myfile, line);
	while (line != " Number of Faces of each Region , BC , Region Name")
	{
		std::stringstream s(line);
		s >> a[i];
		i++;
		getline(myfile, line);
	}
	//Extract five line for refrence data and convert to int
	mesh_dim = stoi(a[0]);
	NP = stoi(a[1]);
	NC = stoi(a[2]);
	NF = stoi(a[3]);
	NR = stoi(a[4]);
	//resize object for number of point
	PointStd.reserve(NP * SIZE);
	CellStd.reserve(NC * SIZE);
	EdgStd.reserve(NF * SIZE);


	PointStd.resize(NP);
	CellStd.resize(NC);
	EdgStd.resize(NF);

	for (int i = 0; i < 2 * NR; i++)
		myfile >> Reg_data[i];
	getline(myfile, line);
	while (line != " Number of Points of each Face , Left Cell  , Right Cell  ,  Points")
	{
		stringstream s(line);
		s >> a[0] >> a[1];
		countZone.push_back(stoi(a[0]));
		countZone.push_back(stoi(a[1]));
		getline(myfile, line);
	}

	//Extroud edge p1 , p2 point of them
	getline(myfile, line);
	i = 0;
	while (line != " Coordinates:     X              ,               Y               ,          Z  ")
	{
		while (L != countZone[k])
		{
			stringstream s(line);
			s >> EdgStd[L].EdgType >> me >> ne >> p1 >> p2;
			CellStd[me - 1].CellAddEdg(&EdgStd[L]);
			EdgStd[L].region = Reg_data[2 * j + 1];
			EdgStd[L].P[0] = &PointStd[p1 - 1];
			EdgStd[L].P[1] = &PointStd[p2 - 1];
			if (ne != 0) {
				CellStd[ne - 1].CellAddEdg(&EdgStd[L]);
			}
			else {
				PointStd[p1 - 1].is_boundary = 1;
				PointStd[p2 - 1].is_boundary = 1;
			}
			i++;
			L++;
			getline(myfile, line);
		}
		zoneNumber++;
		L = 0;
		k += 2;

	}

	//Extroud Coordinate Data and set it to string
	getline(myfile, line);

	i = 0;
	while (line != "   Cell Type  ")
	{
		stringstream s(line);

		s >> PointStd[i].x >> PointStd[i].y >> PointStd[i].z;
		i++;
		getline(myfile, line);
	}

	for (int v = 0; v < NF; v++) {
		EdgStd[v].P[0]->PointAddEdg(&EdgStd[v]);
		EdgStd[v].P[1]->PointAddEdg(&EdgStd[v]);
	}

	int n;
	for (int v = 0; v < NC; v++) {
		n = find(&CellStd[v], point);
		for (int j = 0; j < n; j++) {
			Point[j]->PointAddCell(&CellStd[v]);
		}
	}
	for (int v = 0; v<NC; v++)
		if (CellStd[v].CellEdgSize == 4) {
			if (CellStd[v].E[0]->is_joined(CellStd[v].E[2]) == 0)
				swap(CellStd[v].E[1], CellStd[v].E[2]);
			else if (CellStd[v].E[0]->is_joined(CellStd[v].E[3]) == 0)
				swap(CellStd[v].E[1], CellStd[v].E[3]);
		}
}

// Definition of analyse Method in MESH Class : Analyse the given mesh data and define zone number of each cell in the maesh
void MESH::analyse() {
	// part 1:
	ZNum = 1;
	// part 2:
	for (int i = 0; i < NC; i++)
		CellStd[i].CellZnum = 0;
	// part 3:
	for (int k = 0; k < NC; k++) {
		CELL* C = &CellStd[k];
		int n = 0;
		int t;
		if (C->CellZnum == 0) {
			C->CellZnum = ZNum;
			C_stack[n++] = C;

			// part 4:
			while (n != 0) {
				C_stack[0]->CellZnum = ZNum;

				//part 5:
				for (int i = 0; i < C_stack[0]->CellEdgSize; i++) {
					if (C_stack[0]->E[i]->state() == 1) {
						t = find(C_stack[0]->E[i], cell);
						if (Cell[0] != C_stack[0] && Cell[0]->CellZnum == 0)
							C_stack[n++] = Cell[0];
						if (Cell[1] != C_stack[0] && Cell[1]->CellZnum == 0)
							C_stack[n++] = Cell[1];
					}
				}

				// part 6:
				swap(C_stack[n - 1], C_stack[0]);
				n--;
			}

			// part 7:
			ZNum++;
		}

		
	}
	
}

// Definition of fill_zone Method in Mesh : Extract data of each specified zone
void MESH::fill_zone(vector<ZONE>& Z) {

	// Part 1:
	Z.clear();
	Z.resize(ZNum - 1);
	set<EDGE*> E;
	set<POINT*> P;

	// Part 2:
	for (int i = 0; i < NC; i++) {
		Z[CellStd[i].CellZnum - 1].ZCell.push_back(&CellStd[i]);
	}

	// Part 3:
	for (int i = 0; i < Z.size(); i++) {
		for (int j = 0; j < Z[i].ZCell.size(); j++)
			for (int k = 0; k < Z[i].ZCell[j]->CellEdgSize; k++)
				E.emplace(Z[i].ZCell[j]->E[k]);
		copy(E.begin(), E.end(), back_inserter(Z[i].ZEdge));
		E.clear();
	}

	// Part 4:
	int m;
	for (int i = 0; i < Z.size(); i++) {
		for (int j = 0; j < Z[i].ZCell.size(); j++) {
			m = find(Z[i].ZCell[j], point);
			for (int k = 0; k < m; k++)
				P.emplace(Point[k]);
		}
		copy(P.begin(), P.end(), back_inserter(Z[i].ZPoint));
		P.clear();
	}

	// Part 5:
	for (int i = 0; i < Z.size(); i++)
		for (int j = 0; j < Z[i].ZEdge.size(); j++) {
			if (Z[i].ZEdge[j]->state() == 0)
				Z[i].CurveBound.push_back(Z[i].ZEdge[j]);
			if (Z[i].ZEdge[j]->state() == 2) {
				Z[i].CurveInterface.push_back(Z[i].ZEdge[j]);
				Z[i].ZEdge[j]->is_interface = 1;
				Z[i].ZEdge[j]->P[0]->is_on_interface = 1;
				Z[i].ZEdge[j]->P[1]->is_on_interface = 1;
			}
		}

	// Part 6:
	for (int i = 0; i < Z.size(); i++)
		if (Z[i].ZCell[0]->CellEdgSize != 3)
			Z[i].ZType = ZONE::hybrid;

	// Part 7:
	for (int i = 0; i < Z.size(); i++) {
		cout << endl << "zone number: " << i + 1 << "\tzone type: " << Z[i].ZType << endl;
		cout << "cell number: " << Z[i].ZCell.size() << endl;
		cout << "edge number: " << Z[i].ZEdge.size() << endl;
		cout << "point number: " << Z[i].ZPoint.size() << endl;
	}

	// Part 8:
	for (int i = 0; i < Z.size(); i++)
		if (Z[i].ZType == 1)
			Z[i].order();
}

EDGE* MESH::find(POINT* p1, POINT* p2) {
	for (int j = 0; j < p1->PointEdgSize; j++)
		for (int k = 0; k < p2->PointEdgSize; k++)
			if (p1->PointEdgs[j] == p2->PointEdgs[k])
				return p1->PointEdgs[j];
	return nullptr;
}



// Definition of delcheck Method in MESH Class : check delauny condition for two cell neigbour of specified edge 
int MESH::delcheck(EDGE* E) {
	POINT* P1 = nullptr;   //non_common point
	POINT* P2 = nullptr;   //non_common point
	double sinP1;
	double sinP2;
	double cosP1;
	double cosP2;
	find(E, cell);
	if (E->state() == 1 && Cell[0]->CellEdgSize == 3) {
		for (int i = 0; i < 3; i++)
			if (Cell[0]->E[i] != E)
				if (Cell[0]->E[i]->P[0] != E->P[0] && Cell[0]->E[i]->P[0] != E->P[1]) {
					P1 = Cell[0]->E[i]->P[0];
					break;
				}
				else {
					P1 = Cell[0]->E[i]->P[1];
					break;
				}

				for (int i = 0; i < 3; i++)
					if (Cell[1]->E[i] != E)
						if (Cell[1]->E[i]->P[0] != E->P[0] && Cell[1]->E[i]->P[0] != E->P[1]) {
							P2 = Cell[1]->E[i]->P[0];
							break;
						}
						else {
							P2 = Cell[1]->E[i]->P[1];
							break;
						}

						cosP1 = Cell[0]->cosin(P1);
						sinP1 = sqrt(1 - cosP1 * cosP1);

						cosP2 = Cell[1]->cosin(P2);
						sinP2 = sqrt(1 - cosP2 * cosP2);

						if (sinP1*cosP2 + sinP2 * cosP1 < 0)
							return 0;
						else
							return 1;

	}
	else
		return -1;

}

//Definition of delauny Method in MESH Class : 
void MESH::do_delauney(EDGE* E) {

	if (delcheck(E) == 0) {

		// Part 1 :
		find(E, cell);
		CELL C1 = *Cell[0];
		CELL C2 = *Cell[1];

		// Part 2 :
		EDGE* edge[5];
		int counter = 3;
		copy(C1.E, C1.E + C1.CellEdgSize, edge);

		// Part 3 :
		for (int i = 0; i < 3; i++)
			if (C2.E[i] != E)
				edge[counter++] = C2.E[i];

		// Part 4 :
		for (int i = 0; i<3; i++)
			if (edge[i] == E) {
				swap(edge[i], edge[0]);
				break;
			}

		// Part 5 :
		for (int j = 0; j<2; j++)
			for (int i = 1; i<4; i++)
				if (edge[i]->P[0] != E->P[0] && edge[i]->P[1] != E->P[0]) {
					swap(edge[i], edge[i + 1]);

				}

		// Part 6 :
		EDGE E_old = *E;
		if (edge[1]->P[0] != E_old.P[0])
			E->P[0] = edge[1]->P[0];
		else
			E->P[0] = edge[1]->P[1];
		if (edge[2]->P[0] != E_old.P[0])
			E->P[1] = edge[2]->P[0];
		else
			E->P[1] = edge[2]->P[1];

		// Part 7 :
		Cell[0]->E[0] = edge[0];
		Cell[0]->E[1] = edge[1];
		Cell[0]->E[2] = edge[2];
		Cell[1]->E[0] = edge[0];
		Cell[1]->E[1] = edge[3];
		Cell[1]->E[2] = edge[4];

		// Part 8 :
		EDGE** e;
		e = E_old.P[0]->PointEdgs;
		for (int i = 0; i < E_old.P[0]->PointEdgSize; i++)
			if (e[i] == E) {
				E_old.P[0]->PointRemEdg(i);
				break;
			}

		// Part 9 :
		e = E_old.P[1]->PointEdgs;
		for (int i = 0; i < E_old.P[1]->PointEdgSize; i++)
			if (e[i] == E) {
				E_old.P[1]->PointRemEdg(i);
				break;
			}

		// Part 10 :
		E->P[0]->PointAddEdg(E);
		E->P[1]->PointAddEdg(E);

		// Part 11 :
		CELL** c;
		c = E_old.P[0]->PointCells;
		for (int i = 0; i < E_old.P[0]->PointCellSize; i++)
			if (c[i] == Cell[1]) {
				E_old.P[0]->PointRemCell(i);
				break;
			}

		// Part 12 :
		c = E_old.P[1]->PointCells;
		for (int i = 0; i < E_old.P[1]->PointCellSize; i++)
			if (c[i] == Cell[0]) {
				E_old.P[1]->PointRemCell(i);
				break;
			}

		// Part 13 :
		E->P[0]->PointAddCell(Cell[1]);
		E->P[1]->PointAddCell(Cell[0]);
		
	}
}


void MESH::flip22(EDGE* E) {

	// Part 1 :
	find(E, cell);
	CELL C1 = *Cell[0];
	CELL C2 = *Cell[1];

	// Part 2 :
	EDGE* edge[5];
	int counter = 3;
	copy(C1.E, C1.E + C1.CellEdgSize, edge);

	// Part 3:
	for (int i = 0; i < 3; i++)
		if (C2.E[i] != E)
			edge[counter++] = C2.E[i];

	// Part 4:
	for (int i = 0; i < 3; i++)
		if (edge[i] == E) {
			swap(edge[i], edge[0]);
			break;
		}

	// Part 5 :
	for (int j = 0; j < 2; j++)
		for (int i = 1; i < 4; i++)
			if (edge[i]->P[0] != E->P[0] && edge[i]->P[1] != E->P[0]) {
				swap(edge[i], edge[i + 1]);

			}

	// Part 6:
	EDGE E_old = *E;
	if (edge[1]->P[0] != E_old.P[0])
		E->P[0] = edge[1]->P[0];
	else
		E->P[0] = edge[1]->P[1];
	if (edge[2]->P[0] != E_old.P[0])
		E->P[1] = edge[2]->P[0];
	else
		E->P[1] = edge[2]->P[1];

	// Part 7 :
	Cell[0]->E[0] = edge[0];
	Cell[0]->E[1] = edge[1];
	Cell[0]->E[2] = edge[2];
	Cell[1]->E[0] = edge[0];
	Cell[1]->E[1] = edge[3];
	Cell[1]->E[2] = edge[4];

	// Part 8 :
	EDGE** e;
	e = E_old.P[0]->PointEdgs;
	for (int i = 0; i < E_old.P[0]->PointEdgSize; i++)
		if (e[i] == E) {
			E_old.P[0]->PointRemEdg(i);
			break;
		}

	// Part 9 :
	e = E_old.P[1]->PointEdgs;
	for (int i = 0; i < E_old.P[1]->PointEdgSize; i++)
		if (e[i] == E) {
			E_old.P[1]->PointRemEdg(i);
			break;
		}

	// Part 10 :
	E->P[0]->PointAddEdg(E);
	E->P[1]->PointAddEdg(E);

	// Part 11 :
	CELL** c;
	c = E_old.P[0]->PointCells;
	for (int i = 0; i < E_old.P[0]->PointCellSize; i++)
		if (c[i] == Cell[1]) {
			E_old.P[0]->PointRemCell(i);
			break;
		}

	// Part 12 :
	c = E_old.P[1]->PointCells;
	for (int i = 0; i < E_old.P[1]->PointCellSize; i++)
		if (c[i] == Cell[0]) {
			E_old.P[1]->PointRemCell(i);
			break;
		}

	// Part 13 :
	E->P[0]->PointAddCell(Cell[1]);
	E->P[1]->PointAddCell(Cell[0]);
}

//Definition of add Method in MESH Class : this method takes only simple cell (triangle) and devides it to three new cell by adding one point and three edge
void MESH::flip13(CELL* C) {
	if (C->CellEdgSize == 3) {

		// Part 1 :
		CELL* c_a = CellStd.data();
		EDGE* e_a = EdgStd.data();
		POINT* p_a = PointStd.data();

		// Part 2 :
		find(C, point);
		CELL C_old = *C;

		// Part 3 :
		double x;
		double y;
		x = (Point[0]->x + Point[1]->x + Point[2]->x) / 3;
		y = (Point[0]->y + Point[1]->y + Point[2]->y) / 3;

		// Part 4 :
		PointStd.resize(NP + 1);
		NP++;

		// Part 5 :
		POINT* Pp = &PointStd[NP - 1]; //new point pointer
		Pp->x = x;
		Pp->y = y;

		// Part 6 :
		int region;
		for (int i = 0; i < 3; i++)
			if (C_old.E[i]->state() != 0) {
				region = C_old.E[i]->region;
				break;
			}

		// Part 7 :
		EDGE E[3];
		E[0].P[0] = Pp;
		E[1].P[0] = Pp;
		E[2].P[0] = Pp;
		E[0].P[1] = Point[0];
		E[1].P[1] = Point[1];
		E[2].P[1] = Point[2];

		// Part 8 :
		for (int i = 0; i < 3; i++)
			E[i].region = region;

		// Part 9 :
		EdgStd.resize(NF + 3);
		NF = NF + 3;
		e_a[NF - 3] = E[0];
		e_a[NF - 2] = E[1];
		e_a[NF - 1] = E[2];

		// Part 10 :
		CELL C1;
		CELL C2;
		C->E[0] = &e_a[NF - 3];
		C->E[1] = &e_a[NF - 2];
		C1.CellAddEdg(&e_a[NF - 2]);
		C1.CellAddEdg(&e_a[NF - 1]);
		C2.CellAddEdg(&e_a[NF - 1]);
		C2.CellAddEdg(&e_a[NF - 3]);

		// Part 11 :
		EDGE E_fake[2];
		E_fake[0].P[0] = Point[0];
		E_fake[0].P[1] = Point[1];
		E_fake[1].P[0] = Point[1];
		E_fake[1].P[1] = Point[2];

		// Part 12 :
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				if (!(*C_old.E[i] == E_fake[i]))
					swap(C_old.E[i], C_old.E[i + 1]);

		}

		// Part 13 :
		C->E[2] = C_old.E[0];
		C1.CellAddEdg(C_old.E[1]);
		C2.CellAddEdg(C_old.E[2]);

		// Part 14 :
		CellStd.resize(NC + 2);
		NC = NC + 2;
		c_a[NC - 2] = C1;
		c_a[NC - 1] = C2;

		// Part 15 : 
		c_a[NC - 2].CellZnum = C_old.CellZnum;
		c_a[NC - 1].CellZnum = C_old.CellZnum;

		// Part 16 :
		p_a[NP - 1].PointAddEdg(&e_a[NF - 3]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);

		// Part 17 :
		Point[0]->PointAddEdg(&e_a[NF - 3]);
		Point[1]->PointAddEdg(&e_a[NF - 2]);
		Point[2]->PointAddEdg(&e_a[NF - 1]);

		// Part 18 :
		for (int i = 0; i < Point[2]->PointCellSize; i++)
			if (Point[2]->PointCells[i] == C)
				Point[2]->PointRemCell(i);

		// Part 19 :
		p_a[NP - 1].PointAddCell(C);
		p_a[NP - 1].PointAddCell(&c_a[NC - 2]);
		p_a[NP - 1].PointAddCell(&c_a[NC - 1]);

		// Part 20 :
		Point[0]->PointAddCell(&c_a[NC - 1]);
		Point[1]->PointAddCell(&c_a[NC - 2]);
		Point[2]->PointAddCell(&c_a[NC - 1]);
		Point[2]->PointAddCell(&c_a[NC - 2]);
		
	}
}
void MESH::flip13(CELL* C,bPOINT bP) {
	if (C->CellEdgSize == 3 ) {

		// Part 1 :
		CELL* c_a = CellStd.data();
		EDGE* e_a = EdgStd.data();
		POINT* p_a = PointStd.data();

		// Part 2 :
		find(C, point);
		CELL C_old = *C;

		// Part 3 :
		double x;
		double y;
		x = bP.x;
		y = bP.y;

		// Part 4 :
		PointStd.resize(NP + 1);
		NP++;

		// Part 5 : 
		POINT* Pp = &PointStd[NP - 1]; //new point pointer
		Pp->x = x;
		Pp->y = y;

		// Part 6 :
		int region;
		for (int i = 0; i < 3; i++)
			if (C_old.E[i]->state() != 0) {
				region = C_old.E[i]->region;
				break;
			}

		// Part 7 :
		EDGE E[3];
		E[0].P[0] = Pp;
		E[1].P[0] = Pp;
		E[2].P[0] = Pp;
		E[0].P[1] = Point[0];
		E[1].P[1] = Point[1];
		E[2].P[1] = Point[2];

		// Part 8 :
		for (int i = 0; i < 3; i++)
			E[i].region = region;

		// Part 9 :
		EdgStd.resize(NF + 3);
		NF = NF + 3;
		e_a[NF - 3] = E[0];
		e_a[NF - 2] = E[1];
		e_a[NF - 1] = E[2];

		// Part 10 :
		CELL C1;
		CELL C2;
		C->E[0] = &e_a[NF - 3];
		C->E[1] = &e_a[NF - 2];
		C1.CellAddEdg(&e_a[NF - 2]);
		C1.CellAddEdg(&e_a[NF - 1]);
		C2.CellAddEdg(&e_a[NF - 1]);
		C2.CellAddEdg(&e_a[NF - 3]);

		// Part 11 : 
		EDGE E_fake[2];
		E_fake[0].P[0] = Point[0];
		E_fake[0].P[1] = Point[1];
		E_fake[1].P[0] = Point[1];
		E_fake[1].P[1] = Point[2];

		// Part 12 :
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++)
				if (!(*C_old.E[i] == E_fake[i]))
					swap(C_old.E[i], C_old.E[i + 1]);

		}

		// Part 13 :
		C->E[2] = C_old.E[0];
		C1.CellAddEdg(C_old.E[1]);
		C2.CellAddEdg(C_old.E[2]);

		// Part 14 :
		CellStd.resize(NC + 2);
		NC = NC + 2;
		c_a[NC - 2] = C1;
		c_a[NC - 1] = C2;

		// Part 15 :
		c_a[NC - 2].CellZnum = C_old.CellZnum;
		c_a[NC - 1].CellZnum = C_old.CellZnum;

		// Part 16 :
		p_a[NP - 1].PointAddEdg(&e_a[NF - 3]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);

		// Part 17 :
		Point[0]->PointAddEdg(&e_a[NF - 3]);
		Point[1]->PointAddEdg(&e_a[NF - 2]);
		Point[2]->PointAddEdg(&e_a[NF - 1]);

		// Part 18 :
		for (int i = 0; i < Point[2]->PointCellSize; i++)
			if (Point[2]->PointCells[i] == C)
				Point[2]->PointRemCell(i);

		// Part 19 : 
		p_a[NP - 1].PointAddCell(C);
		p_a[NP - 1].PointAddCell(&c_a[NC - 2]);
		p_a[NP - 1].PointAddCell(&c_a[NC - 1]);

		// Part 20 :
		Point[0]->PointAddCell(&c_a[NC - 1]);
		Point[1]->PointAddCell(&c_a[NC - 2]);
		Point[2]->PointAddCell(&c_a[NC - 1]);
		Point[2]->PointAddCell(&c_a[NC - 2]);
		
	}
}

//Definition of add Method in MESH Class : this method takes zone_interior edge and boundary edge 
void MESH::flip24(EDGE * Ep) {
	// part 1:
	CELL* c_a = CellStd.data();
	EDGE* e_a = EdgStd.data();
	POINT* p_a = PointStd.data();

	// part 2:
	int n = find(Ep, cell);  //number of cell neighbors
	CELL* C[2] = { Cell[0], Cell[1] };
	EDGE E = *Ep;

	// part3 :
	if (n == 1) {
		int m = find(C[0], point);
		if (m == 3) {
			// part 4:
			CELL C1 = *C[0];
			for (int i = 0; i < 3; i++)
				if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
					swap(Point[i], Point[0]);
					break;
				}

			// part 5 :
			bPOINT P = E.mid();
			PointStd.resize(NP + 1);
			NP++;
			p_a[NP - 1].x = P.x;
			p_a[NP - 1].y = P.y;
			p_a[NP - 1].is_boundary = 1;

			// part 6:
			int region;
			for (int i = 0; i < 3; i++)
				if (C1.E[i]->state() != 0) {
					region = C1.E[i]->region;
					break;
				}

			// part 7 :
			Ep->P[0] = &p_a[NP - 1];
			EdgStd.resize(NF + 2);
			NF = NF + 2;
			e_a[NF - 2].P[0] = &p_a[NP - 1];
			e_a[NF - 2].P[1] = E.P[0];
			e_a[NF - 2].region = E.region;
			e_a[NF - 1].P[0] = &p_a[NP - 1];
			e_a[NF - 1].P[1] = Point[0];
			e_a[NF - 1].region = region;

			// part 8:
			CellStd.resize(NC + 1);
			NC++;

			// part 9 :
			EDGE E_fake;
			E_fake.P[0] = Point[0];
			E_fake.P[1] = E.P[0];
			for (int i = 0; i < 3; i++)
				if (E_fake == *C1.E[i]) {
					c_a[NC - 1].CellAddEdg(C1.E[i]);
					C[0]->E[i] = &e_a[NF - 1];
					break;
				}

			// part 10 :
			c_a[NC - 1].CellAddEdg(&e_a[NF - 1]);
			c_a[NC - 1].CellAddEdg(&e_a[NF - 2]);
			c_a[NC - 1].CellZnum = C1.CellZnum;

			// part 11 :
			p_a[NP - 1].PointAddEdg(Ep);
			p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
			p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);

			// part 12 :
			EDGE** e = E.P[0]->PointEdgs;
			for (int i = 0; i < E.P[0]->PointEdgSize; i++)
				if (e[i] == Ep) {
					E.P[0]->PointRemEdg(i);
					break;
				}

			// part 13 :
			E.P[0]->PointAddEdg(&e_a[NF - 2]);
			Point[0]->PointAddEdg(&e_a[NF - 1]);

			// part 14 :
			p_a[NP - 1].PointAddCell(C[0]);
			p_a[NP - 1].PointAddCell(&c_a[NC - 1]);
			Point[0]->PointAddCell(&c_a[NC - 1]);

			// part 15 :
			for (int i = 0; i < E.P[0]->PointCellSize; i++)
				if (E.P[0]->PointCells[i] == C[0]) {
					E.P[0]->PointRemCell(i);
					break;
				}

			// part 16 :
			E.P[0]->PointAddCell(&c_a[NC - 1]);


			

		}
	}
	// part 17 :
	if (n == 2 && C[0]->CellEdgSize == 3 && C[1]->CellEdgSize == 3) {
		CELL C1 = *C[0];
		CELL C2 = *C[1];

		// part 18 :
		find(Cell[0], point);
		POINT* P[4];
		copy(Point, Point + 3, P);

		// part 19 :
		find(Cell[1], point);
		for (int i = 0; i < 3; i++)
			if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
				P[3] = Point[i];
				break;
			}

		// part 20 :
		for (int i = 0; i < 3; i++)
			if (P[i] != E.P[0] && P[i] != E.P[1])
				swap(P[i], P[2]);

		// part 21 :
		bPOINT mP = E.mid();
		PointStd.resize(NP + 1);
		NP++;
		p_a[NP - 1].x = mP.x;
		p_a[NP - 1].y = mP.y;
		Ep->P[0] = &p_a[NP - 1];

		// part 22 :
		EdgStd.resize(NF + 3);
		NF = NF + 3;
		EDGE En[3];
		En[0].P[0] = &p_a[NP - 1];
		En[1].P[0] = &p_a[NP - 1];
		En[2].P[0] = &p_a[NP - 1];
		En[0].P[1] = E.P[0];
		En[1].P[1] = P[2];
		En[2].P[1] = P[3];
		En[0].region = E.region;
		En[1].region = E.region;
		En[2].region = E.region;

		e_a[NF - 3] = En[0];
		e_a[NF - 2] = En[1];
		e_a[NF - 1] = En[2];
	
		
		

		// part 23 :
		CellStd.resize(NC + 2);
		NC = NC + 2;
		c_a[NC - 2].CellAddEdg(&e_a[NF - 3]);
		c_a[NC - 2].CellAddEdg(&e_a[NF - 2]);
		c_a[NC - 1].CellAddEdg(&e_a[NF - 3]);
		c_a[NC - 1].CellAddEdg(&e_a[NF - 1]);


		// part 24 :
		EDGE E_fake;
		E_fake.P[0] = P[2];
		E_fake.P[1] = E.P[0];
		for (int i = 0; i < 3; i++)
			if (E_fake == *C1.E[i]) {
				c_a[NC - 2].CellAddEdg(C1.E[i]);
				C[0]->E[i] = &e_a[NF - 2];
				break;
			}

		// part 25 :
		E_fake.P[0] = P[3];
		E_fake.P[1] = E.P[0];
		for (int i = 0; i < 3; i++)
			if (E_fake == *C2.E[i]) {
				c_a[NC - 1].CellAddEdg(C2.E[i]);
				C[1]->E[i] = &e_a[NF - 1];
				break;
			}
		c_a[NC - 2].CellZnum = C1.CellZnum;
		c_a[NC - 1].CellZnum = C1.CellZnum;

		// part 26 :
		p_a[NP - 1].PointAddEdg(&e_a[NF - 3]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);
		p_a[NP - 1].PointAddEdg(Ep);

		// part 27 :
		EDGE** e = E.P[0]->PointEdgs;
		for (int i = 0; i<E.P[0]->PointEdgSize; i++)
			if (e[i] == Ep) {
				E.P[0]->PointRemEdg(i);
				break;
			}

		// part 28 :
		E.P[0]->PointAddEdg(&e_a[NF - 3]);
		P[2]->PointAddEdg(&e_a[NF - 2]);
		P[3]->PointAddEdg(&e_a[NF - 1]);

		// part 29 :
		p_a[NP - 1].PointAddCell(C[0]);
		p_a[NP - 1].PointAddCell(C[1]);
		p_a[NP - 1].PointAddCell(&c_a[NC - 2]);
		p_a[NP - 1].PointAddCell(&c_a[NC - 1]);

		// part 30 :
		CELL** c = E.P[0]->PointCells;
		for (int i = 0; i < E.P[0]->PointCellSize; i++)
			if (c[i] == C[0] || c[i] == C[1])
				E.P[0]->PointRemCell(i--);

		// part 31 :
		E.P[0]->PointAddCell(&c_a[NC - 2]);
		E.P[0]->PointAddCell(&c_a[NC - 1]);
		P[2]->PointAddCell(&c_a[NC - 2]);
		P[3]->PointAddCell(&c_a[NC - 1]);

	}
}

void MESH::flip24(EDGE * Ep,bPOINT bP) {
	// part 1 :
	CELL* c_a = CellStd.data();
	EDGE* e_a = EdgStd.data();
	POINT* p_a = PointStd.data();

	// part 2 :
	int n = find(Ep, cell);  //number of cell neighbors
	CELL* C[2] = { Cell[0], Cell[1] };
	EDGE E = *Ep;

	// part 3 :
	if (n == 1  ) {
		int m = find(C[0], point);
		if (m == 3) {

			// part 4 :
			CELL C1 = *C[0];
			for (int i = 0; i < 3; i++)
				if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
					swap(Point[i], Point[0]);
					break;
				}

			// part 5 :
			PointStd.resize(NP + 1);
			NP++;
			p_a[NP - 1].x = bP.x;
			p_a[NP - 1].y = bP.y;
			p_a[NP - 1].is_boundary = 1;

			// part 6 :
			int region;
			for (int i = 0; i < 3; i++)
				if (C1.E[i]->state() != 0) {
					region = C1.E[i]->region;
					break;
				}

			// part 7 :
			Ep->P[0] = &p_a[NP - 1];
			EdgStd.resize(NF + 2);
			NF = NF + 2;
			e_a[NF - 2].P[0] = &p_a[NP - 1];
			e_a[NF - 2].P[1] = E.P[0];
			e_a[NF - 2].region = E.region;
			e_a[NF - 1].P[0] = &p_a[NP - 1];
			e_a[NF - 1].P[1] = Point[0];
			e_a[NF - 1].region = region;

			// part 8 :
			CellStd.resize(NC + 1);
			NC++;
			
			// part 9 :
			EDGE E_fake;
			E_fake.P[0] = Point[0];
			E_fake.P[1] = E.P[0];
			for (int i = 0; i < 3; i++)
				if (E_fake == *C1.E[i]) {
					c_a[NC - 1].CellAddEdg(C1.E[i]);
					C[0]->E[i] = &e_a[NF - 1];
					break;
				}

			// part 10 :
			c_a[NC - 1].CellAddEdg(&e_a[NF - 1]);
			c_a[NC - 1].CellAddEdg(&e_a[NF - 2]);
			c_a[NC - 1].CellZnum = C1.CellZnum;

			// part 11 :
			p_a[NP - 1].PointAddEdg(Ep);
			p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
			p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);

			// part 12 :
			EDGE** e = E.P[0]->PointEdgs;
			for (int i = 0; i < E.P[0]->PointEdgSize; i++)
				if (e[i] == Ep) {
					E.P[0]->PointRemEdg(i);
					break;
				}

			// part 13 :
			E.P[0]->PointAddEdg(&e_a[NF - 2]);
			Point[0]->PointAddEdg(&e_a[NF - 1]);

			// part 14 :
			p_a[NP - 1].PointAddCell(C[0]);
			p_a[NP - 1].PointAddCell(&c_a[NC - 1]);
			Point[0]->PointAddCell(&c_a[NC - 1]);

			// part 15 :
			for (int i = 0; i < E.P[0]->PointCellSize; i++)
				if (E.P[0]->PointCells[i] == C[0]) {
					E.P[0]->PointRemCell(i);
					break;
				}

			// part 16 :
			E.P[0]->PointAddCell(&c_a[NC - 1]);


			

		}
	}

	// part 17 :
	if (n == 2 && C[0]->CellEdgSize == 3 && C[1]->CellEdgSize == 3 ) {
		CELL C1 = *C[0];
		CELL C2 = *C[1];

		// part 18 :
		find(Cell[0], point);
		POINT* P[4];
		copy(Point, Point + 3, P);

		// part 19 :
		find(Cell[1], point);
		for (int i = 0; i < 3; i++)
			if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
				P[3] = Point[i];
				break;
			}

		// part 20 :
		for (int i = 0; i < 3; i++)
			if (P[i] != E.P[0] && P[i] != E.P[1])
				swap(P[i], P[2]);
		
		// part 21 :
		PointStd.resize(NP + 1);
		NP++;
		p_a[NP - 1].x = bP.x;
		p_a[NP - 1].y = bP.y;
		Ep->P[0] = &p_a[NP - 1];

		// part 22 :
		EdgStd.resize(NF + 3);
		NF = NF + 3;
		EDGE En[3];
		En[0].P[0] = &p_a[NP - 1];
		En[1].P[0] = &p_a[NP - 1];
		En[2].P[0] = &p_a[NP - 1];
		En[0].P[1] = E.P[0];
		En[1].P[1] = P[2];
		En[2].P[1] = P[3];
		En[0].region = E.region;
		En[1].region = E.region;
		En[2].region = E.region;

		e_a[NF - 3] = En[0];
		e_a[NF - 2] = En[1];
		e_a[NF - 1] = En[2];

	
		// part 23 :
		CellStd.resize(NC + 2);
		NC = NC + 2;
		c_a[NC - 2].CellAddEdg(&e_a[NF - 3]);
		c_a[NC - 2].CellAddEdg(&e_a[NF - 2]);
		c_a[NC - 1].CellAddEdg(&e_a[NF - 3]);
		c_a[NC - 1].CellAddEdg(&e_a[NF - 1]);


		// part 24 :
		EDGE E_fake;
		E_fake.P[0] = P[2];
		E_fake.P[1] = E.P[0];
		for (int i = 0; i < 3; i++)
			if (E_fake == *C1.E[i]) {
				c_a[NC - 2].CellAddEdg(C1.E[i]);
				C[0]->E[i] = &e_a[NF - 2];
				break;
			}

		// part 25 :
		E_fake.P[0] = P[3];
		E_fake.P[1] = E.P[0];
		for (int i = 0; i < 3; i++)
			if (E_fake == *C2.E[i]) {
				c_a[NC - 1].CellAddEdg(C2.E[i]);
				C[1]->E[i] = &e_a[NF - 1];
				break;
			}
		c_a[NC - 2].CellZnum = C1.CellZnum;
		c_a[NC - 1].CellZnum = C1.CellZnum;


		// part 26 :
		p_a[NP - 1].PointAddEdg(&e_a[NF - 3]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
		p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);
		p_a[NP - 1].PointAddEdg(Ep);

		// part 27 :
		EDGE** e = E.P[0]->PointEdgs;
		for (int i = 0; i<E.P[0]->PointEdgSize; i++)
			if (e[i] == Ep) {
				E.P[0]->PointRemEdg(i);
				break;
			}

		// part 28 :
		E.P[0]->PointAddEdg(&e_a[NF - 3]);
		P[2]->PointAddEdg(&e_a[NF - 2]);
		P[3]->PointAddEdg(&e_a[NF - 1]);

		// part 29 :
		p_a[NP - 1].PointAddCell(C[0]);
		p_a[NP - 1].PointAddCell(C[1]);
		p_a[NP - 1].PointAddCell(&c_a[NC - 2]);
		p_a[NP - 1].PointAddCell(&c_a[NC - 1]);

		// part 30 :
		CELL** c = E.P[0]->PointCells;
		for (int i = 0; i < E.P[0]->PointCellSize; i++)
			if (c[i] == C[0] || c[i] == C[1])
				E.P[0]->PointRemCell(i--);

		// part 31 :
		E.P[0]->PointAddCell(&c_a[NC - 2]);
		E.P[0]->PointAddCell(&c_a[NC - 1]);
		P[2]->PointAddCell(&c_a[NC - 2]);
		P[3]->PointAddCell(&c_a[NC - 1]);

	}
}



void MESH::laplacian() {

	// part 1 :
	POINT* p_a = PointStd.data();
	EDGE* e_a = EdgStd.data();
	CELL* c_a = CellStd.data();
	double a[MAX_NEIB] = { 0 };
	double an[MAX_NEIB] = { 0 };
	double A, B;
	bPOINT P[MAX_NEIB];
	bPOINT Cp;
	bPOINT Pold;

	// part 2 :
	for (int i = 0; i < mesh.NP; i++) {
		Cp.x = 0;
		Cp.y = 0;
		
		A = 0;
		B = 0;

		// part 3 :
		for (int j = 0; j < p_a[i].PointCellSize; j++) {
			if (p_a[i].PointCells[j]->CellEdgSize != 3) {
				goto lable;
			}

			// part 4 : 
			a[j] = p_a[i].PointCells[j]->area();
			A = A + a[j];
			P[j] = p_a[i].PointCells[j]->center();
			Cp.x = Cp.x + P[j].x;
			Cp.y = Cp.y + P[j].y;
		}

		// part 5 :
		Cp.x = Cp.x / p_a[i].PointCellSize;
		Cp.y = Cp.y / p_a[i].PointCellSize;
		Pold.x = p_a[i].x;
		Pold.y = p_a[i].y;
		p_a[i].x = Cp.x;
		p_a[i].y=Cp.y;
	

		// part 6 :
		for (int j = 0; j < p_a[i].PointCellSize; j++) {
			an[j] = p_a[i].PointCells[j]->area();
			B = B + an[j];
			if (an[j] / a[j] < .001 || an[j]<.0001*A) {
				p_a[i].x = Pold.x;
				p_a[i].y = Pold.y;
				goto lable;
			}
		}

		// part 7 :
		if (abs((A - B) / A) > ERROR1) {
			p_a[i].x = Pold.x;
			p_a[i].y = Pold.y;
			goto lable;
		}

	lable:
		;
	}
}





//Definition of add Method in MESH Class : this method takes interface edge only (private)

void MESH::I_add(EDGE* Ep) {
	// part 1 :
	CELL* c_a = CellStd.data();
	EDGE* e_a = EdgStd.data();
	POINT* p_a = PointStd.data();

	// part 2 :
	int n = find(Ep, cell);  //number of cell neighbors
	CELL* C[2] = { Cell[0], Cell[1] };
	EDGE E = *Ep;
	if (n == 2 && Ep->state() == 2) {
		if (C[1]->CellEdgSize == 3)
			swap(C[0], C[1]);

		// part 3 :
		int m = find(C[0], point);
		if (m == 3) {

			CELL C1 = *C[0];
			for (int i = 0; i < 3; i++)
				if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
					swap(Point[i], Point[0]);
					break;
				}
			// part 4 :
			bPOINT P = E.mid();
			PointStd.resize(NP + 1);
			NP++;
			p_a[NP - 1].x = P.x;
			p_a[NP - 1].y = P.y;
			p_a[NP - 1].is_on_interface = 1;

			// part 5:
			int region;
			for (int i = 0; i < 3; i++)
				if (C1.E[i]->state() != 0) {
					region = C1.E[i]->region;
					break;
				}

			// part 6 :
			Ep->P[0] = &p_a[NP - 1];
			EdgStd.resize(NF + 2);
			NF = NF + 2;
			e_a[NF - 2].P[0] = &p_a[NP - 1];
			e_a[NF - 2].P[1] = E.P[0];
			e_a[NF - 2].region = E.region;
			e_a[NF - 2].is_interface = 1;
			e_a[NF - 1].P[0] = &p_a[NP - 1];
			e_a[NF - 1].P[1] = Point[0];
			e_a[NF - 1].region = region;

			// part 7 :
			CellStd.resize(NC + 1);
			NC++;

			// part 8 :
			EDGE E_fake;
			E_fake.P[0] = Point[0];
			E_fake.P[1] = E.P[0];
			for (int i = 0; i < 3; i++)
				if (E_fake == *C1.E[i]) {
					c_a[NC - 1].CellAddEdg(C1.E[i]);
					C[0]->E[i] = &e_a[NF - 1];
					break;
				}

			// part 9 :
			c_a[NC - 1].CellAddEdg(&e_a[NF - 1]);
			c_a[NC - 1].CellAddEdg(&e_a[NF - 2]);
			c_a[NC - 1].CellZnum = C1.CellZnum;

			// part 10 :
			p_a[NP - 1].PointAddEdg(Ep);
			p_a[NP - 1].PointAddEdg(&e_a[NF - 2]);
			p_a[NP - 1].PointAddEdg(&e_a[NF - 1]);

			// part 11 :
			EDGE** e = E.P[0]->PointEdgs;
			for (int i = 0; i < E.P[0]->PointEdgSize; i++)
				if (e[i] == Ep) {
					E.P[0]->PointRemEdg(i);
					break;
				}

			// part 12 :
			E.P[0]->PointAddEdg(&e_a[NF - 2]);
			Point[0]->PointAddEdg(&e_a[NF - 1]);

			// part 13 :
			p_a[NP - 1].PointAddCell(C[0]);
			p_a[NP - 1].PointAddCell(&c_a[NC - 1]);
			Point[0]->PointAddCell(&c_a[NC - 1]);

			// part 14 :
			for (int i = 0; i < E.P[0]->PointCellSize; i++)
				if (E.P[0]->PointCells[i] == C[0]) {
					E.P[0]->PointRemCell(i);
					break;
				}

			// part 15 :
			E.P[0]->PointAddCell(&c_a[NC - 1]);
		}
	}

}

// Definition of EdgContract Method in MESH Class : this method takes interior edge of a simple zone, check condition , execute edge contraction and at the end execute delauny
bool MESH::EdgContract(EDGE* Ep) {

	// part 1 :
	double area[500];
	double arean[500];
	double sum = 0;
	double sumn = 0;
	int check_zero = 0;
	bPOINT P;

	// part 2 :
	if (Ep->P[0]->is_boundary == 1)
		swap(Ep->P[0], Ep->P[1]);
	if (Ep->P[0]->is_boundary == 1) {
		return false;
	}

	// part 3 :
	for (int j = 0; j < Ep->P[0]->PointCellSize; j++) {
		if (Ep->P[0]->PointCells[j]->CellEdgSize == 4) {
			swap(Ep->P[0], Ep->P[1]);
			return false;
		}

		// part 4 :
		area[j] = Ep->P[0]->PointCells[j]->area();
		sum = sum + area[j];
	}

	// part 5 :
	P.x = Ep->P[0]->x;
	P.y = Ep->P[0]->y;

	// part 6 :
	Ep->P[0]->x = Ep->P[1]->x;
	Ep->P[0]->y = Ep->P[1]->y;

	// part 7 :
	for (int j = 0; j < Ep->P[0]->PointCellSize; j++) {
		arean[j] = Ep->P[0]->PointCells[j]->area();
		sumn = sumn + arean[j];
		if (arean[j] < (sum/ Ep->P[0]->PointCellSize)*ERROR2)
			check_zero++;

	}

	// part 8 :
	Ep->P[0]->x = P.x;
	Ep->P[0]->y = P.y;
	if (!(abs(sum - sumn)/sum < ERROR1 && check_zero == 2)) {
		swap(Ep->P[0], Ep->P[1]);
		return false;
	}


	// part 9 :
	find(Ep, cell);
	CELL* C[2] = { Cell[0], Cell[1] };

	// part 10 :
	if (Ep->state() == 1 && C[0]->CellEdgSize == 3) {

		EDGE E = *Ep;
		CELL C1 = *C[0];
		CELL C2 = *C[1];
		find(C[0], point);
		POINT* P[4]; //point of cells
		copy(Point, Point + 3, P);

		// part 11 :
		find(C[1], point);
		for (int i = 0; i < 3; i++)
			if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
				P[3] = Point[i];
				break;
			}

		// part 12 :
		for (int i = 0; i < 3; i++)
			if (P[i] != E.P[0] && P[i] != E.P[1])
				swap(P[i], P[2]);
		if (P[0] != E.P[0]) {
			swap(P[0], P[1]);
		}

		// part 13 :
		EDGE* lE[4]; //edge of cells
		int m = 0;
		for (int i = 0; i < 3; i++) {
			if (C1.E[i] != Ep)
				lE[m++] = C1.E[i];
		}
		if (lE[0]->P[0] != P[0] && lE[0]->P[1] != P[0])
			swap(lE[0], lE[1]);

		// part 14 :
		for (int i = 0; i < 3; i++) {
			if (C2.E[i] != Ep)
				lE[m++] = C2.E[i];
		}
		if (lE[2]->P[0] != P[0] && lE[2]->P[1] != P[0])
			swap(lE[2], lE[3]);


		// part 15 :
		CELL** c = E.P[0]->PointCells;
		EDGE** e;
		for (int j = 0; j < E.P[0]->PointCellSize; j++) {
			e = c[j]->E;
			if (c[j] != C[0] && c[j] != C[1])
				for (int k = 0; k < 3; k++) {
					if (e[k] == lE[0]) {
						c[j]->E[k] = lE[1];

					}
					if (e[k] == lE[2]) {
						c[j]->E[k] = lE[3];

					}
				}
		}

		// part 16 :
		e = P[0]->PointEdgs;
		for (int i = 0; i < P[0]->PointEdgSize; i++) {
			if (e[i]->P[0] == P[0] && e[i] != Ep && e[i] != lE[0] && e[i] != lE[2])
				e[i]->P[0] = P[1];
			if (e[i]->P[1] == P[0] && e[i] != Ep && e[i] != lE[0] && e[i] != lE[2])
				e[i]->P[1] = P[1];
		}

		// part 17 :
		e = P[1]->PointEdgs;
		for (int i = 0; i < P[1]->PointEdgSize; i++)
			if (e[i] == Ep) {
				P[1]->PointRemEdg(i);
				break;
			}

		// part 18 :
		c = P[1]->PointCells;
		for (int i = 0; i<P[1]->PointCellSize; i++)
			if (c[i] == C[0] || c[i] == C[1]) {
				P[1]->PointRemCell(i);
				i--;
			}

		// part 19 :
		e = P[0]->PointEdgs;
		for (int i = 0; i < P[0]->PointEdgSize; i++)
			if (e[i] != Ep && e[i] != lE[0] && e[i] != lE[2]) {
				P[1]->PointAddEdg(e[i]);
			}

		// part 20 :
		c = P[0]->PointCells;
		for (int i = 0; i < P[0]->PointCellSize; i++)
			if (c[i] != C[0] && c[i] != C[1]) {
				P[1]->PointAddCell(c[i]);
			}

		// part 21 :
		e = P[2]->PointEdgs;
		for (int i = 0; i < P[2]->PointEdgSize; i++)
			if (e[i] == lE[0]) {
				P[2]->PointRemEdg(i);
				break;
			}

		// part 22 :
		c = P[2]->PointCells;
		for (int i = 0; i<P[2]->PointCellSize; i++)
			if (c[i] == C[0]) {
				P[2]->PointRemCell(i);
				break;
			}

		// part 23 :
		e = P[3]->PointEdgs;
		for (int i = 0; i < P[3]->PointEdgSize; i++)
			if (e[i] == lE[2]) {
				P[3]->PointRemEdg(i);
				break;
			}

		// part 24 :
		c = P[3]->PointCells;
		for (int i = 0; i<P[3]->PointCellSize; i++)
			if (c[i] == C[1]) {
				P[3]->PointRemCell(i);
				break;
			}

		// part 25 :
		D.push_back(P[0]);
		D.push_back(Ep);
		D.push_back(lE[0]);
		D.push_back(lE[2]);
		D.push_back(C[0]);
		D.push_back(C[1]);




		return true;

	}
	return false;
}

// Definition of dlt Method in MESH Class : this method takes interior edge of a simple zone, check condition , execute edge contraction and at the end do not execute delauny
bool MESH::dlt(EDGE* Ep) {

	// part 1 :
	double area[500];
	double arean[500];
	double sum = 0;
	double sumn = 0;
	int check_zero = 0;
	bPOINT P;

	// part 2 :
	if (Ep->P[0]->is_boundary == 1)
		swap(Ep->P[0], Ep->P[1]);
	if (Ep->P[0]->is_boundary == 1) {
		return false;
	}

	// part 3 :
	for (int j = 0; j < Ep->P[0]->PointCellSize; j++) {
		if (Ep->P[0]->PointCells[j]->CellEdgSize == 4) {
			swap(Ep->P[0], Ep->P[1]);
			return false;
		}

		// part 4 :
		area[j] = Ep->P[0]->PointCells[j]->area();
		sum = sum + area[j];
	}

	// part 5 :
	P.x = Ep->P[0]->x;
	P.y = Ep->P[0]->y;

	// part 6 :
	Ep->P[0]->x = Ep->P[1]->x;
	Ep->P[0]->y = Ep->P[1]->y;

	// part 7 :
	for (int j = 0; j < Ep->P[0]->PointCellSize; j++) {
		arean[j] = Ep->P[0]->PointCells[j]->area();
		sumn = sumn + arean[j];
		if (arean[j] < (sum / Ep->P[0]->PointCellSize)*ERROR2)
			check_zero++;

	}

	// part 8 :
	Ep->P[0]->x = P.x;
	Ep->P[0]->y = P.y;
	if (!(abs(sum - sumn) / sum < ERROR1 && check_zero == 2)) {
		swap(Ep->P[0], Ep->P[1]);
		return false;
	}


	// part 9 :
	find(Ep, cell);
	CELL* C[2] = { Cell[0], Cell[1] };

	// part 10 :
	if (Ep->state() == 1 && C[0]->CellEdgSize == 3) {
		
		EDGE E = *Ep;
		CELL C1 = *C[0];
		CELL C2 = *C[1];
		find(Cell[0], point);
		POINT* P[4];
		copy(Point, Point + 3, P);

		// part 11 :
		find(Cell[1], point);
		for (int i = 0; i < 3; i++)
			if (Point[i] != E.P[0] && Point[i] != E.P[1]) {
				P[3] = Point[i];
				break;
			}

		// part 12 :
		for (int i = 0; i < 3; i++)
			if (P[i] != E.P[0] && P[i] != E.P[1])
				swap(P[i], P[2]);
		if (P[0] != E.P[0]) {
			swap(P[0], P[1]);
		}

		// part 13 :
		EDGE* lE[4];
		int m = 0;
		for (int i = 0; i < 3; i++) {
			if (C1.E[i] != Ep)
				lE[m++] = C1.E[i];
		}
		if (lE[0]->P[0] != P[0] && lE[0]->P[1] != P[0])
			swap(lE[0], lE[1]);

		// part 14 :
		for (int i = 0; i < 3; i++) {
			if (C2.E[i] != Ep)
				lE[m++] = C2.E[i];
		}
		if (lE[2]->P[0] != P[0] && lE[2]->P[1] != P[0])
			swap(lE[2], lE[3]);

		// part 15 :
		CELL** c = E.P[0]->PointCells;
		EDGE** e;
		for (int j = 0; j < E.P[0]->PointCellSize; j++) {
			e = c[j]->E;
			if (c[j] != C[0] && c[j] != C[1])
				for (int k = 0; k < 3; k++) {
					if (e[k] == lE[0]) {
						c[j]->E[k] = lE[1];

					}
					if (e[k] == lE[2]) {
						c[j]->E[k] = lE[3];

					}
				}
		}

		// part 16 :
		e = P[0]->PointEdgs;
		for (int i = 0; i < P[0]->PointEdgSize; i++) {
			if (e[i]->P[0] == P[0] && e[i] != Ep && e[i] != lE[0] && e[i] != lE[2])
				e[i]->P[0] = P[1];
			if (e[i]->P[1] == P[0] && e[i] != Ep && e[i] != lE[0] && e[i] != lE[2])
				e[i]->P[1] = P[1];
		}

		// part 17 :
		e = P[1]->PointEdgs;
		for (int i = 0; i < P[1]->PointEdgSize; i++)
			if (e[i] == Ep) {
				P[1]->PointRemEdg(i);
				break;
			}

		// part 18 :
		c = P[1]->PointCells;
		for (int i = 0; i<P[1]->PointCellSize; i++)
			if (c[i] == C[0] || c[i] == C[1]) {
				P[1]->PointRemCell(i);
				i--;
			}

		// part 19 :
		e = P[0]->PointEdgs;
		for (int i = 0; i < P[0]->PointEdgSize; i++)
			if (e[i] != Ep && e[i] != lE[0] && e[i] != lE[2]) {
				P[1]->PointAddEdg(e[i]);
			}

		// part 20 :
		c = P[0]->PointCells;
		for (int i = 0; i < P[0]->PointCellSize; i++)
			if (c[i] != C[0] && c[i] != C[1]) {
				P[1]->PointAddCell(c[i]);
			}

		// part 21 :
		e = P[2]->PointEdgs;
		for (int i = 0; i < P[2]->PointEdgSize; i++)
			if (e[i] == lE[0]) {
				P[2]->PointRemEdg(i);
				break;
			}

		// part 22 :
		c = P[2]->PointCells;
		for (int i = 0; i<P[2]->PointCellSize; i++)
			if (c[i] == C[0]) {
				P[2]->PointRemCell(i);
				break;
			}

		// part 23 :
		e = P[3]->PointEdgs;
		for (int i = 0; i < P[3]->PointEdgSize; i++)
			if (e[i] == lE[2]) {
				P[3]->PointRemEdg(i);
				break;
			}

		// part 24 :
		c = P[3]->PointCells;
		for (int i = 0; i<P[3]->PointCellSize; i++)
			if (c[i] == C[1]) {
				P[3]->PointRemCell(i);
				break;
			}

		// part 25 :
		D.push_back(P[0]);
		D.push_back(Ep);
		D.push_back(lE[0]);
		D.push_back(lE[2]);
		D.push_back(C[0]);
		D.push_back(C[1]);




		return true;

	}
	return false;
}

void MESH::clear() {

	// part 1 :
	static unsigned long long a = reinterpret_cast<unsigned long long>(PointStd.data());
	static unsigned long long b = reinterpret_cast<unsigned long long>(EdgStd.data());
	static unsigned long long c = reinterpret_cast<unsigned long long>(CellStd.data());
	unsigned long long ae = reinterpret_cast<unsigned long long>(PointStd.data() + NP);
	unsigned long long be = reinterpret_cast<unsigned long long>(EdgStd.data() + NF);
	unsigned long long ce = reinterpret_cast<unsigned long long>(CellStd.data() + NC);

	// part 2 :
	sort(D.begin(), D.end());

	// part 3 :
	for (int i = D.size() - 1; i >= 0; i--) {
		unsigned long long v = reinterpret_cast<unsigned long long>(D[i]);
		if (v >= a && v < ae)
			deletedata(reinterpret_cast<POINT*>(D[i]));
		if (v >= b && v < be)
			deletedata(reinterpret_cast<EDGE*>(D[i]));
		if (v >= c && v < ce)
			deletedata(reinterpret_cast<CELL*>(D[i]));
	}
	D.clear();
}

void MESH::add(SURFACE& S, int C_num) {

	// part 1 :
	vector<CELL*> C = S.C[C_num];
	POINT* p_a = PointStd.data();
	EDGE* e_a = EdgStd.data();
	CELL* c_a = CellStd.data();

	


	// part 2 :
	CELL C1 = *C[0];				//first cell
	CELL C2 = *C[C.size() - 1];		//last cell
	EDGE* IE[4] = { nullptr, nullptr, nullptr, nullptr };	//array of interface edges
	int m = 0;		//number of boundary edges of first and last cell
	int n = 0;		//numbber of interface edges of first and last element
	int o = 0;		//0 for non_circular surface and 1 for circular
	int flag = 0;

	// part 3 :
	int t = find(C[0], cell);
	for (int i = 0; i<t; i++)
		if (Cell[i] == C[C.size() - 1]) {
			for (int j = 0; j < 4; j++)
				if (C[C.size() - 2]->is_in(C[1]->E[j]) == 0)
					flag++;
				
		}
	if (flag == 4)
		o = 1;

	// part 4 :
	for (int i = 0; i < 4; i++) {
		if (C1.E[i]->state() == 0) {
			m++;
		}
		if (C2.E[i]->state() == 0) {
			m++;
		}
	}

	// part 5 :
	for (int i = 0; i < 4; i++) {
		if (C1.E[i]->is_interface == 1) {
			IE[n++] = C1.E[i];
		}
	}
	for (int i = 0; i < 4; i++) {
		if (C2.E[i]->is_interface == 1) {
			IE[n++] = C2.E[i];
		}
	}

	// part 6 :
	if (n == 2 && o == 0) {
		POINT* P1;
		POINT* P2;
		if (IE[0]->P[1] != S.E[0]->P[0] && IE[0]->P[1] != S.E[0]->P[1])
			swap(IE[0]->P[0], IE[0]->P[1]);

		if (IE[1]->P[1] != S.E[C.size() - 1]->P[0] && IE[1]->P[1] != S.E[C.size() - 1]->P[1])
			swap(IE[1]->P[0], IE[1]->P[1]);
		P1 = IE[0]->P[0];
		P2 = IE[1]->P[0];


		// part 7 :
		I_add(IE[1]);
		I_add(IE[0]);

		// part 8 :
		bPOINT* Pn = new bPOINT[C.size() + 1];
		Pn[0].x = p_a[NP - 2].x;
		Pn[0].y = p_a[NP - 2].y;
		Pn[C.size()].x = p_a[NP - 1].x;
		Pn[C.size()].y = p_a[NP - 1].y;

		// part 9 :
		EDGE* lE = IE[0];
		for (int i = 0; i < C.size() - 1; i++) {
			lE = C[i]->E[C[i]->across(lE)];
			if (lE->P[1] != S.E[i]->P[0] && lE->P[1] != S.E[i]->P[1]) {
				swap(lE->P[0], lE->P[1]);
			}

			Pn[i + 1] = lE->mid();

		}

		// part 10 :
		PointStd.resize(NP + C.size() - 1);

		// part 11 :
		EdgStd.resize(NF + C.size());

		// part 12 :
		int check_E1 = NF;	//number of edges before adding horizontal edges
		int check_P = NP; //number of points before adding mid points

		//building horizontal edges and mid points
		// part 13 :
		for (int i = 1; i < C.size(); i++) {
			p_a[NP].x = Pn[i].x;
			p_a[NP].y = Pn[i].y;
			e_a[NF].P[0] = &p_a[NP - 1];
			e_a[NF].P[1] = &p_a[NP];

			NF++;
			NP++;
		}

		// part 14 :
		e_a[NF].P[0] = &p_a[check_P - 2];
		e_a[NF].P[1] = &p_a[NP - 1];
		NF++;

		// part 15 :
		int check_E2 = NF;	//number of edges before adding vertical edges
		EdgStd.resize(NF + C.size() - 1);

		// part 16 :
		lE = IE[0];
		for (int i = 0; i < C.size() - 1; i++) {
			lE = C[i]->E[C[i]->across(lE)];
			e_a[NF].P[0] = &p_a[check_P + i];
			e_a[NF].P[1] = lE->P[0];
			lE->P[0] = &p_a[check_P + i];
			NF++;
		}

		// part 17 :
		int check_C = NC;	//number of cell before building new cells
		CellStd.resize(NC + C.size());

		// part 18 :
		c_a[NC].CellAddEdg(C[0]->E[C[0]->across(S.E[0])]);
		C[0]->E[C[0]->across(S.E[0])] = &e_a[check_E1];
		c_a[NC].CellAddEdg(&e_a[check_E1]);
		c_a[NC].CellAddEdg(&e_a[check_E2]);
		c_a[NC].CellAddEdg(&e_a[check_E1 - 2]);
		NC++;

		// part 19 :
		for (int i = 1; i < C.size() - 1; i++) {
			c_a[NC].CellAddEdg(C[i]->E[C[i]->across(S.E[i])]);
			C[i]->E[C[i]->across(S.E[i])] = &e_a[check_E1 + i];
			c_a[NC].CellAddEdg(&e_a[check_E1 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i - 1]);
			NC++;
		}

		// part 20 :
		c_a[NC].CellAddEdg(C[C.size() - 1]->E[C[C.size() - 1]->across(S.E[C.size() - 1])]);
		C[C.size() - 1]->E[C[C.size() - 1]->across(S.E[C.size() - 1])] = &e_a[check_E1 + C.size() - 1];
		c_a[NC].CellAddEdg(&e_a[check_E1 + C.size() - 1]);
		c_a[NC].CellAddEdg(&e_a[check_E2 + C.size() - 2]);
		c_a[NC].CellAddEdg(&e_a[check_E1 - 4]);
		NC++;

		// part 21 :
		P1->PointAddCell(&c_a[check_C]);
		P2->PointAddCell(&c_a[check_C + C.size() - 1]);

		// part 22 :
		for (int i = 0; i < P1->PointCellSize; i++)
			if (P1->PointCells[i] == C[0]) {
				P1->PointRemCell(i);
			}

		// part 23 :
		for (int i = 0; i < P2->PointCellSize; i++)
			if (P2->PointCells[i] == C[C.size() - 1]) {
				P2->PointRemCell(i);
			}

		// part 24 :
		p_a[check_P - 1].PointAddEdg(&e_a[check_E1]);
		p_a[check_P - 2].PointAddEdg(&e_a[check_E2 - 1]);

		// part 25 :
		p_a[check_P - 1].PointAddCell(&c_a[check_C]);
		p_a[check_P - 1].PointAddCell(C[0]);
		p_a[check_P - 2].PointAddCell(&c_a[NC - 1]);
		p_a[check_P - 2].PointAddCell(C[C.size() - 1]);

		// part 26 :
		lE = IE[0];
		for (int i = 0; i < C.size() - 1; i++) {
			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i + 1]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E2 + i]);
			lE = C[i]->E[C[i]->across(lE)];

			// part 27 :
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointCellSize; j++)
				if (e_a[check_E2 + i].P[1]->PointCells[j] == C[i] || e_a[check_E2 + i].P[1]->PointCells[j] == C[i + 1])
					e_a[check_E2 + i].P[1]->PointRemCell(j--);

			// part 28 :
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointEdgSize; j++)
				if (e_a[check_E2 + i].P[1]->PointEdgs[j] == lE)
					e_a[check_E2 + i].P[1]->PointRemEdg(j);

			// part 29 :
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i]);
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i + 1]);
			e_a[check_E2 + i].P[1]->PointAddEdg(&e_a[check_E2 + i]);

			// part 30 :
			p_a[check_P + i].PointAddEdg(lE);
			p_a[check_P + i].PointAddCell(&c_a[check_C + i]);
			p_a[check_P + i].PointAddCell(&c_a[check_C + i + 1]);
			p_a[check_P + i].PointAddCell(C[i]);
			p_a[check_P + i].PointAddCell(C[i + 1]);
		}

	}

	// part 31 :
	if (n == 4 && o == 0) {
		EDGE* nE[2];
		int q = 0;

		// part 32 :
		for (int i = 0; i < 4; i++)
			if (C[0]->E[C[0]->across(IE[0])] == C[1]->E[i])
				nE[q++] = IE[0];
		if (q == 0)
			nE[q++] = IE[1];
		for (int i = 0; i < 4; i++)
			if (C2.E[C2.across(IE[2])] == C[C.size() - 2]->E[i])
				nE[q++] = IE[2];
		if (q == 1)
			nE[q++] = IE[3];

		// part 33 :
		IE[0] = nE[0];
		IE[1] = nE[1];

		// part 34 :
		POINT *P1;
		POINT *P2;
		if (IE[0]->P[1] != S.E[0]->P[0] && IE[0]->P[1] != S.E[0]->P[1])
			swap(IE[0]->P[0], IE[0]->P[1]);

		if (IE[1]->P[1] != S.E[C.size() - 1]->P[0] && IE[1]->P[1] != S.E[C.size() - 1]->P[1])
			swap(IE[1]->P[0], IE[1]->P[1]);
		P1 = IE[0]->P[0];
		P2 = IE[1]->P[0];
	
	
		// part 35 :
		I_add(IE[1]);
		I_add(IE[0]);


		// part 36 :
		bPOINT* Pn = new bPOINT[C.size() + 1];
		Pn[0].x = p_a[NP - 2].x;
		Pn[0].y = p_a[NP - 2].y;
		Pn[C.size()].x = p_a[NP - 1].x;
		Pn[C.size()].y = p_a[NP - 1].y;

		// part 37 :
		EDGE* lE = IE[0];
		for (int i = 0; i < C.size() - 1; i++) {
			lE = C[i]->E[C[i]->across(lE)];
			if (lE->P[1] != S.E[i]->P[0] && lE->P[1] != S.E[i]->P[1])
				swap(lE->P[0], lE->P[1]);

			Pn[i + 1] = lE->mid();

		}

		// part 38 :
		PointStd.resize(NP + C.size() - 1);

		// part 39 :
		EdgStd.resize(NF + C.size());

		// part 40 :
		int check_E1 = NF;
		int check_P = NP;

		// part 41 :
		for (int i = 1; i < C.size(); i++) {
			p_a[NP].x = Pn[i].x;
			p_a[NP].y = Pn[i].y;
			e_a[NF].P[0] = &p_a[NP - 1];
			e_a[NF].P[1] = &p_a[NP];

			NF++;
			NP++;
		}

		// part 42 :
		e_a[NF].P[0] = &p_a[check_P - 2];
		e_a[NF].P[1] = &p_a[NP - 1];
		NF++;

		// part 43 :
		int check_E2 = NF;
		EdgStd.resize(NF + C.size() - 1);

		// part 44 :
		lE = IE[0];
		for (int i = 0; i < C.size() - 1; i++) {
			lE = C[i]->E[C[i]->across(lE)];
			e_a[NF].P[0] = &p_a[check_P + i];
			e_a[NF].P[1] = lE->P[0];
			lE->P[0] = &p_a[check_P + i];
			NF++;
		}

		// part 45 :
		int check_C = NC;
		CellStd.resize(NC + C.size());

		// part 46:
		c_a[NC].CellAddEdg(C[0]->E[C[0]->across(S.E[0])]);
		C[0]->E[C[0]->across(S.E[0])] = &e_a[check_E1];
		c_a[NC].CellAddEdg(&e_a[check_E1]);
		c_a[NC].CellAddEdg(&e_a[check_E2]);
		c_a[NC].CellAddEdg(&e_a[check_E1 - 2]);
		NC++;

		// part 47 :
		for (int i = 1; i < C.size() - 1; i++) {
			c_a[NC].CellAddEdg(C[i]->E[C[i]->across(S.E[i])]);
			C[i]->E[C[i]->across(S.E[i])] = &e_a[check_E1 + i];
			c_a[NC].CellAddEdg(&e_a[check_E1 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i - 1]);
			NC++;
		}

		// part 48 :
		c_a[NC].CellAddEdg(C[C.size() - 1]->E[C[C.size() - 1]->across(S.E[C.size() - 1])]);
		C[C.size() - 1]->E[C[C.size() - 1]->across(S.E[C.size() - 1])] = &e_a[check_E1 + C.size() - 1];
		c_a[NC].CellAddEdg(&e_a[check_E1 + C.size() - 1]);
		c_a[NC].CellAddEdg(&e_a[check_E2 + C.size() - 2]);
		c_a[NC].CellAddEdg(&e_a[check_E1 - 4]);
		NC++;

		// part 49 :
		P1->PointAddCell(&c_a[check_C]);
		P2->PointAddCell(&c_a[check_C + C.size() - 1]);

		// part 50 :
		for (int i = 0; i < P1->PointCellSize; i++)
			if (P1->PointCells[i] == C[0]) {
				P1->PointRemCell(i);

			}

		// part 51 :
		for (int i = 0; i < P2->PointCellSize; i++)
			if (P2->PointCells[i] == C[C.size() - 1]) {
				P2->PointRemCell(i);

			}


		// part 52 :
		p_a[check_P - 1].PointAddEdg(&e_a[check_E1]);
		p_a[check_P - 2].PointAddEdg(&e_a[check_E2 - 1]);
		p_a[check_P - 1].PointAddCell(&c_a[check_C]);
		p_a[check_P - 1].PointAddCell(C[0]);
		p_a[check_P - 2].PointAddCell(&c_a[NC - 1]);
		p_a[check_P - 2].PointAddCell(C[C.size() - 1]);

		// part 53 :
		lE = IE[0];
		for (int i = 0; i < C.size() - 1; i++) {
			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i + 1]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E2 + i]);
			lE = C[i]->E[C[i]->across(lE)];

			// part 54 :
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointCellSize; j++)
				if (e_a[check_E2 + i].P[1]->PointCells[j] == C[i] || e_a[check_E2 + i].P[1]->PointCells[j] == C[i + 1])
					e_a[check_E2 + i].P[1]->PointRemCell(j--);

			// part 55 :
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointEdgSize; j++)
				if (e_a[check_E2 + i].P[1]->PointEdgs[j] == lE)
					e_a[check_E2 + i].P[1]->PointRemEdg(j);

			// part 56 :
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i]);
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i + 1]);
			e_a[check_E2 + i].P[1]->PointAddEdg(&e_a[check_E2 + i]);

			// part 57 :
			p_a[check_P + i].PointAddEdg(lE);
			p_a[check_P + i].PointAddCell(&c_a[check_C + i]);
			p_a[check_P + i].PointAddCell(&c_a[check_C + i + 1]);
			p_a[check_P + i].PointAddCell(C[i]);
			p_a[check_P + i].PointAddCell(C[i + 1]);
		}

	}

	// part 58 :
	if (n == 1 && m == 1 && o == 0) {

		if (C[0]->is_in(IE[0]) == false) {
			S.flip();
			C = S.C[C_num];
		}

		// part 59 :
		EDGE* EC1;
		if (IE[0]->P[1] != S.E[0]->P[0] && IE[0]->P[1] != S.E[0]->P[1])
			swap(IE[0]->P[0], IE[0]->P[1]);

		// part 60 :
		POINT* P1 = IE[0]->P[0];
		I_add(IE[0]);


		// part 61 :
		bPOINT* Pn = new bPOINT[C.size() + 1];
		Pn[0].x = p_a[NP - 1].x;
		Pn[0].y = p_a[NP - 1].y;

		// part 62 :
		EDGE* lE = IE[0];
		for (int i = 0; i < C.size(); i++) {
			lE = C[i]->E[C[i]->across(lE)];
			if (lE->P[1] != S.E[i]->P[0] && lE->P[1] != S.E[i]->P[1])
				swap(lE->P[0], lE->P[1]);

			Pn[i + 1] = lE->mid();

		}

		// part 63 :
		PointStd.resize(NP + C.size());

		// part 64 :
		EdgStd.resize(NF + C.size());

		// part 65 :
		int check_E1 = NF;
		int check_P = NP;

		// part 66 :
		for (int i = 0; i < C.size(); i++) {
			p_a[NP].x = Pn[i + 1].x;
			p_a[NP].y = Pn[i + 1].y;
			e_a[NF].P[0] = &p_a[NP - 1];
			e_a[NF].P[1] = &p_a[NP];

			NF++;
			NP++;
		}

		// part 67 :
		int check_E2 = NF;
		EdgStd.resize(NF + C.size());

		// part 68 :
		lE = IE[0];
		for (int i = 0; i < C.size(); i++) {
			lE = C[i]->E[C[i]->across(lE)];
			e_a[NF].P[0] = &p_a[check_P + i];
			e_a[NF].P[1] = lE->P[0];
			lE->P[0] = &p_a[check_P + i];
			NF++;
		}

		// part 69 :
		int check_C = NC;
		CellStd.resize(NC + C.size());

		// part 70 :
		c_a[NC].CellAddEdg(C[0]->E[C[0]->across(S.E[0])]);
		C[0]->E[C[0]->across(S.E[0])] = &e_a[check_E1];
		c_a[NC].CellAddEdg(&e_a[check_E1]);
		c_a[NC].CellAddEdg(&e_a[check_E2]);
		c_a[NC].CellAddEdg(&e_a[check_E1 - 2]);
		NC++;

		// part 71 :
		for (int i = 1; i < C.size(); i++) {
			c_a[NC].CellAddEdg(C[i]->E[C[i]->across(S.E[i])]);
			C[i]->E[C[i]->across(S.E[i])] = &e_a[check_E1 + i];
			c_a[NC].CellAddEdg(&e_a[check_E1 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i - 1]);
			NC++;
		}

		// part 72 :
		P1->PointAddCell(&c_a[check_C]);
		for (int i = 0; i < P1->PointCellSize; i++)
			if (P1->PointCells[i] == C[0]) {
				P1->PointRemCell(i);
			}

		// part 73 :
		p_a[check_P - 1].PointAddEdg(&e_a[check_E1]);
		p_a[check_P - 1].PointAddCell(&c_a[check_C]);
		p_a[check_P - 1].PointAddCell(C[0]);

		// part 74 :
		lE = IE[0];
		for (int i = 0; i < C.size(); i++) {
			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i]);
			if (i != C.size() - 1)
				p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i + 1]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E2 + i]);
			lE = C[i]->E[C[i]->across(lE)];
			p_a[check_P + i].PointAddEdg(lE);

			// part 75 :
			if (i == C.size() - 1)
				for (int j = 0; j < e_a[check_E2 + i].P[1]->PointCellSize; j++)
					if (e_a[check_E2 + i].P[1]->PointCells[j] == C[i])
						e_a[check_E2 + i].P[1]->PointRemCell(j);

			// part 76 :
			if (i != C.size() - 1)
				for (int j = 0; j < e_a[check_E2 + i].P[1]->PointCellSize; j++)
					if (e_a[check_E2 + i].P[1]->PointCells[j] == C[i] || e_a[check_E2 + i].P[1]->PointCells[j] == C[i + 1])
						e_a[check_E2 + i].P[1]->PointRemCell(j--);

			// part 77 :
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointEdgSize; j++)
				if (e_a[check_E2 + i].P[1]->PointEdgs[j] == lE)
					e_a[check_E2 + i].P[1]->PointRemEdg(j);
			

			// part 78 :
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i]);
			if (i != C.size() - 1)
				e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i + 1]);
			e_a[check_E2 + i].P[1]->PointAddEdg(&e_a[check_E2 + i]);
			

			// part 79 :
			p_a[check_P + i].PointAddCell(&c_a[check_C + i]);
			if (i != C.size() - 1)
				p_a[check_P + i].PointAddCell(&c_a[check_C + i + 1]);
			p_a[check_P + i].PointAddCell(C[i]);
			if (i != C.size() - 1)
				p_a[check_P + i].PointAddCell(C[i + 1]);



		}

	}

	// part 80 :
	if (o == 1) {

		EDGE* lE=nullptr;
		for (int i = 0; i < 4; i++)
			if (C[C.size() - 1]->is_in(C[0]->E[i]))
				lE = C[0]->E[i];

		// part 81 :
		bPOINT* Pn = new bPOINT[C.size()];
		for (int i = 0; i < C.size(); i++) {
			Pn[i] = lE->mid();
			lE = C[i]->E[C[i]->across(lE)];
			if (lE->P[1] != S.E[i]->P[0] && lE->P[1] != S.E[i]->P[1])
				swap(lE->P[0], lE->P[1]);


		}

		// part 82 :
		PointStd.resize(NP + C.size());

		// part 83 :
		EdgStd.resize(NF + C.size());

		// part 84 :
		int check_E1 = NF;
		int check_P = NP;

		// part 85:
		p_a[NP].x = Pn[0].x;
		p_a[NP].y = Pn[0].y;
		NP++;

		// part 86 :
		for (int i = 1; i < C.size(); i++) {
			p_a[NP].x = Pn[i].x;
			p_a[NP].y = Pn[i].y;
			e_a[NF].P[0] = &p_a[NP];
			e_a[NF].P[1] = &p_a[NP - 1];

			NF++;
			NP++;
		}

		// part 87 :
		e_a[NF].P[0] = &p_a[check_P];
		e_a[NF].P[1] = &p_a[NP - 1];
		NF++;

		// part 88 :
		int check_E2 = NF;
		EdgStd.resize(NF + C.size());

		// part 89 :
		for (int i = 0; i < 4; i++)
			if (C[C.size() - 1]->is_in(C[0]->E[i]))
				lE = C[0]->E[i];

		// part 90 :
		e_a[NF].P[0] = &p_a[check_P];
		e_a[NF].P[1] = lE->P[0];
		lE->P[0] = &p_a[check_P];
		NF++;

		// part 91 :
		for (int i = 0; i < C.size() - 1; i++) {
			lE = C[i]->E[C[i]->across(lE)];
			e_a[NF].P[0] = &p_a[check_P + i + 1];
			e_a[NF].P[1] = lE->P[0];
			lE->P[0] = &p_a[check_P + i + 1];
			NF++;
		}

		// part 92 :
		int check_C = NC;
		CellStd.resize(NC + C.size());

		// part 93 :
		for (int i = 0; i < C.size() - 1; i++) {
			c_a[NC].CellAddEdg(C[i]->E[C[i]->across(S.E[i])]);
			C[i]->E[C[i]->across(S.E[i])] = &e_a[check_E1 + i];
			c_a[NC].CellAddEdg(&e_a[check_E1 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i]);
			c_a[NC].CellAddEdg(&e_a[check_E2 + i + 1]);
			NC++;
		}

		// part 94 :
		c_a[NC].CellAddEdg(C[C.size() - 1]->E[C[C.size() - 1]->across(S.E[C.size() - 1])]);
		C[C.size() - 1]->E[C[C.size() - 1]->across(S.E[C.size() - 1])] = &e_a[check_E1 + C.size() - 1];
		c_a[NC].CellAddEdg(&e_a[check_E1 + C.size() - 1]);
		c_a[NC].CellAddEdg(&e_a[check_E2 + C.size() - 1]);
		c_a[NC].CellAddEdg(&e_a[check_E2]);
		NC++;

		// part 95 :
		for (int i = 0; i < 4; i++)
			if (C[C.size() - 1]->is_in(C[0]->E[i]))
				lE = C[0]->E[i];

		// part 96 :
		
		p_a[check_P].PointAddEdg(&e_a[check_E1]);
		p_a[check_P].PointAddEdg(&e_a[check_E1 + C.size() - 1]);
		p_a[check_P].PointAddEdg(&e_a[check_E2]);
		p_a[check_P].PointAddEdg(lE);

		// part 97 :
		
		p_a[check_P].PointAddCell(&c_a[check_C]);
		p_a[check_P].PointAddCell(&c_a[check_C + C.size() - 1]);
		p_a[check_P].PointAddCell(C[0]);
		p_a[check_P].PointAddCell(C[C.size() - 1]);

		// part 98 :
		e_a[check_E2].P[1]->PointAddEdg(&e_a[check_E2]);
		e_a[check_E2].P[1]->PointAddCell(&c_a[check_C]);
		e_a[check_E2].P[1]->PointAddCell(&c_a[check_C + C.size() - 1]);

		// part 99 :
		for (int j = 0; j < e_a[check_E2].P[1]->PointEdgSize; j++)
			if (e_a[check_E2].P[1]->PointEdgs[j] == lE) {
				e_a[check_E2].P[1]->PointRemEdg(j);
			}

		// part 100 :
		for (int j = 0; j < e_a[check_E2].P[1]->PointCellSize; j++)
			if (e_a[check_E2].P[1]->PointCells[j] == C[0] || e_a[check_E2].P[1]->PointCells[j] == C[C.size() - 1]) {
				e_a[check_E2].P[1]->PointRemCell(j--);
			}

		// part 101 :
		for (int i = 1; i < C.size(); i++) {

			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E1 + i - 1]);
			p_a[check_P + i].PointAddEdg(&e_a[check_E2 + i]);


			// part 102 :
			lE = C[i - 1]->E[C[i - 1]->across(lE)];
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointEdgSize; j++)
				if (e_a[check_E2 + i].P[1]->PointEdgs[j] == lE) {
					e_a[check_E2 + i].P[1]->PointRemEdg(j);
				}

			// part 103 :
			for (int j = 0; j < e_a[check_E2 + i].P[1]->PointCellSize; j++)
				if (e_a[check_E2 + i].P[1]->PointCells[j] == C[i] || e_a[check_E2 + i].P[1]->PointCells[j] == C[i - 1]) {
					e_a[check_E2 + i].P[1]->PointRemCell(j--);
				}

			// part 104 :
			e_a[check_E2 + i].P[1]->PointAddEdg(&e_a[check_E2 + i]);
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i]);
			e_a[check_E2 + i].P[1]->PointAddCell(&c_a[check_C + i - 1]);

			// part 105 :
			p_a[check_P + i].PointAddEdg(lE);
			p_a[check_P + i].PointAddCell(&c_a[check_C + i]);
			p_a[check_P + i].PointAddCell(&c_a[check_C + i - 1]);
			p_a[check_P + i].PointAddCell(C[i]);
			p_a[check_P + i].PointAddCell(C[i - 1]);
		}
	}

}


//Definition of dlt Method in MESH Class : this method give specified surface and delete it 
bool MESH::dlt(SURFACE& S) {

	// part 1 :
	vector<CELL*> C1 = S.C[0];
	vector<CELL*> C2 = S.C[1];
	POINT* p_a = PointStd.data();
	EDGE* e_a = EdgStd.data();
	CELL* c_a = CellStd.data();
	int m = 0;
	int n = 0;
	int o = 0;
	int flaq1 = 0;
	int flaq2 = 0;


	// part 2 :
	for (int i = 0; i < 4; i++) {
		if (C1[0]->E[i]->state() == 0) {
			m++;
		}
		if (C1[C1.size() - 1]->E[i]->state() == 0) {
			m++;
		}
	}

	// part 3 :
	for (int i = 0; i < 4; i++) {
		if (C1[0]->E[i]->is_interface == 1) {
			n++;
		}
	}
	for (int i = 0; i < 4; i++) {
		if (C1[C1.size() - 1]->E[i]->is_interface == 1) {
			n++;
		}
	}

	// part 4 :
	
	for (int i = 0; i < 4; i++) {
		if (C1[0]->is_in(C1[C1.size() - 1]->E[i]))
			flaq1 = 1;
		if (C2[0]->is_in(C2[C2.size() - 1]->E[i]))
			flaq2 = 1;
	}

	// part 5 :
	if (flaq1 == 1 && flaq2 == 1)
		o = 1;

	// part 6 :
	if ((n == 2 || n == 4) && o == 0) {
		EDGE *nE[2]{ S.E[0], S.E[C1.size() - 1] };
		if (nE[0]->P[0]->is_on_interface != 1)
			swap(nE[0]->P[0], nE[0]->P[1]);
		if (nE[1]->P[0]->is_on_interface != 1)
			swap(nE[1]->P[0], nE[1]->P[1]);


		// part 7 :
		POINT * P1 = nE[0]->P[0];
		POINT * P2 = nE[1]->P[0];

		// part 8 :
		CELL* CP[4];
		bool r = true;
		int counter = 0;
		for (int i = 0; i<P1->PointEdgSize; i++)
			if (P1->PointEdgs[i]->is_interface == 1) {
				find(P1->PointEdgs[i], cell);
				CP[counter++] = Cell[0];
				CP[counter++] = Cell[1];
			}

		// part 9 :
		for (int i = 0; i < P1->PointCellSize; i++)
			if (P1->PointCells[i] != CP[0] && P1->PointCells[i] != CP[1] && P1->PointCells[i] != CP[2] && P1->PointCells[i] != CP[3])
				for (int k = 0; k < 3; k++)
					if (P1->PointCells[i]->E[k]->P[0] != P1 && P1->PointCells[i]->E[k]->P[1] != P1) {
						r = dlt(P1->PointCells[i]->E[k]);
						if (r == false) {
							cout << "this surface could not be deleted" << endl;
							return false;
						}
						i--;
						break;
					}

		// part 10 :
		counter = 0;
		for (int i = 0; i<P2->PointEdgSize; i++)
			if (P2->PointEdgs[i]->is_interface == 1) {
				find(P2->PointEdgs[i], cell);
				CP[counter++] = Cell[0];
				CP[counter++] = Cell[1];
			}

		// part 11 :
		for (int i = 0; i < P2->PointCellSize; i++)
			if (P2->PointCells[i] != CP[0] && P2->PointCells[i] != CP[1] && P2->PointCells[i] != CP[2] && P2->PointCells[i] != CP[3])
				for (int k = 0; k < 3; k++)
					if (P2->PointCells[i]->E[k]->P[0] != P2 && P2->PointCells[i]->E[k]->P[1] != P2) {
						r = dlt(P2->PointCells[i]->E[k]);
						if (r == false) {
							cout << "this surface could not be deleted" << endl;
							return false;
						}
						i--;
						break;
					}
		// part 12 :
		for (int i = 0; i < C1.size(); i++)
			for (int j = 0; j<4; j++)
				if (C1[i]->E[j] != S.E[i] && C1[i]->E[j]->is_joined(S.E[i]) == 1) {
					if (C1[i]->E[j]->P[1] == S.E[i]->P[0] || C1[i]->E[j]->P[1] == S.E[i]->P[1])
						swap(C1[i]->E[j]->P[1], C1[i]->E[j]->P[0]);
				}

		// part 13 :
		for (int i = 0; i < C2.size(); i++)
			for (int j = 0; j<4; j++)
				if (C2[i]->E[j] != S.E[i] && C2[i]->E[j]->is_joined(S.E[i]) == 1) {
					if (C2[i]->E[j]->P[1] == S.E[i]->P[0] || C2[i]->E[j]->P[1] == S.E[i]->P[1])
						swap(C2[i]->E[j]->P[1], C2[i]->E[j]->P[0]);
				}

		// part 14 :
		EDGE* dE[4];
		for (int i = 0; i < 4; i++) {
			if (C1[0]->E[i]->is_interface == 1 && C1[0]->E[i]->is_joined(S.E[0]))
				dE[0] = C1[0]->E[i];
			if (C1[C1.size() - 1]->E[i]->is_interface == 1 && C1[C1.size() - 1]->E[i]->is_joined(S.E[C1.size() - 1]))
				dE[2] = C1[C1.size() - 1]->E[i];
		}

		// part 15 :
		for (int i = 0; i < 4; i++) {
			if (P1->PointEdgs[i] != S.E[0] && P1->PointEdgs[i]->is_interface != 1)
				dE[1] = P1->PointEdgs[i];
			if (P2->PointEdgs[i] != S.E[C1.size() - 1] && P2->PointEdgs[i]->is_interface != 1)
				dE[3] = P2->PointEdgs[i];
		}

		// part 16 :
		CELL* dC[4];
		find(dE[0], cell);
		if (Cell[0]->CellEdgSize == 3)
			dC[0] = Cell[0];
		else
			dC[0] = Cell[1];

		// part 17 :
		find(dE[2], cell);
		if (Cell[0]->CellEdgSize == 3)
			dC[1] = Cell[0];
		else
			dC[1] = Cell[1];

		// part 18 :
		EDGE* mE[2];
		for (int i = 0; i < 3; i++)
			if (dC[0]->E[i] != dE[0] && dC[0]->E[i] != dE[1])
				mE[0] = dC[0]->E[i];
		for (int i = 0; i < 3; i++)
			if (dC[1]->E[i] != dE[2] && dC[1]->E[i] != dE[3])
				mE[1] = dC[1]->E[i];

		// part 19 :
		find(dE[1], cell);
		if (Cell[0] == dC[0])
			swap(Cell[0], Cell[1]);
		dC[2] = Cell[0];
		for (int i = 0; i < 3; i++)
			if (dC[2]->E[i] == dE[1])
				dC[2]->E[i] = mE[0];

		// part 20 ;
		find(dE[3], cell);
		if (Cell[0] == dC[1])
			swap(Cell[0], Cell[1]);
		dC[3] = Cell[0];
		for (int i = 0; i < 3; i++)
			if (dC[3]->E[i] == dE[3])
				dC[3]->E[i] = mE[1];

		// part 21 :
		EDGE* lE1=nullptr;
		EDGE* lE2=nullptr;
		for (int i = 0; i < C2[0]->CellEdgSize; i++)
			if (C2[0]->E[i]->is_interface == 1 && C2[0]->E[i]->is_joined(S.E[0]))
				lE1 = C2[0]->E[i];
		lE2 = dE[0];

		// part 22 :
		for (int i = 0; i < C1.size(); i++)
			for (int j = 0; j < 4; j++) {
				if (C2[i]->E[j]->is_joined(S.E[i]) == 1 && C2[i]->E[j] != S.E[i])
					for (int k = 0; k < C2[i]->E[j]->P[0]->PointEdgSize; k++) {
						if (C2[i]->E[j]->P[0]->PointEdgs[k] != S.E[i] && C1[i]->is_in(C2[i]->E[j]->P[0]->PointEdgs[k]) == 1) {
							C2[i]->E[j]->P[0] = C2[i]->E[j]->P[0]->PointEdgs[k]->P[1];
							break;
						}
					}

			}

		// part 23 :
		for (int i = 0; i < C1.size(); i++)
			for (int j = 0; j < 4; j++)
				if (C2[i]->E[j] == S.E[i])
					C2[i]->E[j] = C1[i]->E[C1[i]->across(S.E[i])];


		// part 24 :
		for (int i = 0; i < dE[0]->P[1]->PointEdgSize; i++)
			if (dE[0]->P[1]->PointEdgs[i] == dE[0])
				dE[0]->P[1]->PointRemEdg(i);
		for (int i = 0; i < dE[2]->P[1]->PointEdgSize; i++)
			if (dE[2]->P[1]->PointEdgs[i] == dE[2])
				dE[2]->P[1]->PointRemEdg(i);

		// part 25 :
		for (int i = 0; i < dE[0]->P[1]->PointCellSize; i++)
			if (dE[0]->P[1]->PointCells[i] == dC[0])
				dE[0]->P[1]->PointRemCell(i);
		for (int i = 0; i < dE[0]->P[1]->PointCellSize; i++)
			if (dE[0]->P[1]->PointCells[i] == C1[0])
				dE[0]->P[1]->PointRemCell(i);

		// part 26 :
		for (int i = 0; i < dE[2]->P[1]->PointCellSize; i++)
			if (dE[2]->P[1]->PointCells[i] == dC[1])
				dE[2]->P[1]->PointRemCell(i);
		for (int i = 0; i < dE[2]->P[1]->PointCellSize; i++)
			if (dE[2]->P[1]->PointCells[i] == C1[C1.size() - 1])
				dE[2]->P[1]->PointRemCell(i);

		// part 27 :
		POINT* P3;
		POINT* P4;
		if (dE[1]->P[0] != P1)
			P3 = dE[1]->P[0];
		else
			P3 = dE[1]->P[1];


		// part 28 :
		if (dE[3]->P[0] != P2)
			P4 = dE[3]->P[0];
		else
			P4 = dE[3]->P[1];

		// part 29 :
		for (int i = 0; i < P3->PointEdgSize; i++)
			if (P3->PointEdgs[i] == dE[1])
				P3->PointRemEdg(i);
		for (int i = 0; i < P4->PointEdgSize; i++)
			if (P4->PointEdgs[i] == dE[3])
				P4->PointRemEdg(i);

		// part 30 :
		for (int i = 0; i < P3->PointCellSize; i++)
			if (P3->PointCells[i] == dC[0])
				P3->PointRemCell(i);
		for (int i = 0; i < P4->PointCellSize; i++)
			if (P4->PointCells[i] == dC[1])
				P4->PointRemCell(i);


		// part 31 :
		dE[0]->P[1]->PointAddEdg(lE1);
		dE[0]->P[1]->PointAddCell(C2[0]);
		dE[0]->P[1]->PointAddCell(dC[2]);

		// part 32 :
		D.push_back(dE[0]);
		D.push_back(dE[1]);
		D.push_back(dE[2]);
		D.push_back(dE[3]);
		D.push_back(dC[0]);
		D.push_back(dC[1]);
		D.push_back(lE2->P[0]);

		// part 33 :
		for (int i = 0; i < C2.size() - 1; i++) {
			lE1 = C2[i]->E[C2[i]->across(lE1)];
			lE2 = C1[i]->E[C1[i]->across(lE2)];

			D.push_back(lE2);
			D.push_back(lE2->P[0]);
			D.push_back(C1[i]);
			D.push_back(S.E[i]);

			// part 34 :
			lE1->P[0]->PointAddEdg(lE1);
			for (int j = 0; j < lE1->P[0]->PointEdgSize; j++)
				if (lE1->P[0]->PointEdgs[j] == lE2)
					lE1->P[0]->PointRemEdg(j);

			// part 35 :
			for (int j = 0; j < lE1->P[0]->PointCellSize; j++)
				if (lE1->P[0]->PointCells[j] == C1[i] || lE1->P[0]->PointCells[j] == C1[i + 1])
					lE1->P[0]->PointRemCell(j--);

			// part 36 :
			lE1->P[0]->PointAddCell(C2[i]);
			lE1->P[0]->PointAddCell(C2[i + 1]);
		}

		// part 37 :
		lE1 = C2[C2.size() - 1]->E[C2[C2.size() - 1]->across(lE1)];
		lE2 = C1[C2.size() - 1]->E[C1[C2.size() - 1]->across(lE2)];
		lE1->P[0]->PointAddEdg(lE1);
		lE1->P[0]->PointAddCell(C2[C2.size() - 1]);
		lE1->P[0]->PointAddCell(dC[3]);

		// part 38 :
		D.push_back(lE2->P[0]);
		D.push_back(C1[C2.size() - 1]);
		D.push_back(S.E[C2.size() - 1]);
		return true;
	}

	// part 39 :
	if (o == 1) {
		for (int i = 0; i < C1.size(); i++)
			for (int j = 0; j<4; j++)
				if (C1[i]->E[j] != S.E[i] && C1[i]->E[j]->is_joined(S.E[i]) == 1) {
					if (C1[i]->E[j]->P[1] == S.E[i]->P[0] || C1[i]->E[j]->P[1] == S.E[i]->P[1])
						swap(C1[i]->E[j]->P[1], C1[i]->E[j]->P[0]);
				}

		// part 40 :
		for (int i = 0; i < C2.size(); i++)
			for (int j = 0; j<4; j++)
				if (C2[i]->E[j] != S.E[i] && C2[i]->E[j]->is_joined(S.E[i]) == 1) {
					if (C2[i]->E[j]->P[1] == S.E[i]->P[0] || C2[i]->E[j]->P[1] == S.E[i]->P[1])
						swap(C2[i]->E[j]->P[1], C2[i]->E[j]->P[0]);
				}

		// part 41 :
		for (int i = 0; i < C1.size(); i++)
			for (int j = 0; j < 4; j++) {
				if (C2[i]->E[j]->is_joined(S.E[i]) == 1 && C2[i]->E[j] != S.E[i])
					for (int k = 0; k < 4; k++) {
						if (C2[i]->E[j]->P[0]->PointEdgs[k] != S.E[i] && C1[i]->is_in(C2[i]->E[j]->P[0]->PointEdgs[k]) == 1) {
							C2[i]->E[j]->P[0] = C2[i]->E[j]->P[0]->PointEdgs[k]->P[1];
							break;
						}
					}

			}

		// part 42 :
		for (int i = 0; i < C1.size(); i++)
			for (int j = 0; j < 4; j++)
				if (C2[i]->E[j] == S.E[i])
					C2[i]->E[j] = C1[i]->E[C1[i]->across(S.E[i])];


		// part 43 :
		EDGE* lE1=nullptr;
		EDGE* lE2=nullptr;
		for (int i = 0; i < C2[0]->CellEdgSize; i++)
			if (C2[C2.size() - 1]->is_in(C2[0]->E[i]) == 1)
				lE1 = C2[0]->E[i];
		for (int i = 0; i < C1[0]->CellEdgSize; i++)
			if (C1[C1.size() - 1]->is_in(C1[0]->E[i]) == 1)
				lE2 = C1[0]->E[i];

		// part 44 :
		for (int i = 0; i < C2.size() - 1; i++) {

			lE1 = C2[i]->E[C2[i]->across(lE1)];
			lE2 = C1[i]->E[C1[i]->across(lE2)];

			D.push_back(lE2);
			D.push_back(lE2->P[0]);
			D.push_back(C1[i]);
			D.push_back(S.E[i]);

			// part 45 :
			lE1->P[0]->PointAddEdg(lE1);
			for (int j = 0; j < lE1->P[0]->PointEdgSize; j++)
				if (lE1->P[0]->PointEdgs[j] == lE2)
					lE1->P[0]->PointRemEdg(j);

			// part 46 :
			for (int j = 0; j < lE1->P[0]->PointCellSize; j++)
				if (lE1->P[0]->PointCells[j] == C1[i] || lE1->P[0]->PointCells[j] == C1[i + 1])
					lE1->P[0]->PointRemCell(j--);

			// part 47 :
			lE1->P[0]->PointAddCell(C2[i]);
			lE1->P[0]->PointAddCell(C2[i + 1]);

		}

		// part 48 :
		lE1 = C2[C2.size() - 1]->E[C2[C2.size() - 1]->across(lE1)];
		lE2 = C1[C2.size() - 1]->E[C1[C2.size() - 1]->across(lE2)];
		for (int j = 0; j <lE1->P[0]->PointEdgSize; j++)
			if (lE1->P[0]->PointEdgs[j] == lE2)
				lE1->P[0]->PointRemEdg(j);

		// part 49 :
		for (int j = 0; j < lE1->P[0]->PointCellSize; j++)
			if (lE1->P[0]->PointCells[j] == C1[C2.size() - 1] || lE1->P[0]->PointCells[j] == C1[0])
				lE1->P[0]->PointRemCell(j--);

		// part 50 :
		lE1->P[0]->PointAddEdg(lE1);
		lE1->P[0]->PointAddCell(C2[C2.size() - 1]);
		lE1->P[0]->PointAddCell(C2[0]);

		// part 51 :
		D.push_back(lE2);
		D.push_back(lE2->P[0]);
		D.push_back(C1[C2.size() - 1]);
		D.push_back(S.E[C2.size() - 1]);


		return true;

	}
	return false;
}


// Definition of deletedata Method in MESH Class : delete POINT data
void MESH::deletedata(POINT* address) {
	// part 1 :
	POINT* p_a = PointStd.data();
	if (A2N(address) != NP) {

		EDGE** e = p_a[NP - 1].PointEdgs;
		for (int i = 0; i < p_a[NP - 1].PointEdgSize; i++) {
			if ((e[i])->P[0] == &p_a[NP - 1])
				(e[i])->P[0] = address;
			if (e[i]->P[1] == &p_a[NP - 1])
				e[i]->P[1] = address;
		}
		
		// part 2 :
		*address = p_a[NP - 1];
		
		// part 3 :
		PointStd.pop_back();
		p_a[NP - 1].PointIsDel = 1;
		NP--;
	}
	// part 4 :
	else {

		PointStd.pop_back();
		p_a[NP - 1].PointIsDel = 1;
		NP--;
	}



}

// Definition of deletedata Method in MESH Class : delete CELL data
void MESH::deletedata(CELL* address) {
	// part 1 :
	CELL* c_a = CellStd.data();
	if (A2N(address) != NC) {

		int m = find(&c_a[NC - 1], point);
		for (int i = 0; i < m; i++)
			for (int j = 0; j < Point[i]->PointCellSize; j++)
				if (Point[i]->PointCells[j] == &c_a[NC - 1])
					Point[i]->PointCells[j] = address;

		// part 2:
		*address = c_a[NC - 1];

		// part 3 :
		CellStd.pop_back();
		c_a[NC - 1].CellIsDel = 1;
		NC--;
	}

	// part 4 :
	else {

		CellStd.pop_back();
		c_a[NC - 1].CellIsDel = 1;
		NC--;
	}
}

// Definition of deletedata Method in MESH Class : delete EDGE data
void MESH::deletedata(EDGE* address) {

	// part 1 :
	POINT* p_a = PointStd.data();
	EDGE* e_a = EdgStd.data();
	if (A2N(address) != NF) {

		int m = A2N(e_a[NF - 1].P[0]) - 1;
		int n = A2N(e_a[NF - 1].P[1]) - 1;
		EDGE** e = p_a[m].PointEdgs;

		// part 2 :
		for (int i = 0; i < p_a[m].PointEdgSize; i++) {
			if (e[i] == &e_a[NF - 1])
				e[i] = address;
		}

		// part 3 :
		e = p_a[n].PointEdgs;
		for (int i = 0; i < p_a[n].PointEdgSize; i++) {
			if (e[i] == &EdgStd[NF - 1])
				e[i] = address;
		}

		// part 4 :
		int a = find(&e_a[NF - 1], cell);
		for (int i = 0; i < a; i++)
			for (int j = 0; j < Cell[i]->CellEdgSize; j++)
				if (Cell[i]->E[j] == &e_a[NF - 1])
					Cell[i]->E[j] = address;

		// part 5 :
		*address = e_a[NF - 1];

		// part 6 :
		EdgStd.pop_back();
		e_a[NF - 1].EdgIsDel = 1;
		NF--;
	}

	// part 7:
	else {

		EdgStd.pop_back();
		e_a[NF - 1].EdgIsDel = 1;
		NF--;
	}
}


// Definition of writemesh Method in MESH Class: write all data of the mesh in .txt file
void MESH::writemesh(const char* name) {
	ofstream file;
	file.open(name);
	if (file.is_open() == 1) {
		file << NP << "\tnumber of points" << endl;
		file << NC << "\tnumber of cells" << endl;
		file << NF << "\tnumberof faces" << endl;
		file << ZNum << "\t number of zones" << endl;
		file << endl << "cell data" << endl;

		for (int i = 0; i < NC; i++)
			file << i + 1 << "\t" << A2N(CellStd[i].E[0]) << "\t" << A2N(CellStd[i].E[1]) << "\t" << A2N(CellStd[i].E[2]) << "\t" << A2N(CellStd[i].E[3]) << "\t" << CellStd[i].CellZnum << endl;
		file << endl << "edge data" << endl;
		for (int i = 0; i < NF; i++)
			file << i + 1 << "\t" << A2N(EdgStd[i].P[0]) << "\t" << A2N(EdgStd[i].P[1]) << "\t" << EdgStd[i].region << "\t" << EdgStd[i].is_interface << endl;
		file << endl << "point data" << endl;
		for (int i = 0; i < NP; i++) {
			file << i + 1 << "\t";
			for (int j = 0; j < PointStd[i].PointEdgSize; j++)
				file << A2N(PointStd[i].PointEdgs[j]) << "\t";
			file << "||\t";
			for (int j = 0; j < PointStd[i].PointCellSize; j++)
				file << A2N(PointStd[i].PointCells[j]) << "\t";
			file << "||" << "\t" << PointStd[i].is_on_interface << endl;

		}
		file << "point coordinate" << endl;
		for (int i = 0; i < NP; i++)
			file << i + 1 << "\t" << PointStd[i].x << "\t" << PointStd[i].y << "\t" << PointStd[i].is_boundary << endl;


	}
}


void MESH::writeplt(const char* file_name) {
	ofstream file;
	file.open(file_name);
	if (file.is_open() == 1) {
		file << "TITLE=" << "'title'" << endl;
		file << "Variables=" << "'X'" << "," << "'Y'" << endl;
		file << "Zone" << " " << "T='title'," << "N=" << " " << NP << ",E=" << " " << NF << ",ET=LINESEG" << ",F=FEBLOCK" << endl;
		for (int i = 0; i < NP; i++)
			file << PointStd[i].x << endl;
		for (int i = 0; i < NP; i++)
			file << PointStd[i].y << endl;
		for (int i = 0; i < NF; i++)
			file << A2N(EdgStd[i].P[0]) << " " << A2N(EdgStd[i].P[1]) << endl;

	}
}

void MESH::delauny() {
	for (int i = 0; i < NF; i++) {
		do_delauney(&EdgStd[i]);
	}
}

bool MESH::extend(EDGE* Ep, SURFACE& S) {

	// part 1 :
	find(Ep, MESH::cell);
	static int m = 0;
	static int n = 0;
	CELL* C[2] = { Cell[0], Cell[1] };

	// part 2 :
	if (Ep->state() == 1  && Ep->EdgIsLabl == 0 && C[0]->CellEdgSize == 4) {
		Ep->EdgIsLabl = 1;
		S.E.push_back(Ep);
		EDGE E = *Ep;
		EDGE** e;
		S.C[0].push_back(C[0]);
		S.C[1].push_back(C[1]);

		// part 3 :
		EDGE* Ec[2] = { nullptr };
		for (int j = 0; j < 2; j++)
			for (int i = 0; i < 4; i++)
				if ((C[j]->E[i]->P[0] == E.P[0] || C[j]->E[i]->P[1] == E.P[0]) && C[j]->E[i] != Ep) {
					Ec[j] = C[j]->E[i];
					break;
				}

		// part 4 :
		e = E.P[0]->PointEdgs;
		for (int i = 0; i < 4; i++)
			if (e[i] != Ep && e[i] != Ec[0] && e[i] != Ec[1] && E.P[0]->PointEdgSize == 4) {
				if (E.P[0]->PointEdgs[i]->P[0] == E.P[0])
					swap(E.P[0]->PointEdgs[i]->P[0], E.P[0]->PointEdgs[i]->P[1]);

				extend(E.P[0]->PointEdgs[i], S);
				break;

			}

		// part 5 :
		if (m == 0 && n == 0) {
			m++;
			swap(S.E[0]->P[0], S.E[0]->P[1]);
			S.E[0]->EdgIsLabl = 0;
			extend(S.E[0], S);
		}

		// part 6 :
		if (Ep == S.E[0] && n == 1) {
			n = 0;
			m = 0;
			S.order();
		}
		else if (Ep == S.E[0] && n == 0) {
			n++;
			m = 0;
		}
		return true;

	}
	else
		return false;

}

bool MESH::extend_one_layer(EDGE* Ep, SURFACE& S,ZONE& Z) {

	// part 1 :
	bool is_circular = 1;
	bool is_cone = 1;
	EDGE* Enew;

	// part 2 :
	if (Ep->is_interface == 1) {
		for (int i = 0; i < Z.CurveInterface.size(); i++)
			if (Z.CurveInterface[i]->P[0]->is_boundary == 1 || Z.CurveInterface[i]->P[1]->is_boundary == 1) {
				is_circular = 0;
				break;
			}

		// part 3 :
		if (is_circular == 1) {
			for (int j = 0; j < Ep->P[0]->PointCellSize; j++)
				for (int k = 0; k < Ep->P[1]->PointCellSize; k++)
					if (Ep->P[0]->PointCells[j] == Ep->P[1]->PointCells[k] && Ep->P[0]->PointCells[j]->CellEdgSize == 4) {
						S.E.push_back(Ep);
						S.C[0].push_back(Ep->P[0]->PointCells[j]);
						S.C[1].push_back(Ep->P[0]->PointCells[j]);
						break;
					}

			// part 4 :
			for (int i = 0; i < 4; i++)
				if (S.C[0][0]->E[i]->is_interface == 1)
					is_cone = !is_cone;

			// part 5 :
			if (is_cone == 1) {
				S.E.clear();
				S.C[0].clear();
				S.C[1].clear();
				return false;
			}

			// part 6 :
			Enew = Ep;
			while (is_cone == 0) {
				is_cone = 1;
				for (int i = 0; i < Enew->P[0]->PointEdgSize; i++)
					if (Enew->P[0]->PointEdgs[i]->is_interface == 1 && Enew->P[0]->PointEdgs[i] != Enew) {
						S.E.push_back(Enew->P[0]->PointEdgs[i]);
						break;
					}

				// part 7 :
				Enew = S.E[S.E.size() - 1];
				for (int j = 0; j < Enew->P[0]->PointCellSize; j++)
					for (int k = 0; k < Enew->P[1]->PointCellSize; k++)
						if (Enew->P[0]->PointCells[j] == Enew->P[1]->PointCells[k] && Enew->P[0]->PointCells[j]->CellEdgSize == 4) {
							S.C[0].push_back(Enew->P[0]->PointCells[j]);
							S.C[1].push_back(Enew->P[0]->PointCells[j]);
						}

				// part 8 :
				for (int i = 0; i < 4; i++)
					if (S.C[0][S.E.size() - 1]->E[i]->is_interface == 1)
						is_cone = !is_cone;


				// part 9
				if (Enew == S.E[0]) {
					S.order();
					return true;
				}

				// part 10 :
				if (Enew->P[0] == S.E[S.E.size() - 2]->P[0])
					swap(Enew->P[0], Enew->P[1]);

			}

			// part 11 :
			if (is_cone == 1) {
				is_cone = 0;
				Enew = S.E[0];
				S.E.push_back(S.E[0]);
				S.C[0].push_back(S.C[0][0]);
				S.C[1].push_back(S.C[0][0]);
				swap(Enew->P[0], Enew->P[1]);
			}

			// part 12 :
			while (is_cone == 0) {
				is_cone = 1;
				for (int i = 0; i < Enew->P[0]->PointEdgSize; i++)
					if (Enew->P[0]->PointEdgs[i]->is_interface == 1 && Enew->P[0]->PointEdgs[i] != Enew) {
						S.E.push_back(Enew->P[0]->PointEdgs[i]);
						break;
					}

				// part 13 :
				Enew = S.E[S.E.size() - 1];
				for (int j = 0; j < Enew->P[0]->PointCellSize; j++)
					for (int k = 0; k < Enew->P[1]->PointCellSize; k++)
						if (Enew->P[0]->PointCells[j] == Enew->P[1]->PointCells[k] && Enew->P[0]->PointCells[j]->CellEdgSize == 4) {
							S.C[0].push_back(Enew->P[0]->PointCells[j]);
							S.C[1].push_back(Enew->P[0]->PointCells[j]);
						}

				// part 14 :
				for (int i = 0; i < 4; i++)
					if (S.C[0][S.E.size() - 1]->E[i]->is_interface == 1)
						is_cone = !is_cone;

				// part 15 :
				if (Enew->P[0] == S.E[S.E.size() - 2]->P[0])
					swap(Enew->P[0], Enew->P[1]);

			}

			// part 16 :
			S.order();
			return true;

		}
	}
	else
		return false;
}
