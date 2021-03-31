#pragma once
#include "ZONE.h"
#include "Perm_Mesh.h"

union A {
	POINT* p;
	EDGE* e;
	CELL* c;
};


extern CELL* Cell[100];
extern EDGE* Edge[100];
extern POINT* Point[100];
extern Perm_Mesh permmesh;

// Definition of MESH Class
class MESH {
public:
	// Define Vector
	vector<POINT> PointStd;
	vector<CELL> CellStd;
	vector<EDGE> EdgStd;
	enum TYPE { cell, edge, point };

	// Searching Method
	//Definition of A2N Method in MESH Class: Convert Address of each data type to it's number in Global Vector
	template<typename T>
	
	int A2N(T address) {
		// part 1 :
		int n = -1;
		unsigned long long addressdata = reinterpret_cast<unsigned  long long>(address);

		static	unsigned long long pointdata = reinterpret_cast<unsigned  long long>(PointStd.data());
		static	unsigned long long edgedata = reinterpret_cast<unsigned  long long>(EdgStd.data());
		static	unsigned long long celldata = reinterpret_cast<unsigned  long long>(CellStd.data());
		static int t = 0;

		static unsigned long long i[3] = { pointdata,edgedata,celldata };
		static size_t s[3] = { sizeof(POINT),sizeof(EDGE),sizeof(CELL) };

		// part 2 :
		if (t == 0) {
			if (i[0] < i[1]) {
				swap(i[0], i[1]);
				swap(s[0], s[1]);
			}
			if (i[0] < i[2]) {
				swap(i[0], i[2]);
				swap(s[0], s[2]);
			}
			if (i[1] < i[2]) {
				swap(i[1], i[2]);
				swap(s[1], s[2]);
			}
			t = 1;
		}

		// part 3 :
		if (addressdata >= i[0]) {
			n = (addressdata - i[0]) / s[0] + 1;
			return n;
		}

		// part 4 :
		if (addressdata >= i[1]) {
			n = (addressdata - i[1]) / s[1] + 1;
			return n;
		}

		// part 5 :
		if (addressdata >= i[2]) {
			n = (addressdata - i[2]) / s[2] + 1;
			return n;
		}
		throw;
		return n;
	}


	template<typename T>
	int find(T address, TYPE tname) {

		// part 1 :
		static CELL* c_a = CellStd.data();
		static EDGE* e_a = EdgStd.data();
		static POINT* p_a = PointStd.data();

		// part 2 :
		A add;
		add.e = reinterpret_cast<EDGE*>(address);
		size_t s[3] = { add.c - c_a,add.e - e_a,add.p - p_a };

		{
			// part 3 :
			if (s[0] >= 0 && s[0] <= NC && tname == point) {

				CELL C = *(add.c);
				int m = 0;    //point counter
				POINT* P[8] = { nullptr };
				int n = C.CellEdgSize;

				// part 4 :
				for (int j = 0; j < n - 1; j++) {
					P[2 * j] = C.E[j]->P[0];
					P[2 * j + 1] = C.E[j]->P[1];
				}

				// part 5 :
				Point[m++] = P[0];
				Point[m++] = P[1];

				// part 6 :
				for (int j = 2; j < 2 * n; j++)
					if (P[j] != P[1] && P[j] != P[0]) {
						Point[m++] = P[j];
						break;
					}

				// part 7 :
				if (m == n)
					return m;

				// part 8 :
				else {
					for (int j = 3; j < 2 * n; j++)
						if (P[j] != P[1] && P[j] != P[0] && P[j] != Point[2]) {
							Point[m++] = P[j];
							break;
						}

					// part 9 :
					if (m == n)
						return m;
					else {
						throw;
					}
				}
			}
		}
		{
			// part 10 :
			if (s[1] >= 0 && s[1] <= NF && tname == cell) {
				int n = 0;
				EDGE E = *(add.e);
				EDGE* E_add = add.e;
				CELL** c = E.P[0]->PointCells;
				EDGE** e;

				// part 11 :
				for (int j = 0; (j < E.P[0]->PointCellSize && n != 2); j++) {
					e = c[j]->E;
					for (int k = 0; k < 4; k++)
						if (e[k] == E_add) {
							Cell[n++] = c[j];
							break;
						}
				}

				return n;

			}
		}
		{
			// part 12 :
			if (s[0] >= 0 && s[0] <= NC && tname == cell) {
				CELL C = *(add.c);
				CELL* c[8];
				CELL* C_add = add.c;
				int m = 0;
				int n = 0;

				// part 13 :
				for (int j = 0; j < C.CellEdgSize; j++) {
					if (2 == find(C.E[j], cell)) {
						c[m++] = Cell[0];
						c[m++] = Cell[1];
					}
				}

				// part 14 :
				for (int j = 0; j < m; j++)
					if (c[j] != C_add)
						Cell[n++] = c[j];
				return n;
			}
		}
	}
	EDGE* find(POINT*, POINT*);

	// Preprocessing Method
	void readmesh(const char*);
	void readmeshGID(const char*);
	void analyse();
	void fill_zone(vector<ZONE>&);

	


	// Operators of Simple Cell
	int delcheck(EDGE*);
	void delauny();
	void do_delauney(EDGE*);
	void flip22(EDGE*);
	void flip13(CELL*);
	void flip13(CELL*, bPOINT);
	void flip24(EDGE*);
	void flip24(EDGE*, bPOINT);
	void laplacian();
	bool EdgContract(EDGE*);


	// Operator of Hybrid Mesh
	bool extend(EDGE*, SURFACE&);
	bool extend_one_layer(EDGE*, SURFACE&,ZONE&);
	void add(SURFACE&, int);
	bool dlt(SURFACE&);
	void clear();




	// Postprocessing Method
	void writemesh(const char*);
	void writeplt(const char*);


	// Mesh Parameters
	int ZNum = 1;
	int n = 0; //number of delauny
	int mesh_dim;
	int NP; //number of points
	int NC; //number of cells
	int NF; //number of faces
	int NR; //number of regions
private:
	CELL * * C_stack = new CELL*[100000];
	void I_add(EDGE*);
	bool dlt(EDGE*);
	// Operator of data deleting
	void deletedata(POINT*);
	void deletedata(CELL*);
	void deletedata(EDGE*);
};


extern MESH mesh;
extern vector<void*> D;