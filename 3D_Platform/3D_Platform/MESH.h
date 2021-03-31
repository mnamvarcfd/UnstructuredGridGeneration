#pragma once
#include "CELL.h"
#include "ZONE.h"
#include "EDGE.h"
#include "uni.h"
#include <typeinfo>
#include <set>


extern POINT* Point[100];
extern EDGE* Edge[100];
extern FACE* Face[100];
extern CELL* Cell[100];

class MESH {
public:
	// Define Vector
	vector<POINT> PointStd;
	vector<CELL> CellStd;
	vector<FACE> FaceStd;
	enum TYPE { cell, face, edge, point };

	// Preprocessing Method
	void readmesh(const char*);
	void analyse();
	void fill_zone(vector<ZONE>&);
	void Cell_order();
	bool Cell_order(vector<CELL*>&);
	// Searching Method

	// part 1 :
	template<typename T>
	int A2N(T address) {

		int n = -1;
		unsigned long long a = reinterpret_cast<unsigned  long long>(address);

		static	unsigned long long e = reinterpret_cast<unsigned  long long>(PointStd.data());
		static	unsigned long long f = reinterpret_cast<unsigned  long long>(FaceStd.data());
		static	unsigned long long g = reinterpret_cast<unsigned  long long>(CellStd.data());
		static int t = 0;
		static unsigned long long i[3] = { e,f,g };
		static size_t s[3] = { sizeof(POINT),sizeof(FACE),sizeof(CELL) };

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
		if (a >= i[0]) {
			n = (a - i[0]) / s[0] + 1;
			return n;
		}

		// part 4 :
		if (a >= i[1]) {
			n = (a - i[1]) / s[1] + 1;
			return n;
		}

		 // part 5 :
		if (a >= i[2]) {
			n = (a - i[2]) / s[2] + 1;
			return n;
		}
		throw;
		return n;
	}


	
	template<typename T>
	int find(T address, TYPE tname) {

		// part 1 :
		static CELL* c_a = CellStd.data();
		static FACE* f_a = FaceStd.data();
		static POINT* p_a = PointStd.data();

		// part 2 :
		A add;
		add.f = reinterpret_cast<FACE*>(address);
		size_t s[3] = { add.c - c_a,add.f - f_a,add.p - p_a };

		{
			// part 3 :
			if (s[0] >= 0 && s[0] <= NC && tname == point) {
				int P_counter = 0;
				FACE* f;


				{
					// part 4 :
					if (add.c->CType == CELL::tetra) {

						f = add.c->F[0];
						memcpy(Point, f->P, 3 * sizeof(POINT*));
						f = add.c->F[1];
						if (f->P[0] != Point[0] && f->P[0] != Point[1] && f->P[0] != Point[2]) {
							Point[3] = f->P[0];
							return 4;
						}
						if (f->P[1] != Point[0] && f->P[1] != Point[1] && f->P[1] != Point[2]) {
							Point[3] = f->P[1];
							return 4;
						}
						if (f->P[2] != Point[0] && f->P[2] != Point[1] && f->P[2] != Point[2]) {
							Point[3] = f->P[2];
							return 4;
						}
						throw;
					}

					// part 5 :
					if (add.c->CType == CELL::cube) {
						f = add.c->F[0];
						memcpy(Point, f->P, 4 * sizeof(POINT*));
						f = add.c->F[1];
						memcpy(&Point[4], f->P, 4 * sizeof(POINT*));
						return 8;
					}

					// part 6 :
					if (add.c->CType == CELL::prism) {
						f = add.c->F[0];
						memcpy(Point, f->P, 3 * sizeof(POINT*));
						f = add.c->F[1];
						memcpy(&Point[3], f->P, 3 * sizeof(POINT*));
						return 6;
					}

					// part 7 :
					if (add.c->CType == CELL::pyramid) {
						f = add.c->F[0];
						memcpy(Point, f->P, 4 * sizeof(POINT*));
						f = add.c->F[1];
						if (f->P[0] != Point[0] && f->P[0] != Point[1] && f->P[0] != Point[2] && f->P[0] != Point[3]) {
							Point[4] = f->P[0];
							return 5;
						}
						if (f->P[1] != Point[0] && f->P[1] != Point[1] && f->P[1] != Point[2] && f->P[1] != Point[3]) {
							Point[4] = f->P[1];
							return 5;
						}
						if (f->P[2] != Point[0] && f->P[2] != Point[1] && f->P[2] != Point[2] && f->P[2] != Point[3]) {
							Point[4] = f->P[2];
							return 5;
						}
						throw;
					}

				}
			}
		}

		{
			// part 8 :
			if (s[1] >= 0 && s[1] <= NF && tname == cell) {

				FACE* F_add = add.f;
				FACE** f;
				int m = 0;

				for (int i = 0; i < add.f->P[0]->PointCellSize; i++) {
					f = (add.f->P[0]->PointCells[i])->F;
					for (int j = 0; j<6; j++)
						if (*(f + j) == F_add) {
							Cell[m++] = add.f->P[0]->PointCells[i];
							break;
						}
				}
				if (m > 2 || m == 0)
					throw;
				return m;
			}
		}
		{
			// part 9 :
			if (s[0] >= 0 && s[0] <= NC && tname == cell) {
				CELL* c = add.c;
				FACE** f = add.c->F;
				int m;
				int C_counter = 0;
				CELL* C[6];
				for (int i = 0; i < add.c->CellFaceSize; i++) {
					m = find(*(f + i), cell);
					if (m == 2) {
						if (Cell[0] != c) {
							C[C_counter++] = Cell[0];
							continue;
						}
						else {
							C[C_counter++] = Cell[1];
							continue;
						}
					}
				}
				for (int i = 0; i < C_counter; i++)
					Cell[i] = C[i];
				return C_counter;

			}
		}
		{

			// part 10 :
			if (s[2] >= 0 && s[2] <= NP && tname == face) {
				int n = A2N(address) - 1;
				POINT* p = &p_a[n];
				CELL** c = p_a[n].PointCells;
				int C_size = p_a[n].PointCellSize;
				int F_counter = 0;
				for (int i = 0; i < (*c)->CellFaceSize; i++)
					if ((*c)->F[i]->P[0] == p || (*c)->F[i]->P[1] == p || (*c)->F[i]->P[2] == p || (*c)->F[i]->P[3] == p)
						Face[F_counter++] = (*c)->F[i];

				for (int j = 1; j < C_size; j++)
					for (int i = 0; i < (*(c + j))->CellFaceSize; i++)
						if ((*(c + j))->F[i]->P[0] == p || (*(c + j))->F[i]->P[1] == p || (*(c + j))->F[i]->P[2] == p || (*(c + j))->F[i]->P[3] == p) {
							if (check_in(Face, (*(c + j))->F[i], F_counter) == 0)
								Face[F_counter++] = (*(c + j))->F[i];
						}
				return F_counter;

			}
		}
		{

			// part 11 :
			if (typeid(address) == typeid(Edge[0]) && tname == cell) {
				POINT** P1 = reinterpret_cast<POINT**>(address);
				POINT* p1 = *P1;
				POINT* p2 = *(P1 + 1);
				CELL** c1 = p1->PointCells;
				CELL** c2 = p2->PointCells;
				int m = p1->PointCellSize;
				int n = p2->PointCellSize;
				int counter = 0;

				int P_n;
				if (m <= n) {
					for (int i = 0; i < m; i++) {
						P_n = find(*(c1 + i), point);
						for (int j = 0; j<P_n; j++)
							if (Point[j] == p2) {
								Cell[counter++] = *(c1 + i);
								break;
							}

					}

				}
				else {
					for (int i = 0; i < n; i++) {
						P_n = find(*(c2 + i), point);
						for (int j = 0; j<P_n; j++)
							if (Point[j] == p1) {
								Cell[counter++] = *(c2 + i);
								break;
							}

					}

				}
				return counter;
			}
		}
		return -1;
	}




	template<typename T>
	bool check_in(T* address, T object, int max_num) {
		{
			for (int i = 0; i < max_num && address[i] != nullptr; i++)
				if (address[i] == object) {
					return true;
				}
			return false;
		}
	}

	bPOINT mid(POINT*, POINT*);
	int delcheck(FACE*);
	int delauny(FACE*);
	void laplacian();
	void flip14(CELL*);
	void flip14(CELL*, bPOINT);
	void flip12(EDGE*);
	void flip12(EDGE*, bPOINT);
	bool flip23(FACE*);
	bool flip32(EDGE*);
	int check22(EDGE*);
	void flip26(FACE*);
	void flip26(FACE*, bPOINT);
	void flip13(FACE*);
	void flip13(FACE*, bPOINT);
	bool flip44(EDGE*);
	bool flip22(EDGE*);
	int check44(EDGE*);
	int check23(FACE*);
	int check32(EDGE*);
	bool extend(CELL*, vector<CELL*>&);
	bool add(vector<CELL*>&);
	bool dlt(POINT*);
	double volume_(POINT**);
	void clear();

	void writeplt(const char*);


	// Mesh Parameters
	int ZNum = 1;
	int n = 0; //number of delauny
	int mesh_dim;
	int NP; //number of points
	int NE; //number of edges
	int NC; //number of cells
	int NF; //number of faces
	int NR; //number of regions
private:
	CELL * C_stack[10000000];
	FACE * F_stack[10000000];

	double det(double A[][3]);

	// Operator of data deleting
	void deletedata(POINT*);
	void deletedata(CELL*);
	void deletedata(FACE*);
};


extern MESH mesh;
extern vector<void*> D;