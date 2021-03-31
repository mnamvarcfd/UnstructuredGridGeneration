#include "stdafx.h"
#include "MESH.h"
#include <fstream>
#include <iostream>

using namespace std;


void MESH::readmesh(const char* file_name) {

	// part 1 :
	int p1;
	int p2;
	int p3;
	int p4;
	int me;
	int ne;
	int Reg_data[100];
	cout << file_name << endl;

	// part 2 :
	ifstream file;
	file.open(file_name);
	if (file.is_open() == 1) {
		file >> mesh_dim >> NP >> NC >> NF >> NR;
		cout << "mesh dimention :" << mesh_dim << "\nnumber of points :" << NP << "\nnumber of cells :" << NC << "\nnumber of faces :" << NF << "\nnumber of regions :" << NR << endl;
	}

	// part 3 :
	PointStd.reserve(NP * SIZE);
	CellStd.reserve(NC * SIZE);
	FaceStd.reserve(NF * SIZE);

	PointStd.resize(NP);
	CellStd.resize(NC);
	FaceStd.resize(NF);


	// part 4 :
	for (int i = 0; i < 2 * NR; i++)
		file >> Reg_data[i];

	// part 5 :
	int f = 0;
	for (int j = 0; j < NR; j++) {
		for (int i = 0; i < Reg_data[2 * j]; i++) {

			file >> FaceStd[f].FType;
			if (FaceStd[f].FType == 3)
				file >> me >> ne >> p1 >> p2 >> p3;
			if (FaceStd[f].FType == 4)
				file >> me >> ne >> p1 >> p2 >> p3 >> p4;
			CellStd[me - 1].CellAddFace(&FaceStd[f]);
			if (ne != 0) {
				CellStd[ne - 1].CellAddFace(&FaceStd[f]);
				FaceStd[f].region = Reg_data[2 * j + 1];
			}
			else {
				FaceStd[f].IsOnBound = true;
				FaceStd[f].region = Reg_data[2 * j + 1];
				PointStd[p1 - 1].IsOnBound = 1;
				PointStd[p1 - 1].reg_vec.push_back(Reg_data[2 * j + 1]);
				PointStd[p2 - 1].IsOnBound = 1;
				PointStd[p2 - 1].reg_vec.push_back(Reg_data[2 * j + 1]);
				PointStd[p3 - 1].IsOnBound = 1;
				PointStd[p3 - 1].reg_vec.push_back(Reg_data[2 * j + 1]);
				if (FaceStd[f].FType == 4) {
					PointStd[p4 - 1].IsOnBound = 1;
					PointStd[p4 - 1].reg_vec.push_back(Reg_data[2 * j + 1]);
				}
			}
			FaceStd[f].FaceAddPoint(&PointStd[p1 - 1]);
			FaceStd[f].FaceAddPoint(&PointStd[p2 - 1]);
			FaceStd[f].FaceAddPoint(&PointStd[p3 - 1]);
			




			if (FaceStd[f].FType == 4) {
				FaceStd[f].FaceAddPoint(&PointStd[p4 - 1]);

			}
			f++;
			if (f > NF)
				throw;
		}
	}


	// part 6 :
	for (int i = 0; i < NP; i++) {
		file >> PointStd[i].X[0] >> PointStd[i].X[1] >> PointStd[i].X[2];
		//cout << PointStd[i].X[0] << " " << PointStd[i].X[1] << " " << PointStd[i].X[2] << endl;
	}


	// part 7 :
	int F_counter = 0;
	for (int i = 0; i < NC; i++) {
		F_counter = 0;
		if (CellStd[i].CellFaceSize == 6)
			CellStd[i].CType = CELL::cube;
		if (CellStd[i].CellFaceSize == 5) {
			for (int j = 0; j < 5; j++)
				if (CellStd[i].F[j]->FacePointSize == 4)
					F_counter++;
			if (F_counter == 1)
				CellStd[i].CType = CELL::pyramid;
			else if (F_counter == 3)
				CellStd[i].CType = CELL::prism;
			else
				throw;

		}


		// part 8 :
		if (CellStd[i].CType == CELL::pyramid) {
			for (int j = 0; j<5; j++)
				if (CellStd[i].F[j]->FacePointSize == 4) {
					swap(CellStd[i].F[j], CellStd[i].F[0]);
					break;
				}
		}

		// part 9 :
		if (CellStd[i].CType == CELL::prism) {
			for (int j = 0; j<5; j++)
				if (CellStd[i].F[j]->FacePointSize == 3) {
					swap(CellStd[i].F[j], CellStd[i].F[0]);
					break;
				}
			for (int j = 1; j<5; j++)
				if (CellStd[i].F[j]->FacePointSize == 3) {
					swap(CellStd[i].F[j], CellStd[i].F[1]);
					break;
				}
		}

		// part 10 :
		if (CellStd[i].CType == CELL::cube) {
			for (int j = 1; j < 6; j++)
				if (CellStd[i].F[0]->is_joined(CellStd[i].F[j]) == 0) {
					swap(CellStd[i].F[j], CellStd[i].F[1]);
					break;
				}
			for (int j = 3; j < 6; j++)
				if (CellStd[i].F[2]->is_joined(CellStd[i].F[j]) == 0) {
					swap(CellStd[i].F[j], CellStd[i].F[3]);
					break;
				}
		}

		// part 11 :
		CellStd[i].AddCellToPoints();
	}
}

int MESH::delcheck(FACE* F) {

	// part 1 :
	FACE f = *F;
	int n = find(F, cell);
	double d;

	// part 2 :
	double V[2];
	double Coef;
	V[0] = Cell[0]->volume();
	V[1] = Cell[1]->volume();

	if (V[0] != 0 && V[1] != 0)
		Coef = 1 / (V[0] + V[1]);
	else
		throw;


	// part 3 :
	if (n == 2 && Cell[0]->CType == CELL::tetra && Cell[1]->CType == CELL::tetra) {
		double P[3][5];
		double A[3][3];
		double Ai[3][3];
		double B[3];
		double xc, yc, zc, r2c;

		// part 4 :
		P[0][0] = f.P[0]->X[0];
		P[1][0] = f.P[0]->X[1];
		P[2][0] = f.P[0]->X[2];
		P[0][1] = f.P[1]->X[0];
		P[1][1] = f.P[1]->X[1];
		P[2][1] = f.P[1]->X[2];
		P[0][2] = f.P[2]->X[0];
		P[1][2] = f.P[2]->X[1];
		P[2][2] = f.P[2]->X[2];

		// part 5 :
		find(Cell[0], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != f.P[0] && Point[i] != f.P[1] && Point[i] != f.P[2]) {
				P[0][3] = Point[i]->X[0];
				P[1][3] = Point[i]->X[1];
				P[2][3] = Point[i]->X[2];
				break;
			}

		// part 6 :
		find(Cell[1], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != f.P[0] && Point[i] != f.P[1] && Point[i] != f.P[2]) {
				P[0][4] = Point[i]->X[0];
				P[1][4] = Point[i]->X[1];
				P[2][4] = Point[i]->X[2];
				break;
			}

       // part 7 :   
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 3; j++)
				P[j][i] = P[j][i] * Coef;

		// part 8 :
		A[0][0] = 2 * (P[0][3] - P[0][0]);
		A[0][1] = 2 * (P[1][3] - P[1][0]);
		A[0][2] = 2 * (P[2][3] - P[2][0]);
		A[1][0] = 2 * (P[0][3] - P[0][1]);
		A[1][1] = 2 * (P[1][3] - P[1][1]);
		A[1][2] = 2 * (P[2][3] - P[2][1]);
		A[2][0] = 2 * (P[0][3] - P[0][2]);
		A[2][1] = 2 * (P[1][3] - P[1][2]);
		A[2][2] = 2 * (P[2][3] - P[2][2]);
		B[0] = P[0][3] * P[0][3] - P[0][0] * P[0][0] + P[1][3] * P[1][3] - P[1][0] * P[1][0] + P[2][3] * P[2][3] - P[2][0] * P[2][0];
		B[1] = P[0][3] * P[0][3] - P[0][1] * P[0][1] + P[1][3] * P[1][3] - P[1][1] * P[1][1] + P[2][3] * P[2][3] - P[2][1] * P[2][1];
		B[2] = P[0][3] * P[0][3] - P[0][2] * P[0][2] + P[1][3] * P[1][3] - P[1][2] * P[1][2] + P[2][3] * P[2][3] - P[2][2] * P[2][2];
		d = det(A);

		// part 9 :
		if (abs(d) > .00000000001) {
			memcpy(Ai, A, 9 * sizeof(double));

			Ai[0][0] = B[0];
			Ai[1][0] = B[1];
			Ai[2][0] = B[2];
			xc = det(Ai) / d;
			memcpy(Ai, A, 9 * sizeof(double));

			Ai[0][1] = B[0];
			Ai[1][1] = B[1];
			Ai[2][1] = B[2];
			yc = det(Ai) / d;
			memcpy(Ai, A, 9 * sizeof(double));

			Ai[0][2] = B[0];
			Ai[1][2] = B[1];
			Ai[2][2] = B[2];
			zc = det(Ai) / d;

			// part 10 :

			r2c = (P[0][0] - xc)*(P[0][0] - xc) + (P[1][0] - yc)*(P[1][0] - yc) + (P[2][0] - zc)*(P[2][0] - zc);
			if ((P[0][4] - xc)*(P[0][4] - xc) + (P[1][4] - yc)*(P[1][4] - yc) + (P[2][4] - zc)*(P[2][4] - zc) > r2c)
				return 1;
			else
				return 0;

		}


		else {
			throw;
		}
	}
	else {

		return 3;
	}
}

double MESH::det(double  A[][3]) {
	// part 1 :
	double a = A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) - A[0][1] * (A[1][0] * A[2][2] - A[1][2] * A[2][0]) + A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
	
	return a;
}

void MESH::flip14(CELL* Cp) {
	// part 1 :
	if (Cp->CType == CELL::tetra) {
		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();


		// part 2 :
		
		find(Cp, point);
		POINT* P[4];
		memcpy(P, Point, 4 * sizeof(POINT*));

		// part 3 :
		FACE fake0{ P[0],P[1],P[2] };
		FACE fake1{ P[0],P[1],P[3] };
		FACE fake2{ P[1],P[2],P[3] };
		FACE fake3{ P[0],P[2],P[3] };

		// part 4 :
		CELL C = *Cp;
		int k = 0;
		for (int i = 0; i < 4; i++) {
			if (Cp->F[i]->is_joined(&fake0) == 3) {
				C.F[0] = Cp->F[i];
				k++;
				continue;
			}
			if (Cp->F[i]->is_joined(&fake1) == 3) {
				C.F[1] = Cp->F[i];
				k++;
				continue;
			}
			if (Cp->F[i]->is_joined(&fake2) == 3) {
				C.F[2] = Cp->F[i];
				k++;
				continue;
			}
			if (Cp->F[i]->is_joined(&fake3) == 3) {
				C.F[3] = Cp->F[i];
				k++;
				continue;
			}
		}
		if (k != 4)
			throw;

		// part 5 :
		bPOINT bp = Cp->center();   //NOTICE Cp should be used instead of C
		POINT Pn;
		memcpy(Pn.X, bp.X, 3 * sizeof(double));
		PointStd.push_back(Pn);
		NP++;
		POINT* Pp = &p_a[NP - 1];

		// part 6 :
		FaceStd.resize(NF + 6);
		f_a[NF] = { Pp,P[0],P[1] };
		f_a[NF + 1] = { Pp,P[1],P[2] };
		f_a[NF + 2] = { Pp,P[2],P[0] };
		f_a[NF + 3] = { Pp,P[0],P[3] };
		f_a[NF + 4] = { Pp,P[1],P[3] };
		f_a[NF + 5] = { Pp,P[2],P[3] };
		NF = NF + 6;


		// part 7 :
		Cp->CellRemFace();
		Cp->CellAddFace(C.F[0]);
		Cp->CellAddFace(&f_a[NF - 6]);
		Cp->CellAddFace(&f_a[NF - 5]);
		Cp->CellAddFace(&f_a[NF - 4]);

		// part 8 :
		CellStd.resize(NC + 3);
		c_a[NC].CellAddFace(C.F[1]);
		c_a[NC].CellAddFace(&f_a[NF - 6]);
		c_a[NC].CellAddFace(&f_a[NF - 3]);
		c_a[NC].CellAddFace(&f_a[NF - 2]);

		// part 9 :
		c_a[NC + 1].CellAddFace(C.F[2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 5]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 1]);

		// part 10 :
		c_a[NC + 2].CellAddFace(C.F[3]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 4]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 1]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 3]);
		NC = NC + 3;

		// part 11 :
		Pp->PointAddCell(Cp);
		Pp->PointAddCell(&c_a[NC - 3]);
		Pp->PointAddCell(&c_a[NC - 2]);
		Pp->PointAddCell(&c_a[NC - 1]);

		// part 12 :
		P[0]->PointAddCell(&c_a[NC - 3]);
		P[1]->PointAddCell(&c_a[NC - 3]);
		P[3]->PointAddCell(&c_a[NC - 3]);

		// part 13 :
		P[1]->PointAddCell(&c_a[NC - 2]);
		P[2]->PointAddCell(&c_a[NC - 2]);
		P[3]->PointAddCell(&c_a[NC - 2]);

		// part 14 :
		P[0]->PointAddCell(&c_a[NC - 1]);
		P[2]->PointAddCell(&c_a[NC - 1]);
		P[3]->PointAddCell(&c_a[NC - 1]);

		// part 15 :
		if (P[3]->PointRemCell(Cp) != 1)
			throw;
		for (int i = 0; i < 4; i++)
			if (delauny(C.F[i]) == 32)
				break;


	}
}

void MESH::flip14(CELL* Cp,bPOINT bP) {
	// part 1 :
	if (Cp->CType == CELL::tetra) {
		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();


		// part 2 :

		find(Cp, point);
		POINT* P[4];
		memcpy(P, Point, 4 * sizeof(POINT*));

		// part 3 :
		FACE fake0{ P[0],P[1],P[2] };
		FACE fake1{ P[0],P[1],P[3] };
		FACE fake2{ P[1],P[2],P[3] };
		FACE fake3{ P[0],P[2],P[3] };

		// part 4 :
		CELL C = *Cp;
		int k = 0;
		for (int i = 0; i < 4; i++) {
			if (Cp->F[i]->is_joined(&fake0) == 3) {
				C.F[0] = Cp->F[i];
				k++;
				continue;
			}
			if (Cp->F[i]->is_joined(&fake1) == 3) {
				C.F[1] = Cp->F[i];
				k++;
				continue;
			}
			if (Cp->F[i]->is_joined(&fake2) == 3) {
				C.F[2] = Cp->F[i];
				k++;
				continue;
			}
			if (Cp->F[i]->is_joined(&fake3) == 3) {
				C.F[3] = Cp->F[i];
				k++;
				continue;
			}
		}
		if (k != 4)
			throw;

		// part 5 :
		POINT Pn;
		memcpy(Pn.X, bP.X, 3 * sizeof(double));
		PointStd.push_back(Pn);
		NP++;
		POINT* Pp = &p_a[NP - 1];

		// part 6 :
		FaceStd.resize(NF + 6);
		f_a[NF] = { Pp,P[0],P[1] };
		f_a[NF + 1] = { Pp,P[1],P[2] };
		f_a[NF + 2] = { Pp,P[2],P[0] };
		f_a[NF + 3] = { Pp,P[0],P[3] };
		f_a[NF + 4] = { Pp,P[1],P[3] };
		f_a[NF + 5] = { Pp,P[2],P[3] };
		NF = NF + 6;


		// part 7 :
		Cp->CellRemFace();
		Cp->CellAddFace(C.F[0]);
		Cp->CellAddFace(&f_a[NF - 6]);
		Cp->CellAddFace(&f_a[NF - 5]);
		Cp->CellAddFace(&f_a[NF - 4]);

		// part 8 :
		CellStd.resize(NC + 3);
		c_a[NC].CellAddFace(C.F[1]);
		c_a[NC].CellAddFace(&f_a[NF - 6]);
		c_a[NC].CellAddFace(&f_a[NF - 3]);
		c_a[NC].CellAddFace(&f_a[NF - 2]);

		// part 9 :
		c_a[NC + 1].CellAddFace(C.F[2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 5]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 1]);

		// part 10 :
		c_a[NC + 2].CellAddFace(C.F[3]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 4]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 1]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 3]);
		NC = NC + 3;

		// part 11 :
		Pp->PointAddCell(Cp);
		Pp->PointAddCell(&c_a[NC - 3]);
		Pp->PointAddCell(&c_a[NC - 2]);
		Pp->PointAddCell(&c_a[NC - 1]);

		// part 12 :
		P[0]->PointAddCell(&c_a[NC - 3]);
		P[1]->PointAddCell(&c_a[NC - 3]);
		P[3]->PointAddCell(&c_a[NC - 3]);

		// part 13 :
		P[1]->PointAddCell(&c_a[NC - 2]);
		P[2]->PointAddCell(&c_a[NC - 2]);
		P[3]->PointAddCell(&c_a[NC - 2]);

		// part 14 :
		P[0]->PointAddCell(&c_a[NC - 1]);
		P[2]->PointAddCell(&c_a[NC - 1]);
		P[3]->PointAddCell(&c_a[NC - 1]);

		// part 15 :
		if (P[3]->PointRemCell(Cp) != 1)
			throw;
		for (int i = 0; i < 4; i++)
			if (delauny(C.F[i]) == 32)
				break;


	}
}


int MESH::check23(FACE* Fp) {
	
	// part 1 :
	find(Fp, cell);

	// part 2 :
	if (Fp->state() == 1 && Cell[0]->CType == CELL::tetra) {
		POINT* P[5] = { Fp->P[0],Fp->P[1],Fp->P[2] ,nullptr,nullptr };

		// part 3 :
		find(Cell[0], point);

		for (int i = 0; i<4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2]) {
				P[3] = Point[i];
				break;
			}

		// part 4 :
		find(Cell[1], point);
		for (int i = 0; i<4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2]) {
				P[4] = Point[i];
				break;
			}

		// part 5 :
		POINT* p[4] = { P[0],P[1],P[2],P[3] };
		double a[5] = { 0 };
		a[0] = volume_(p);

		// part 6 :
		p[3] = P[4];
		a[1] = volume_(p);

		// part 7 :
		p[2] = P[3];
		a[2] = volume_(p);

		// part 8 :
		p[0] = P[2];
		a[3] = volume_(p);

		// part 9 :
		p[1] = P[0];
		a[4] = volume_(p);

		// part 10 :
		double A = a[0] + a[1];
		double B = a[2] + a[3] + a[4];
		//cout << A << " " << B << " " << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] <<"   " ;

		// part 11 :
		if (abs((A - B) / A) < ERRORE1 && a[2] / A > ERRORE2 && a[3] / A > ERRORE2 && a[4] / A > ERRORE2)
			return 1;
		else if (a[2] / A < ERRORE2 || a[3] / A < ERRORE2 || a[4] / A < ERRORE2) {
			//cout << endl<<A << " " << B << " " << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] <<endl;

			return 2;
		}
		else
			return 0;

	}
	return 3;
}

double MESH::volume_(POINT** p) {

	// part 1 :
	double A[3] = { p[0]->X[0],p[0]->X[1],p[0]->X[2] };
	double B[3] = { p[1]->X[0],p[1]->X[1],p[1]->X[2] };
	double C[3] = { p[2]->X[0],p[2]->X[1],p[2]->X[2] };
	double D[3] = { p[3]->X[0],p[3]->X[1],p[3]->X[2] };

	// part 2 :
	double a[3] = { A[0] - D[0],A[1] - D[1],A[2] - D[2] };
	double b[3] = { B[0] - D[0],B[1] - D[1],B[2] - D[2] };
	double c[3] = { C[0] - D[0],C[1] - D[1],C[2] - D[2] };

	// part 3 :
	double b_c[3] = { b[1] * c[2] - b[2] * c[1],-(b[0] * c[2] - b[2] * c[0]),b[0] * c[1] - b[1] * c[0] };

	// part 4 :
	double ab_c = a[0] * b_c[0] + a[1] * b_c[1] + a[2] * b_c[2];

	// part 5 :
	return abs(ab_c) / 6;
}
bool MESH::flip23(FACE* Fp) {

	// part 1 :
	if (check23(Fp) == 1) {

		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();


		// part 2 :
		find(Fp, cell);
		CELL* C[2] = { Cell[0],Cell[1] };
		FACE* F[6];
		POINT* P[5] = { Fp->P[0],Fp->P[1],Fp->P[2],nullptr,nullptr };



		// part 3 :
		int m = 0;
		for (int i = 0; i < 4; i++) {
			if (C[0]->F[i] != Fp)
				F[m++] = C[0]->F[i];
		}
		for (int i = 0; i < 4; i++) {
			if (C[1]->F[i] != Fp)
				F[m++] = C[1]->F[i];
		}
		if (m != 6)
			throw;

		// part 4 :
		if (F[0]->is_joined(F[3]) != 2)
			swap(F[3], F[4]);
		if (F[0]->is_joined(F[3]) != 2)
			swap(F[3], F[5]);
		if (F[1]->is_joined(F[4]) != 2)
			swap(F[4], F[5]);

		if (!(F[0]->is_joined(F[3]) == 2 && F[1]->is_joined(F[4]) == 2 && F[2]->is_joined(F[5]) == 2) == 1) {
			cout << endl << F[0]->is_joined(F[3]) << F[1]->is_joined(F[4]) << F[2]->is_joined(F[5]);
			throw;
		}

		// part 5 :
		for (int i = 0; i<3; i++)
			if (F[0]->P[i] != P[0] && F[0]->P[i] != P[1] && F[0]->P[i] != P[2]) {
				P[3] = F[0]->P[i];
				break;
			}
		for (int i = 0; i<3; i++)
			if (F[3]->P[i] != P[0] && F[3]->P[i] != P[1] && F[3]->P[i] != P[2]) {
				P[4] = F[3]->P[i];
				break;
			}

		EDGE E[3] = { { P[0],P[1] },{ P[1],P[2] },{ P[2],P[0] } };
		

		// part 6 :
		if (F[0]->is_in(P[0]) != 1)
			swap(P[0], P[1]);
		if (F[0]->is_in(P[1]) != 1)
			swap(P[1], P[2]);
		if (F[1]->is_in(P[1]) != 1)
			swap(P[0], P[1]);

		if (!(F[0]->is_in(P[0]) && F[0]->is_in(P[1]) && F[1]->is_in(P[1]) && F[1]->is_in(P[2]) && F[2]->is_in(P[2]) && F[2]->is_in(P[0])))
			throw;

		// part 7 :
		C[0]->RemoveCellFromPoints();
		C[1]->RemoveCellFromPoints();
		C[0]->CellRemFace();
		C[1]->CellRemFace();


		// part 8 :
		FaceStd.resize(NF + 2);
		Fp->P[0] = P[0];
		Fp->P[1] = P[3];
		Fp->P[2] = P[4];
		f_a[NF] = { P[1],P[3],P[4] };
		f_a[NF + 1] = { P[2],P[3],P[4] };
		NF = NF + 2;

		
		// part 9 :
		C[0]->CellAddFace(F[0]);
		C[0]->CellAddFace(F[3]);
		C[0]->CellAddFace(Fp);
		C[0]->CellAddFace(&f_a[NF - 2]);

		// part 10 :
		C[1]->CellAddFace(F[1]);
		C[1]->CellAddFace(F[4]);
		C[1]->CellAddFace(&f_a[NF - 2]);
		C[1]->CellAddFace(&f_a[NF - 1]);

		// part 11 :
		CellStd.resize(NC + 1);
		c_a[NC].CellAddFace(F[2]);
		c_a[NC].CellAddFace(F[5]);
		c_a[NC].CellAddFace(Fp);
		c_a[NC].CellAddFace(&f_a[NF - 1]);
		NC++;

		// part 12 :
		C[0]->AddCellToPoints();
		C[1]->AddCellToPoints();
		c_a[NC - 1].AddCellToPoints();



		return true;
	}
	return false;
}


int MESH::check32(EDGE* E) {
	// part 1 :
	int n = find(E, cell);
	CELL* C[3] = { Cell[0],Cell[1],Cell[2] };

	// part 2 :
	if (n == 3 && C[0]->CType == CELL::tetra && C[1]->CType == CELL::tetra && C[2]->CType == CELL::tetra && E->IsOnBound() == 0) {
		double a[5] = { C[0]->volume(),C[1]->volume(),C[2]->volume(),0,0 };
		int m = 2;

		// part 3 :
		POINT* P[5] = { E->P[0],E->P[1],nullptr,nullptr,nullptr };

		// part 4 :
		find(C[0], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1])
				P[m++] = Point[i];

		if (m != 4)
			throw;

		// part 5 :
		find(C[1], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3])
				P[m++] = Point[i];

		if (m != 5)
			throw;

		// part 6 :
		POINT* p[4] = { P[0],P[2],P[3],P[4] };
		a[3] = volume_(p);

		// part 7 :
		p[0] = P[1];
		a[4] = volume_(p);

		// part 8 :
		double A = a[0] + a[1] + a[2];
		double B = a[3] + a[4];

		// part 9 :
		if (abs((A - B) / A) < ERRORE1  && a[3] / A > ERRORE2 && a[4] / A > ERRORE2)
			return 1;
		else if (a[3] / A < ERRORE2 || a[4] / A < ERRORE2) {
			//cout << endl<<A << " " << B << " " << a[0] << " " << a[1] << " " << a[2] << " " << a[3] << " " << a[4] <<endl;

			return 2;
		}
		else
			return 0;
	}
	else
		return 3;
}
void MESH::clear() {

	// part 1 :
	static unsigned long long a = reinterpret_cast<unsigned long long>(PointStd.data());
	static unsigned long long b = reinterpret_cast<unsigned long long>(FaceStd.data());
	static unsigned long long c = reinterpret_cast<unsigned long long>(CellStd.data());
	unsigned long long ae = reinterpret_cast<unsigned long long>(PointStd.data() + NP);
	unsigned long long be = reinterpret_cast<unsigned long long>(FaceStd.data() + NF);
	unsigned long long ce = reinterpret_cast<unsigned long long>(CellStd.data() + NC);

	// part 2 :
	sort(D.begin(), D.end());
	unsigned long long v;

	// part 3 :
	for (int i = D.size() - 1; i >= 0; i--) {
		v = reinterpret_cast<unsigned long long>(D[i]);

		if (v >= a && v < ae)
			deletedata(reinterpret_cast<POINT*>(D[i]));
		else if (v >= b && v < be)
			deletedata(reinterpret_cast<FACE*>(D[i]));
		else if (v >= c && v < ce) {
			deletedata(reinterpret_cast<CELL*>(D[i]));
		}
		else {
			throw;
		}

	}
	D.clear();
}
void MESH::deletedata(POINT* address) {

	// part 1 :
	POINT* p_a = PointStd.data();
	if (A2N(address) != NP) {

		CELL** c = p_a[NP - 1].PointCells;
		for (int i = 0; i < p_a[NP - 1].PointCellSize; i++)
			for (int j = 0; j < (*(c + i))->CellFaceSize; j++)
				for (int k = 0; k < 4; k++)
					if ((*(c + i))->F[j]->P[k] == &p_a[NP - 1]) {
						(*(c + i))->F[j]->P[k] = address;
						break;
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
void MESH::deletedata(CELL* address) {

	// part 1 :
	int k = A2N(address);

	CELL* c_a = CellStd.data();
	if (A2N(address) != NC) {
		int m = find(&c_a[NC - 1], point);
		for (int i = 0; i < m; i++)
			for (int j = 0; j < Point[i]->PointCellSize; j++)
				if (Point[i]->PointCells[j] == &c_a[NC - 1]) {
					Point[i]->PointCells[j] = address;
					break;
				}


        // part 2 :
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
void MESH::deletedata(FACE* address) {

	// part 1 :
	POINT* p_a = PointStd.data();
	FACE* f_a = FaceStd.data();
	if (A2N(address) != NF) {

		int m = mesh.find(&f_a[NF - 1], MESH::cell);
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < Cell[i]->CellFaceSize; j++)
				if (Cell[i]->F[j] == &f_a[NF - 1])
					Cell[i]->F[j] = address;
		}

		// part 2 :
		*address = f_a[NF - 1];

		// part 3 :
		FaceStd.pop_back();
		f_a[NF - 1].FaceIsDel = 1;
		NF--;
	}

	// part 4 :
	else {

		FaceStd.pop_back();
		f_a[NF - 1].FaceIsDel = 1;
		NF--;
	}
}
bool MESH::flip32(EDGE* Ep) {

	// part 1 :
	if (check32(Ep) == 1) {
		FACE* F[9];
		int n = 0;

		// part 2 :
		int k = find(Ep, cell);
		POINT* P[5] = { Ep->P[0],Ep->P[1],nullptr,nullptr,nullptr };
		int m = 2;

		// part 3 :
		find(Cell[0], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1])
				P[m++] = Point[i];
		find(Cell[1], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3])
				P[m++] = Point[i];
		
		// part 4 :
		for (int i = 0; i < 4; i++)
			if (Cell[0]->F[i]->is_in(P[0]) && Cell[0]->F[i]->is_in(P[1]))
				F[n++] = Cell[0]->F[i];
		if (n != 2)
			throw;
		for (int i = 0; i < 4; i++)
			if (Cell[1]->F[i]->is_in(P[0]) && Cell[1]->F[i]->is_in(P[1]) && Cell[1]->F[i] != F[0] && Cell[1]->F[i] != F[1])
				F[n++] = Cell[1]->F[i];

		if (n != 3)
			throw;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 4; j++)
				if (Cell[i]->F[j] != F[0] && Cell[i]->F[j] != F[1] && Cell[i]->F[j] != F[2])
					F[n++] = Cell[i]->F[j];
		if (n != 9)
			throw;

		// part 5 :
		for (int i = 3; i < 9; i = i + 2)
			if (F[i]->is_in(P[0]))
				swap(F[i], F[i + 1]);


        // part 6 :
		Cell[0]->RemoveCellFromPoints();
		Cell[1]->RemoveCellFromPoints();
		Cell[2]->RemoveCellFromPoints();
		Cell[0]->CellRemFace();
		Cell[1]->CellRemFace();
		Cell[2]->CellRemFace();


		// part 7
		F[0]->FaceRemPoint();
		F[0]->FaceAddPoint(P[2]);
		F[0]->FaceAddPoint(P[3]);
		F[0]->FaceAddPoint(P[4]);

		// part 8 :
		Cell[0]->CellAddFace(F[0]);
		Cell[0]->CellAddFace(F[3]);
		Cell[0]->CellAddFace(F[5]);
		Cell[0]->CellAddFace(F[7]);

		// part 9 :
		Cell[1]->CellAddFace(F[0]);
		Cell[1]->CellAddFace(F[4]);
		Cell[1]->CellAddFace(F[6]);
		Cell[1]->CellAddFace(F[8]);

		// part 10 :
		Cell[0]->AddCellToPoints();
		Cell[1]->AddCellToPoints();

		// part 11 :
		D.push_back(Cell[2]);
		D.push_back(F[1]);
		D.push_back(F[2]);

		return true;
	}
	return false;
}




void MESH::flip26(FACE* Fp) {

	// part 1 :
	find(Fp, cell);
	if (Fp->state() == 1 && Cell[0]->CType == CELL::tetra) {
		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();


		// part 2 :
		FACE* F[6];
		POINT* P[5] = { Fp->P[0],Fp->P[1],Fp->P[2],nullptr,nullptr };
		int m = 0;

		// part 3 :
		for (int i = 0; i < 4; i++) {
			if (Cell[0]->F[i] != Fp)
				F[m++] = Cell[0]->F[i];
		}
		if (m != 3)
			throw;
		for (int i = 0; i < 4; i++) {
			if (Cell[1]->F[i] != Fp)
				F[m++] = Cell[1]->F[i];
		}
		if (m != 6)
			throw;


		// part 4 :
		if (F[0]->is_joined(F[3]) != 2) {

			swap(F[3], F[4]);

		}
		if (F[0]->is_joined(F[3]) != 2) {

			swap(F[3], F[5]);

		}
		if (F[1]->is_joined(F[4]) != 2) {

			swap(F[4], F[5]);

		}

		if (!(F[0]->is_joined(F[3]) == 2 && F[1]->is_joined(F[4]) == 2 && F[2]->is_joined(F[5]) == 2) == 1) {
			cout << endl << F[0]->is_joined(F[3]) << F[1]->is_joined(F[4]) << F[2]->is_joined(F[5]);
			throw;
		}


		// part 5 :
		for (int i = 0; i < 3; i++)
			if (F[0]->P[i] != P[0] && F[0]->P[i] != P[1] && F[0]->P[i] != P[2]) {
				P[3] = F[0]->P[i];
				break;
			}
		for (int i = 0; i < 3; i++)
			if (F[3]->P[i] != P[0] && F[3]->P[i] != P[1] && F[3]->P[i] != P[2]) {
				P[4] = F[3]->P[i];
				break;
			}

        // part 6 :
		if (F[0]->is_in(P[0]) != 1)
			swap(P[0], P[1]);
		if (F[0]->is_in(P[1]) != 1)
			swap(P[1], P[2]);
		if (F[1]->is_in(P[1]) != 1)
			swap(P[0], P[1]);

		if (!(F[0]->is_in(P[0]) && F[0]->is_in(P[1]) && F[1]->is_in(P[1]) && F[1]->is_in(P[2]) && F[2]->is_in(P[2]) && F[2]->is_in(P[0])))
			throw;


		memcpy(Fp->P, P, 3 * sizeof(POINT*));


		// part 7 :
		Cell[0]->RemoveCellFromPoints();
		Cell[1]->RemoveCellFromPoints();
		Cell[0]->CellRemFace();
		Cell[1]->CellRemFace();


		// part 8 : 
		bPOINT nP = Fp->center();
		PointStd.resize(NP + 1);
		memcpy(p_a[NP].X, nP.X, 3 * sizeof(double));
		NP++;
		POINT* Pp = &p_a[NP - 1];

		// part 9 :
		FaceStd.resize(NF + 8);
		Fp->FaceRemPoint();
		Fp->FaceAddPoint(Pp);
		Fp->FaceAddPoint(P[0]);
		Fp->FaceAddPoint(P[1]);


		// part 10 :
		f_a[NF].FaceAddPoint(Pp);
		f_a[NF].FaceAddPoint(P[1]);			//NF-8
		f_a[NF].FaceAddPoint(P[2]);

		
		f_a[NF + 1].FaceAddPoint(Pp);
		f_a[NF + 1].FaceAddPoint(P[2]);		//NF-7
		f_a[NF + 1].FaceAddPoint(P[0]);

		
		f_a[NF + 2].FaceAddPoint(Pp);
		f_a[NF + 2].FaceAddPoint(P[0]);		//NF-6
		f_a[NF + 2].FaceAddPoint(P[3]);

        
		f_a[NF + 3].FaceAddPoint(Pp);
		f_a[NF + 3].FaceAddPoint(P[1]);		//NF-5
		f_a[NF + 3].FaceAddPoint(P[3]);

		
		f_a[NF + 4].FaceAddPoint(Pp);
		f_a[NF + 4].FaceAddPoint(P[2]);		//NF-4
		f_a[NF + 4].FaceAddPoint(P[3]);

		
		f_a[NF + 5].FaceAddPoint(Pp);
		f_a[NF + 5].FaceAddPoint(P[0]);		//NF-3
		f_a[NF + 5].FaceAddPoint(P[4]);

		
		f_a[NF + 6].FaceAddPoint(Pp);
		f_a[NF + 6].FaceAddPoint(P[1]);		//NF-2
		f_a[NF + 6].FaceAddPoint(P[4]);

		
		f_a[NF + 7].FaceAddPoint(Pp);
		f_a[NF + 7].FaceAddPoint(P[2]);		//NF-1
		f_a[NF + 7].FaceAddPoint(P[4]);
		NF = NF + 8;

		// part 11 :
		CellStd.resize(NC + 4);
		Cell[0]->CellAddFace(Fp);
		Cell[0]->CellAddFace(F[0]);
		Cell[0]->CellAddFace(&f_a[NF - 6]);
		Cell[0]->CellAddFace(&f_a[NF - 5]);


	
		c_a[NC].CellAddFace(F[1]);
		c_a[NC].CellAddFace(&f_a[NF - 8]);
		c_a[NC].CellAddFace(&f_a[NF - 5]);
		c_a[NC].CellAddFace(&f_a[NF - 4]);


		c_a[NC + 1].CellAddFace(F[2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 7]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 4]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 6]);



		Cell[1]->CellAddFace(Fp);
		Cell[1]->CellAddFace(F[3]);
		Cell[1]->CellAddFace(&f_a[NF - 3]);
		Cell[1]->CellAddFace(&f_a[NF - 2]);



		c_a[NC + 2].CellAddFace(F[4]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 8]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 2]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 1]);

		
		c_a[NC + 3].CellAddFace(F[5]);
		c_a[NC + 3].CellAddFace(&f_a[NF - 7]);
		c_a[NC + 3].CellAddFace(&f_a[NF - 1]);
		c_a[NC + 3].CellAddFace(&f_a[NF - 3]);
		NC = NC + 4;


		// part 12 :
		Cell[1]->AddCellToPoints();
		Cell[0]->AddCellToPoints();
		c_a[NC - 1].AddCellToPoints();
		c_a[NC - 2].AddCellToPoints();
		c_a[NC - 3].AddCellToPoints();
		c_a[NC - 4].AddCellToPoints();




	}


}

void MESH::flip26(FACE* Fp,bPOINT bP) {

	// part 1 :
	find(Fp, cell);
	if (Fp->state() == 1 && Cell[0]->CType == CELL::tetra) {
		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();


		// part 2 :
		FACE* F[6];
		POINT* P[5] = { Fp->P[0],Fp->P[1],Fp->P[2],nullptr,nullptr };
		int m = 0;

		// part 3 :
		for (int i = 0; i < 4; i++) {
			if (Cell[0]->F[i] != Fp)
				F[m++] = Cell[0]->F[i];
		}
		if (m != 3)
			throw;
		for (int i = 0; i < 4; i++) {
			if (Cell[1]->F[i] != Fp)
				F[m++] = Cell[1]->F[i];
		}
		if (m != 6)
			throw;


		// part 4 :
		if (F[0]->is_joined(F[3]) != 2) {

			swap(F[3], F[4]);

		}
		if (F[0]->is_joined(F[3]) != 2) {

			swap(F[3], F[5]);

		}
		if (F[1]->is_joined(F[4]) != 2) {

			swap(F[4], F[5]);

		}

		if (!(F[0]->is_joined(F[3]) == 2 && F[1]->is_joined(F[4]) == 2 && F[2]->is_joined(F[5]) == 2) == 1) {
			cout << endl << F[0]->is_joined(F[3]) << F[1]->is_joined(F[4]) << F[2]->is_joined(F[5]);
			throw;
		}


		// part 5 :
		for (int i = 0; i < 3; i++)
			if (F[0]->P[i] != P[0] && F[0]->P[i] != P[1] && F[0]->P[i] != P[2]) {
				P[3] = F[0]->P[i];
				break;
			}
		for (int i = 0; i < 3; i++)
			if (F[3]->P[i] != P[0] && F[3]->P[i] != P[1] && F[3]->P[i] != P[2]) {
				P[4] = F[3]->P[i];
				break;
			}

		// part 6 :
		if (F[0]->is_in(P[0]) != 1)
			swap(P[0], P[1]);
		if (F[0]->is_in(P[1]) != 1)
			swap(P[1], P[2]);
		if (F[1]->is_in(P[1]) != 1)
			swap(P[0], P[1]);

		if (!(F[0]->is_in(P[0]) && F[0]->is_in(P[1]) && F[1]->is_in(P[1]) && F[1]->is_in(P[2]) && F[2]->is_in(P[2]) && F[2]->is_in(P[0])))
			throw;


		memcpy(Fp->P, P, 3 * sizeof(POINT*));


		// part 7 :
		Cell[0]->RemoveCellFromPoints();
		Cell[1]->RemoveCellFromPoints();
		Cell[0]->CellRemFace();
		Cell[1]->CellRemFace();


		// part 8 : 
		PointStd.resize(NP + 1);
		memcpy(p_a[NP].X, bP.X, 3 * sizeof(double));
		NP++;
		POINT* Pp = &p_a[NP - 1];

		// part 9 :
		FaceStd.resize(NF + 8);
		Fp->FaceRemPoint();
		Fp->FaceAddPoint(Pp);
		Fp->FaceAddPoint(P[0]);
		Fp->FaceAddPoint(P[1]);


		// part 10 :
		f_a[NF].FaceAddPoint(Pp);
		f_a[NF].FaceAddPoint(P[1]);			//NF-8
		f_a[NF].FaceAddPoint(P[2]);


		f_a[NF + 1].FaceAddPoint(Pp);
		f_a[NF + 1].FaceAddPoint(P[2]);		//NF-7
		f_a[NF + 1].FaceAddPoint(P[0]);


		f_a[NF + 2].FaceAddPoint(Pp);
		f_a[NF + 2].FaceAddPoint(P[0]);		//NF-6
		f_a[NF + 2].FaceAddPoint(P[3]);


		f_a[NF + 3].FaceAddPoint(Pp);
		f_a[NF + 3].FaceAddPoint(P[1]);		//NF-5
		f_a[NF + 3].FaceAddPoint(P[3]);


		f_a[NF + 4].FaceAddPoint(Pp);
		f_a[NF + 4].FaceAddPoint(P[2]);		//NF-4
		f_a[NF + 4].FaceAddPoint(P[3]);


		f_a[NF + 5].FaceAddPoint(Pp);
		f_a[NF + 5].FaceAddPoint(P[0]);		//NF-3
		f_a[NF + 5].FaceAddPoint(P[4]);


		f_a[NF + 6].FaceAddPoint(Pp);
		f_a[NF + 6].FaceAddPoint(P[1]);		//NF-2
		f_a[NF + 6].FaceAddPoint(P[4]);


		f_a[NF + 7].FaceAddPoint(Pp);
		f_a[NF + 7].FaceAddPoint(P[2]);		//NF-1
		f_a[NF + 7].FaceAddPoint(P[4]);
		NF = NF + 8;

		// part 11 :
		CellStd.resize(NC + 4);
		Cell[0]->CellAddFace(Fp);
		Cell[0]->CellAddFace(F[0]);
		Cell[0]->CellAddFace(&f_a[NF - 6]);
		Cell[0]->CellAddFace(&f_a[NF - 5]);



		c_a[NC].CellAddFace(F[1]);
		c_a[NC].CellAddFace(&f_a[NF - 8]);
		c_a[NC].CellAddFace(&f_a[NF - 5]);
		c_a[NC].CellAddFace(&f_a[NF - 4]);


		c_a[NC + 1].CellAddFace(F[2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 7]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 4]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 6]);



		Cell[1]->CellAddFace(Fp);
		Cell[1]->CellAddFace(F[3]);
		Cell[1]->CellAddFace(&f_a[NF - 3]);
		Cell[1]->CellAddFace(&f_a[NF - 2]);



		c_a[NC + 2].CellAddFace(F[4]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 8]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 2]);
		c_a[NC + 2].CellAddFace(&f_a[NF - 1]);


		c_a[NC + 3].CellAddFace(F[5]);
		c_a[NC + 3].CellAddFace(&f_a[NF - 7]);
		c_a[NC + 3].CellAddFace(&f_a[NF - 1]);
		c_a[NC + 3].CellAddFace(&f_a[NF - 3]);
		NC = NC + 4;


		// part 12 :
		Cell[1]->AddCellToPoints();
		Cell[0]->AddCellToPoints();
		c_a[NC - 1].AddCellToPoints();
		c_a[NC - 2].AddCellToPoints();
		c_a[NC - 3].AddCellToPoints();
		c_a[NC - 4].AddCellToPoints();




	}


}


void MESH::flip13(FACE* Fp) {

	// part 1 :
	find(Fp, cell);
	if (Fp->state() == 0 && Cell[0]->CType == CELL::tetra) {
		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();
		if (find(Fp, cell) != 1)
			throw;

		// part 2 :
		FACE* F[3];
		POINT* P[4] = { Fp->P[0],Fp->P[1],Fp->P[2],nullptr };
		int m = 0;

		// part 3 :
		for (int i = 0; i < 4; i++) {
			if (Cell[0]->F[i] != Fp)
				F[m++] = Cell[0]->F[i];
		}

		if (m != 3)
			throw;


		// part 4 :
		for (int i = 0; i < 3; i++)
			if (F[0]->P[i] != P[0] && F[0]->P[i] != P[1] && F[0]->P[i] != P[2]) {
				P[3] = F[0]->P[i];
				break;
			}

         // part 5 :
		if (F[0]->is_in(P[0]) != 1)
			swap(P[0], P[1]);
		if (F[0]->is_in(P[1]) != 1)
			swap(P[1], P[2]);
		if (F[1]->is_in(P[1]) != 1)
			swap(P[0], P[1]);

		if (!(F[0]->is_in(P[0]) && F[0]->is_in(P[1]) && F[1]->is_in(P[1]) && F[1]->is_in(P[2]) && F[2]->is_in(P[2]) && F[2]->is_in(P[0])))
			throw;

		memcpy(Fp->P, P, 3 * sizeof(POINT*));


		// part 6 :
		Cell[0]->RemoveCellFromPoints();
		Cell[0]->CellRemFace();

		// part 7 :
		bPOINT nP = Fp->center();
		PointStd.resize(NP + 1);
		memcpy(p_a[NP].X, nP.X, 3 * sizeof(double));
		NP++;
		POINT* Pp = &p_a[NP - 1];
		Pp->IsOnBound = 1;
		Pp->reg_vec.push_back(Fp->region);

		// part 8 :
		FaceStd.resize(NF + 5);
		Fp->P[2] = Pp;


		// part 9 :
		f_a[NF].FaceAddPoint(Pp);
		f_a[NF].FaceAddPoint(P[1]);
		f_a[NF].FaceAddPoint(P[2]);               // NF-5
		f_a[NF].IsOnBound = 1;
		f_a[NF].region = Fp->region;

		f_a[NF + 1].FaceAddPoint(Pp);
		f_a[NF + 1].FaceAddPoint(P[2]);
		f_a[NF + 1].FaceAddPoint(P[0]);          // NF-4
		f_a[NF + 1].IsOnBound = 1;
		f_a[NF + 1].region = Fp->region;

		f_a[NF + 2].FaceAddPoint(Pp);
		f_a[NF + 2].FaceAddPoint(P[0]);           // NF-3
		f_a[NF + 2].FaceAddPoint(P[3]);

		f_a[NF + 3].FaceAddPoint(Pp);
		f_a[NF + 3].FaceAddPoint(P[1]);          // NF-2
		f_a[NF + 3].FaceAddPoint(P[3]);

		f_a[NF + 4].FaceAddPoint(Pp);
		f_a[NF + 4].FaceAddPoint(P[2]);          // NF-1
		f_a[NF + 4].FaceAddPoint(P[3]);
		NF = NF + 5;


		// part 10 :
		CellStd.resize(NC + 2);
		Cell[0]->CellAddFace(Fp);
		Cell[0]->CellAddFace(F[0]);
		Cell[0]->CellAddFace(&f_a[NF - 3]);
		Cell[0]->CellAddFace(&f_a[NF - 2]);

		c_a[NC].CellAddFace(F[1]);
		c_a[NC].CellAddFace(&f_a[NF - 5]);
		c_a[NC].CellAddFace(&f_a[NF - 2]);
		c_a[NC].CellAddFace(&f_a[NF - 1]);

		c_a[NC + 1].CellAddFace(F[2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 4]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 1]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 3]);
		NC = NC + 2;

		// part 11 :
		Cell[0]->AddCellToPoints();
		c_a[NC - 1].AddCellToPoints();
		c_a[NC - 2].AddCellToPoints();


	}

}

void MESH::flip13(FACE* Fp,bPOINT bP) {

	// part 1 :
	find(Fp, cell);
	if (Fp->state() == 0 && Cell[0]->CType == CELL::tetra) {
		POINT* p_a = PointStd.data();
		FACE* f_a = FaceStd.data();
		CELL* c_a = CellStd.data();
		if (find(Fp, cell) != 1)
			throw;

		// part 2 :
		FACE* F[3];
		POINT* P[4] = { Fp->P[0],Fp->P[1],Fp->P[2],nullptr };
		int m = 0;

		// part 3 :
		for (int i = 0; i < 4; i++) {
			if (Cell[0]->F[i] != Fp)
				F[m++] = Cell[0]->F[i];
		}

		if (m != 3)
			throw;


		// part 4 :
		for (int i = 0; i < 3; i++)
			if (F[0]->P[i] != P[0] && F[0]->P[i] != P[1] && F[0]->P[i] != P[2]) {
				P[3] = F[0]->P[i];
				break;
			}

		// part 5 :
		if (F[0]->is_in(P[0]) != 1)
			swap(P[0], P[1]);
		if (F[0]->is_in(P[1]) != 1)
			swap(P[1], P[2]);
		if (F[1]->is_in(P[1]) != 1)
			swap(P[0], P[1]);

		if (!(F[0]->is_in(P[0]) && F[0]->is_in(P[1]) && F[1]->is_in(P[1]) && F[1]->is_in(P[2]) && F[2]->is_in(P[2]) && F[2]->is_in(P[0])))
			throw;

		memcpy(Fp->P, P, 3 * sizeof(POINT*));


		// part 6 :
		Cell[0]->RemoveCellFromPoints();
		Cell[0]->CellRemFace();

		// part 7 :
		PointStd.resize(NP + 1);
		memcpy(p_a[NP].X, bP.X, 3 * sizeof(double));
		NP++;
		POINT* Pp = &p_a[NP - 1];
		Pp->IsOnBound = 1;
		Pp->reg_vec.push_back(Fp->region);

		// part 8 :
		FaceStd.resize(NF + 5);
		Fp->P[2] = Pp;


		// part 9 :
		f_a[NF].FaceAddPoint(Pp);
		f_a[NF].FaceAddPoint(P[1]);
		f_a[NF].FaceAddPoint(P[2]);               // NF-5
		f_a[NF].IsOnBound = 1;
		f_a[NF].region = Fp->region;

		f_a[NF + 1].FaceAddPoint(Pp);
		f_a[NF + 1].FaceAddPoint(P[2]);
		f_a[NF + 1].FaceAddPoint(P[0]);          // NF-4
		f_a[NF + 1].IsOnBound = 1;
		f_a[NF + 1].region = Fp->region;

		f_a[NF + 2].FaceAddPoint(Pp);
		f_a[NF + 2].FaceAddPoint(P[0]);           // NF-3
		f_a[NF + 2].FaceAddPoint(P[3]);

		f_a[NF + 3].FaceAddPoint(Pp);
		f_a[NF + 3].FaceAddPoint(P[1]);          // NF-2
		f_a[NF + 3].FaceAddPoint(P[3]);

		f_a[NF + 4].FaceAddPoint(Pp);
		f_a[NF + 4].FaceAddPoint(P[2]);          // NF-1
		f_a[NF + 4].FaceAddPoint(P[3]);
		NF = NF + 5;


		// part 10 :
		CellStd.resize(NC + 2);
		Cell[0]->CellAddFace(Fp);
		Cell[0]->CellAddFace(F[0]);
		Cell[0]->CellAddFace(&f_a[NF - 3]);
		Cell[0]->CellAddFace(&f_a[NF - 2]);

		c_a[NC].CellAddFace(F[1]);
		c_a[NC].CellAddFace(&f_a[NF - 5]);
		c_a[NC].CellAddFace(&f_a[NF - 2]);
		c_a[NC].CellAddFace(&f_a[NF - 1]);

		c_a[NC + 1].CellAddFace(F[2]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 4]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 1]);
		c_a[NC + 1].CellAddFace(&f_a[NF - 3]);
		NC = NC + 2;

		// part 11 :
		Cell[0]->AddCellToPoints();
		c_a[NC - 1].AddCellToPoints();
		c_a[NC - 2].AddCellToPoints();


	}

}

int MESH::check44(EDGE* Ep) {

	// part 1 :
	int t = 3;
	int n = find(Ep, cell);
    CELL* C[4] = { Cell[0],Cell[1] ,Cell[2] ,Cell[3] };

	// part 2 :
	if (n == 4 && C[0]->CType == CELL::tetra && C[1]->CType == CELL::tetra && C[2]->CType == CELL::tetra && C[3]->CType == CELL::tetra && Ep->IsOnBound() == 0) {
		POINT* P[6] = { Ep->P[0],Ep->P[1],nullptr,nullptr,nullptr,nullptr };
		double a[12];
		int m = 2;


		// part 3 :
		a[0] = C[0]->volume();
		//cout << endl << A2N(Point[0]) << " " << A2N(Point[1]) << " " << A2N(Point[2]) << " " << A2N(Point[3])<<A2N(C[0]);
		a[1] = C[1]->volume();
		//cout << endl << A2N(Point[0]) << " " << A2N(Point[1]) << " " << A2N(Point[2]) << " " << A2N(Point[3]) << A2N(C[1]);
		a[2] = C[2]->volume();
		//cout << endl << A2N(Point[0]) << " " << A2N(Point[1]) << " " << A2N(Point[2]) << " " << A2N(Point[3]) << A2N(C[2]);
		a[3] = C[3]->volume();
		//cout << endl << A2N(Point[0]) << " " << A2N(Point[1]) << " " << A2N(Point[2]) << " " << A2N(Point[3])<<A2N(C[3])<<endl;


		// part 4 :
		find(C[0], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1])
				P[m++] = Point[i];
		if (m != 4)
			throw;

		find(C[1], point);
		if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
			swap(C[1], C[2]);
			find(C[1], point);
		}
		if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
			swap(C[1], C[3]);
			find(C[1], point);
		}

		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3])
				P[m++] = Point[i];
		if (m != 5)
			throw;
		find(C[2], point);
		if (P[4] != Point[0] && P[4] != Point[1] && P[4] != Point[2] && P[4] != Point[3]) {
			swap(C[2], C[3]);
			find(C[2], point);
		}
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3] && Point[i] != P[4])
				P[m++] = Point[i];

		if (m != 6) {
			throw;
		}


		// part 5 :
		POINT* p[4] = { P[0],P[2],P[3],P[4] };
		a[4] = volume_(p);
		p[0] = P[1];
		a[5] = volume_(p);
		p[2] = P[5];
		a[6] = volume_(p);
		p[0] = P[0];
		a[7] = volume_(p);
		p[3] = P[3];
		a[8] = volume_(p);
		p[0] = P[1];
		a[9] = volume_(p);
		p[1] = P[4];
		a[10] = volume_(p);
		p[0] = P[0];
		a[11] = volume_(p);

		// part 6 :
		double A = a[0] + a[1] + a[2] + a[3];
		double B = a[4] + a[5] + a[6] + a[7];
		double D = a[8] + a[9] + a[10] + a[11];
		//cout << A << "\t" << B << "\t" << D << endl;
		t = 0;

		// part 7 :
		if (abs((A - B) / A) < ERRORE1 && a[4] / A > ERRORE2 && a[5] / A > ERRORE2 && a[6] / A > ERRORE2 && a[7] / A > ERRORE2)
			t = 1;
		if (abs((A - D) / A) < ERRORE1 && a[8] / A > ERRORE2 && a[9] / A > ERRORE2 && a[10] / A > ERRORE2 && a[11] / A > ERRORE2)
			t = 2;
		if (abs((A - B) / A) < ERRORE1 && a[4] / A > ERRORE2 && a[5] / A > ERRORE2 && a[6] / A > ERRORE2 && a[7] / A > ERRORE2 && abs((A - D) / A) < ERRORE1 && a[8] / A > ERRORE2 && a[9] / A > ERRORE2 && a[10] / A > ERRORE2 && a[11] / A > ERRORE2)
			t = 4;

	}
	return t;
}
bool MESH::flip44(EDGE* Ep) {

	// part 1 :
	int n = check44(Ep);
	if (n == 0 || n == 3)
		return false;
	if (n == 1 || n == 4) {

		// part 2 :
		find(Ep, cell);
		CELL* C[4] = { Cell[0],Cell[1] ,Cell[2] ,Cell[3] };
		POINT* P[6] = { Ep->P[0],Ep->P[1],nullptr,nullptr,nullptr,nullptr };
		FACE* F[12];
		int m = 2;

		// part 3 :
		find(C[0], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1])
				P[m++] = Point[i];


		find(C[1], point);
		if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
			swap(C[1], C[2]);
			find(C[1], point);
		}
		if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
			swap(C[1], C[3]);
			find(C[1], point);
		}

		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3])
				P[m++] = Point[i];


		find(C[2], point);
		if (P[4] != Point[0] && P[4] != Point[1] && P[4] != Point[2] && P[4] != Point[3]) {
			swap(C[2], C[3]);
			find(C[2], point);
		}
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3] && Point[i] != P[4])
				P[m++] = Point[i];
		if (m != 6)
			throw;


		// part 4 :
		int t = 0;
		for (int i = 0; i < 4; i++)
			if (C[0]->F[i]->is_in(P[0]) && C[0]->F[i]->is_in(P[1]))
				F[t++] = C[0]->F[i];
		for (int i = 0; i < 4; i++)
			if (C[2]->F[i]->is_in(P[0]) && C[2]->F[i]->is_in(P[1]))
				F[t++] = C[2]->F[i];
		if (t != 4)
			throw;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (C[i]->F[j] != F[0] && C[i]->F[j] != F[1] && C[i]->F[j] != F[2] && C[i]->F[j] != F[3])
					F[t++] = C[i]->F[j];
		if (t != 12)
			throw;

		// part 5 :
		for (int i = 4; i < 11; i = i + 2)
			if (F[i]->is_in(P[0]) != 1)
				swap(F[i], F[i + 1]);


        // part 6 :
		for (int i = 0; i < 4; i++)
			C[i]->RemoveCellFromPoints();
		for (int i = 0; i < 4; i++)
			C[i]->CellRemFace();
		for (int i = 0; i < 4; i++)
			F[i]->FaceRemPoint();


		// part 7 :
		F[0]->FaceAddPoint(P[2]);
		F[0]->FaceAddPoint(P[0]);
		F[0]->FaceAddPoint(P[4]);

		F[1]->FaceAddPoint(P[2]);
		F[1]->FaceAddPoint(P[3]);
		F[1]->FaceAddPoint(P[4]);

		F[2]->FaceAddPoint(P[2]);
		F[2]->FaceAddPoint(P[1]);
		F[2]->FaceAddPoint(P[4]);

		F[3]->FaceAddPoint(P[2]);
		F[3]->FaceAddPoint(P[5]);
		F[3]->FaceAddPoint(P[4]);


		// part 8 :
		C[0]->CellAddFace(F[0]);
		C[0]->CellAddFace(F[1]);
		C[0]->CellAddFace(F[4]);
		C[0]->CellAddFace(F[6]);

		C[1]->CellAddFace(F[1]);
		C[1]->CellAddFace(F[2]);
		C[1]->CellAddFace(F[5]);
		C[1]->CellAddFace(F[7]);

		C[2]->CellAddFace(F[2]);
		C[2]->CellAddFace(F[3]);
		C[2]->CellAddFace(F[9]);
		C[2]->CellAddFace(F[11]);

		C[3]->CellAddFace(F[3]);
		C[3]->CellAddFace(F[0]);
		C[3]->CellAddFace(F[8]);
		C[3]->CellAddFace(F[10]);


		// part 9
		for (int i = 0; i < 4; i++)
			C[i]->AddCellToPoints();
		return true;
	}

	//part 10 :
	if (n == 2) {

		// part 11
		find(Ep, cell);
		CELL* C[4] = { Cell[0],Cell[1] ,Cell[2] ,Cell[3] };
		POINT* P[6] = { Ep->P[0],Ep->P[1],nullptr,nullptr,nullptr,nullptr };
		FACE* F[12];
		int m = 2;

		// part 12 :
		find(C[0], point);
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1])
				P[m++] = Point[i];


		find(C[1], point);
		if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
			swap(C[1], C[2]);
			find(C[1], point);
		}
		if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
			swap(C[1], C[3]);
			find(C[1], point);
		}

		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3])
				P[m++] = Point[i];


		find(C[2], point);
		if (P[4] != Point[0] && P[4] != Point[1] && P[4] != Point[2] && P[4] != Point[3]) {
			swap(C[2], C[3]);
			find(C[2], point);
		}
		for (int i = 0; i < 4; i++)
			if (Point[i] != P[0] && Point[i] != P[1] && Point[i] != P[2] && Point[i] != P[3] && Point[i] != P[4])
				P[m++] = Point[i];
		if (m != 6)
			throw;

		// part 13 :
		int t = 0;
		for (int i = 0; i < 4; i++)
			if (C[0]->F[i]->is_in(P[0]) && C[0]->F[i]->is_in(P[1]))
				F[t++] = C[0]->F[i];
		for (int i = 0; i < 4; i++)
			if (C[2]->F[i]->is_in(P[0]) && C[2]->F[i]->is_in(P[1]))
				F[t++] = C[2]->F[i];
		if (t != 4)
			throw;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (C[i]->F[j] != F[0] && C[i]->F[j] != F[1] && C[i]->F[j] != F[2] && C[i]->F[j] != F[3])
					F[t++] = C[i]->F[j];
		if (t != 12)
			throw;

		// part 14 :
		for (int i = 4; i < 11; i = i + 2)
			if (F[i]->is_in(P[0]) != 1)
				swap(F[i], F[i + 1]);


        // part 15 :
		for (int i = 0; i < 4; i++)
			C[i]->RemoveCellFromPoints();
		for (int i = 0; i < 4; i++)
			C[i]->CellRemFace();
		for (int i = 0; i < 4; i++)
			F[i]->FaceRemPoint();

		// part 16 :
		F[0]->FaceAddPoint(P[3]);
		F[0]->FaceAddPoint(P[0]);
		F[0]->FaceAddPoint(P[5]);

		F[1]->FaceAddPoint(P[3]);
		F[1]->FaceAddPoint(P[4]);
		F[1]->FaceAddPoint(P[5]);

		F[2]->FaceAddPoint(P[3]);
		F[2]->FaceAddPoint(P[1]);
		F[2]->FaceAddPoint(P[5]);

		F[3]->FaceAddPoint(P[3]);
		F[3]->FaceAddPoint(P[2]);
		F[3]->FaceAddPoint(P[5]);


		// part 17 :
		C[0]->CellAddFace(F[0]);
		C[0]->CellAddFace(F[1]);
		C[0]->CellAddFace(F[6]);
		C[0]->CellAddFace(F[8]);

		C[1]->CellAddFace(F[1]);
		C[1]->CellAddFace(F[2]);
		C[1]->CellAddFace(F[7]);
		C[1]->CellAddFace(F[9]);

		C[2]->CellAddFace(F[2]);
		C[2]->CellAddFace(F[3]);
		C[2]->CellAddFace(F[5]);
		C[2]->CellAddFace(F[11]);

		C[3]->CellAddFace(F[3]);
		C[3]->CellAddFace(F[0]);
		C[3]->CellAddFace(F[4]);
		C[3]->CellAddFace(F[10]);


		// part 18 :
		for (int i = 0; i < 4; i++)
			C[i]->AddCellToPoints();
		return true;
	}
}

int MESH::check22(EDGE* Ep) {

	// part 1 :
	int n = find(Ep, cell);
	CELL* C[2] = { Cell[0],Cell[1] };
	double vol;
	if (n == 2 && C[0]->CType == CELL::tetra && C[1]->CType == CELL::tetra) {
		FACE* s[2] = { nullptr };
		for (int i = 0; i < 4; i++)
			if (Cell[0]->F[i]->state() == 0)
				s[0] = Cell[0]->F[i];
		for (int i = 0; i < 4; i++)
			if (Cell[1]->F[i]->state() == 0)
				s[1] = Cell[1]->F[i];
		POINT* p[2] = { nullptr };
		s[0]->join(s[1], p);
		POINT* pv[4] = { nullptr };
		pv[0] = p[0];
		pv[1] = p[1];

		for (int i = 0; i < 3; i++)
			if (s[0]->P[i] != p[0] && s[0]->P[i] != p[1])
				pv[2] = s[0]->P[i];
		for (int i = 0; i < 3; i++)
			if (s[1]->P[i] != p[0] && s[1]->P[i] != p[1])
				pv[3] = s[1]->P[i];
		vol = volume_(pv);
	}
	// part 2 :
	if (n == 2 && C[0]->CType == CELL::tetra && C[1]->CType == CELL::tetra && Ep->IsOnBound() == 1 && vol==0) {
		POINT* P[5] = { Ep->P[0],Ep->P[1],nullptr,nullptr,nullptr };
		double a[4];
		find(C[0], point);
		int m = 2;
		FACE* Fp = nullptr;

		// part 3 :
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (C[0]->F[i] == C[1]->F[j])
					Fp = C[0]->F[i];

        // part 4 :
		for (int i = 0; i < 4; i++)
			if (Fp->is_in(Point[i]) == 0)
				P[m++] = Point[i];
		if (m != 3)
			throw;
		find(C[1], point);
		for (int i = 0; i < 4; i++)
			if (Fp->is_in(Point[i]) == 0)
				P[m++] = Point[i];
		if (m != 4)
			throw;
		for (int i = 0; i < 3; i++)
			if (Fp->P[i] != P[0] && Fp->P[i] != P[1])
				P[4] = Fp->P[i];

        // part 5 :
		a[0] = C[0]->volume();
		a[1] = C[1]->volume();

		// part 6 :
		POINT* p[4] = { P[0],P[2],P[3],P[4] };
		a[2] = volume_(p);
		p[0] = P[1];
		a[3] = volume_(p);

		// part 7 :
		double A = a[0] + a[1];
		double B = a[2] + a[3];

		// part 8 :
		if (abs((A - B) / A) < ERRORE1 && a[2] / A > ERRORE2 && a[3] / A > ERRORE2) 
			return 1;
		else
			return 0;

	}
	return 3;
}
bool MESH::flip22(EDGE* Ep) {

	// part 1 :
	if (check22(Ep) == 1) {

		// part 2 :
		find(Ep, cell);
		CELL* C[2] = { Cell[0],Cell[1] };
		POINT* P[5] = { Ep->P[0],Ep->P[1],nullptr,nullptr,nullptr };
		FACE* F[6];
		FACE* Fp = nullptr;

		// part 3 :
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				if (C[0]->F[i] == C[1]->F[j]) {
					Fp = C[0]->F[i];
					break;
				}

        // part 4 :
		find(C[0], point);
		for (int i = 0; i < 4; i++)
			if (Fp->is_in(Point[i]) == 0)
				P[2] = Point[i];

		find(C[1], point);
		for (int i = 0; i < 4; i++)
			if (Fp->is_in(Point[i]) == 0)
				P[3] = Point[i];

		for (int i = 0; i < 3; i++)
			if (Fp->P[i] != P[0] && Fp->P[i] != P[1])
				P[4] = Fp->P[i];

        // part 5 :
		int m = 0;
		for (int i = 0; i < 4; i++)
			if (C[0]->F[i] != Fp)
				F[m++] = C[0]->F[i];
		if (m != 3)
			throw;
		for (int i = 0; i < 4; i++)
			if (C[1]->F[i] != Fp)
				F[m++] = C[1]->F[i];
		if (m != 6)
			throw;

		// part 6 :
		for (int i = 0; i<3; i++)
			if (F[i]->is_in(P[0]) == 1 && F[i]->is_in(P[1]) == 1) {
				swap(F[i], F[0]);
				break;
			}
		for (int i = 3; i<6; i++)
			if (F[i]->is_in(P[0]) == 1 && F[i]->is_in(P[1]) == 1) {
				swap(F[i], F[3]);
				break;
			}
		if (F[1]->is_in(P[0]) != 1)
			swap(F[1], F[2]);
		if (F[4]->is_in(P[0]) != 1)
			swap(F[4], F[5]);


		// part 7 :
		C[0]->RemoveCellFromPoints();
		C[1]->RemoveCellFromPoints();
		C[0]->CellRemFace();
		C[1]->CellRemFace();
		Fp->FaceRemPoint();
		F[0]->FaceRemPoint();
		F[3]->FaceRemPoint();

		// part 8 :
		Fp->FaceAddPoint(P[2]);
		Fp->FaceAddPoint(P[3]);
		Fp->FaceAddPoint(P[4]);

		F[0]->FaceAddPoint(P[2]);
		F[0]->FaceAddPoint(P[3]);
		F[0]->FaceAddPoint(P[0]);

		F[3]->FaceAddPoint(P[2]);
		F[3]->FaceAddPoint(P[3]);
		F[3]->FaceAddPoint(P[1]);


		// part 9 :
		C[0]->CellAddFace(Fp);
		C[0]->CellAddFace(F[0]);
		C[0]->CellAddFace(F[1]);
		C[0]->CellAddFace(F[4]);

		C[1]->CellAddFace(Fp);
		C[1]->CellAddFace(F[3]);
		C[1]->CellAddFace(F[2]);
		C[1]->CellAddFace(F[5]);

		// part 10 :
		C[0]->AddCellToPoints();
		C[1]->AddCellToPoints();

		return true;


	}
	return false;
}


void MESH::laplacian() {

	// part 1 :
	POINT* p_a = PointStd.data();
	FACE* f_a = FaceStd.data();
	CELL* c_a = CellStd.data();
	double a[MAX_NEIB] = { 0 };
	double an[MAX_NEIB] = { 0 };
	double A, B;
	bPOINT P[MAX_NEIB];
	bPOINT Cp;
	bPOINT Pold;

	// part 2 :
	for (int i = 0; i < mesh.NP; i++) {
		Cp.X[0] = 0;
		Cp.X[1] = 0;
		Cp.X[2] = 0;
		A = 0;
		B = 0;

		// part 3 :
		for (int j = 0; j < p_a[i].PointCellSize; j++) {
			if (p_a[i].PointCells[j]->CType != 0) {
				goto lable;
			}

			// part 4 : 
			a[j] = p_a[i].PointCells[j]->volume();
			A = A + a[j];
			P[j] = p_a[i].PointCells[j]->center();
			Cp.X[0] = Cp.X[0] + P[j].X[0];
			Cp.X[1] = Cp.X[1] + P[j].X[1];
			Cp.X[2] = Cp.X[2] + P[j].X[2];
		}

		// part 5 :
		Cp.X[0] = Cp.X[0] / p_a[i].PointCellSize;
		Cp.X[1] = Cp.X[1] / p_a[i].PointCellSize;
		Cp.X[2] = Cp.X[2] / p_a[i].PointCellSize;
		memcpy(Pold.X, p_a[i].X, 3 * sizeof(double));
		memcpy(p_a[i].X, Cp.X, 3 * sizeof(double));

		// part 6 :
		for (int j = 0; j < p_a[i].PointCellSize; j++) {
			an[j] = p_a[i].PointCells[j]->volume();
			B = B + an[j];
			if (an[j] / a[j] < .001 || an[j]<.0001*A) {
				memcpy(p_a[i].X, Pold.X, 3 * sizeof(double));
				goto lable;
			}
		}

		// part 7 :
		if (abs((A - B) / A) > ERRORE1) {
			memcpy(p_a[i].X, Pold.X, 3 * sizeof(double));
			goto lable;
		}


	lable:
		;
	}
}
void MESH::flip12(EDGE* Ep) {

	// part 1 :
	POINT* p_a = PointStd.data();
	FACE* f_a = FaceStd.data();
	CELL* c_a = CellStd.data();
	double a = 0, b = 0, c = 0;
	POINT* Pe[2] = { Ep->P[0],Ep->P[1] };
	int n = find(Ep, cell);
	CELL* C[100] = { nullptr };
	memcpy(C, Cell, n * sizeof(CELL*));
	CELL* New_C[100] = { nullptr };
	int t = 0;
	for (int i = 0; i < n; i++)
		if (Cell[i]->CType != CELL::tetra) {
			t = 1;
			break;
		}

    // part 2 :
	CELL* Cp = C[0];
	if (t == 0 && Ep->IsOnBound() == 0) {
		FACE F[200];
		bPOINT Mp = Ep->mid();
		PointStd.resize(NP + 1);
		memcpy(p_a[NP].X, Mp.X, 3 * sizeof(double));
		NP++;
		POINT* P[100] = { nullptr };
		int C_count = 0;
		New_C[C_count++] = Cp;
		FACE* Fp = nullptr;

		// part 3 :
		for (int i = 0; i < 4; i++)
			if (C[0]->F[i]->is_in(Pe[0]) && C[0]->F[i]->is_in(Pe[1])) {
				Fp = C[0]->F[i];
				break;
			}

        // part 4 :
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 3; j++)
				if (Fp->P[j] != Pe[0] && Fp->P[j] != Pe[1])
					P[i] = Fp->P[j];
        
			// part 5 :
			find(Cp, point);

			if (find(Fp, cell) == 2) {
				if (Cell[0] == Cp)
					swap(Cell[0], Cell[1]);
				for (int k = 0; k < 4; k++)
					if (Cell[0]->F[k]->is_in(Pe[0]) && Cell[0]->F[k]->is_in(Pe[1]) && Cell[0]->F[k] != Fp) {
						Fp = Cell[0]->F[k];
						Cp = Cell[0];
						New_C[C_count++] = Cp;
						break;
					}
			}
			else
				throw;

			//part 6 :
			F[2 * i].FaceAddPoint(&p_a[NP - 1]);
			for (int j = 0; j<4; j++)
				if (Point[j] != Pe[0] && Point[j] != Pe[1])
					F[2 * i].FaceAddPoint(Point[j]);
			if (F[2 * i].FacePointSize != 3)
				throw;
			F[2 * i + 1].FaceAddPoint(&p_a[NP - 1]);
			F[2 * i + 1].FaceAddPoint(Pe[0]);
			F[2 * i + 1].FaceAddPoint(P[i]);

		}

		// part 7 :
		FaceStd.resize(NF + 2 * n);
		for (int i = 0; i < 2 * n; i++)
			f_a[NF + i] = F[i];

		int check_F = NF;
		NF = NF + 2 * n;

		// part 8 :
		for (int i = 0; i < n; i++)
			C[i]->RemoveCellFromPoints();


		// part 9 :
		CellStd.resize(NC + n);
		for (int i = 0; i<n; i++)
			for (int j = 0; j < 4; j++) {
				if (New_C[i]->F[j]->is_in(Pe[1]) == 0) {
					c_a[NC + i].CellAddFace(New_C[i]->F[j]);
					New_C[i]->F[j] = &f_a[check_F + 2 * i];
					break;
				}
			}

        // part 10 :
		for (int i = 0; i < n; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 3; k++)
					if (New_C[i]->F[j]->P[k] == Pe[0])
						New_C[i]->F[j]->P[k] = &p_a[NP - 1];


		// part 11 :			 
		for (int i = 0; i < n; i++)
			C[i]->AddCellToPoints();


		// part 12 :
		c_a[NC].CellAddFace(&f_a[check_F]);
		c_a[NC].CellAddFace(&f_a[check_F + 1]);
		c_a[NC].CellAddFace(&f_a[NF - 1]);

		c_a[NC].AddCellToPoints();
		NC++;

		// part 13 :
		for (int i = 1; i < n; i++) {
			c_a[NC].CellAddFace(&f_a[check_F + 2 * i]);
			c_a[NC].CellAddFace(&f_a[check_F + 2 * i - 1]);
			c_a[NC].CellAddFace(&f_a[check_F + 2 * i + 1]);
			c_a[NC].AddCellToPoints();
			NC++;
		}

	}



}

void MESH::flip12(EDGE* Ep,bPOINT bP) {

	// part 1 :
	POINT* p_a = PointStd.data();
	FACE* f_a = FaceStd.data();
	CELL* c_a = CellStd.data();
	double a = 0, b = 0, c = 0;
	POINT* Pe[2] = { Ep->P[0],Ep->P[1] };
	int n = find(Ep, cell);
	CELL* C[100] = { nullptr };
	memcpy(C, Cell, n * sizeof(CELL*));
	CELL* New_C[100] = { nullptr };
	int t = 0;
	for (int i = 0; i < n; i++)
		if (Cell[i]->CType != CELL::tetra) {
			t = 1;
			break;
		}

	// part 2 :
	CELL* Cp = C[0];
	if (t == 0 && Ep->IsOnBound() == 0) {
		FACE F[200];
		
		PointStd.resize(NP + 1);
		memcpy(p_a[NP].X, bP.X, 3 * sizeof(double));
		NP++;
		POINT* P[100] = { nullptr };
		int C_count = 0;
		New_C[C_count++] = Cp;
		FACE* Fp = nullptr;

		// part 3 :
		for (int i = 0; i < 4; i++)
			if (C[0]->F[i]->is_in(Pe[0]) && C[0]->F[i]->is_in(Pe[1])) {
				Fp = C[0]->F[i];
				break;
			}

		// part 4 :
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < 3; j++)
				if (Fp->P[j] != Pe[0] && Fp->P[j] != Pe[1])
					P[i] = Fp->P[j];

			// part 5 :
			find(Cp, point);

			if (find(Fp, cell) == 2) {
				if (Cell[0] == Cp)
					swap(Cell[0], Cell[1]);
				for (int k = 0; k < 4; k++)
					if (Cell[0]->F[k]->is_in(Pe[0]) && Cell[0]->F[k]->is_in(Pe[1]) && Cell[0]->F[k] != Fp) {
						Fp = Cell[0]->F[k];
						Cp = Cell[0];
						New_C[C_count++] = Cp;
						break;
					}
			}
			else
				throw;

			//part 6 :
			F[2 * i].FaceAddPoint(&p_a[NP - 1]);
			for (int j = 0; j < 4; j++)
				if (Point[j] != Pe[0] && Point[j] != Pe[1])
					F[2 * i].FaceAddPoint(Point[j]);
			if (F[2 * i].FacePointSize != 3)
				throw;
			F[2 * i + 1].FaceAddPoint(&p_a[NP - 1]);
			F[2 * i + 1].FaceAddPoint(Pe[0]);
			F[2 * i + 1].FaceAddPoint(P[i]);

		}

		// part 7 :
		FaceStd.resize(NF + 2 * n);
		for (int i = 0; i < 2 * n; i++)
			f_a[NF + i] = F[i];

		int check_F = NF;
		NF = NF + 2 * n;

		// part 8 :
		for (int i = 0; i < n; i++)
			C[i]->RemoveCellFromPoints();


		// part 9 :
		CellStd.resize(NC + n);
		for (int i = 0; i < n; i++)
			for (int j = 0; j < 4; j++) {
				if (New_C[i]->F[j]->is_in(Pe[1]) == 0) {
					c_a[NC + i].CellAddFace(New_C[i]->F[j]);
					New_C[i]->F[j] = &f_a[check_F + 2 * i];
					break;
				}
			}

		// part 10 :
		for (int i = 0; i < n; i++)
			for (int j = 0; j < 4; j++)
				for (int k = 0; k < 3; k++)
					if (New_C[i]->F[j]->P[k] == Pe[0])
						New_C[i]->F[j]->P[k] = &p_a[NP - 1];


		// part 11 :			 
		for (int i = 0; i < n; i++)
			C[i]->AddCellToPoints();


		// part 12 :
		c_a[NC].CellAddFace(&f_a[check_F]);
		c_a[NC].CellAddFace(&f_a[check_F + 1]);
		c_a[NC].CellAddFace(&f_a[NF - 1]);

		c_a[NC].AddCellToPoints();
		NC++;

		// part 13 :
		for (int i = 1; i < n; i++) {
			c_a[NC].CellAddFace(&f_a[check_F + 2 * i]);
			c_a[NC].CellAddFace(&f_a[check_F + 2 * i - 1]);
			c_a[NC].CellAddFace(&f_a[check_F + 2 * i + 1]);
			c_a[NC].AddCellToPoints();
			NC++;
		}

	}



}

int MESH::delauny(FACE* Fp) {
	//cout << A2N(Fp) << endl;
	int state = delcheck(Fp);
	if (state == 0) {
		if (flip23(Fp) == 1) {

			return 23;
		}


		else {
			EDGE E[3] = { { Fp->P[0],Fp->P[1] },{ Fp->P[1],Fp->P[2] },{ Fp->P[2],Fp->P[0] } };
			CELL* CE[3][100];
			int C_count[3];


			for (int i = 0; i < 3; i++) {

				C_count[i] = find(&E[i], cell);
				for (int j = 0; j < C_count[i]; j++)
					CE[i][j] = Cell[j];


				if (C_count[i] == 3) {
					if (CE[i][0]->CType == CELL::tetra && CE[i][1]->CType == CELL::tetra && CE[i][2]->CType == CELL::tetra && E[i].IsOnBound() == 0) {
						double a[5] = { CE[i][0]->volume(),CE[i][1]->volume(),CE[i][2]->volume(),0,0 };
						int m = 2;
						POINT* P[5] = { E[i].P[0],E[i].P[1],nullptr,nullptr,nullptr };

						find(CE[i][0], point);
						for (int k = 0; k < 4; k++)
							if (Point[k] != P[0] && Point[k] != P[1])
								P[m++] = Point[k];

						if (m != 4)
							throw;
						find(CE[i][1], point);
						for (int k = 0; k < 4; k++)
							if (Point[k] != P[0] && Point[k] != P[1] && Point[k] != P[2] && Point[k] != P[3])
								P[m++] = Point[k];

						if (m != 5)
							throw;
						POINT* p[4] = { P[0],P[2],P[3],P[4] };
						a[3] = volume_(p);
						p[0] = P[1];
						a[4] = volume_(p);
						double A = a[0] + a[1] + a[2];
						double B = a[3] + a[4];

						if (abs((A - B) / A) < ERRORE1  && a[3] / A > ERRORE2 && a[4] / A > ERRORE2) {
							{
								FACE* F[9];
								int n = 0;



								for (int k = 0; k < 4; k++)
									if (CE[i][0]->F[k]->is_in(P[0]) && CE[i][0]->F[k]->is_in(P[1]))
										F[n++] = CE[i][0]->F[k];
								if (n != 2)
									throw;
								for (int k = 0; k < 4; k++)
									if (CE[i][1]->F[k]->is_in(P[0]) && CE[i][1]->F[k]->is_in(P[1]) && CE[i][1]->F[k] != F[0] && CE[i][1]->F[k] != F[1])
										F[n++] = CE[i][1]->F[k];


								if (n != 3)
									throw;
								for (int k = 0; k < 3; k++)
									for (int j = 0; j < 4; j++)
										if (CE[i][k]->F[j] != F[0] && CE[i][k]->F[j] != F[1] && CE[i][k]->F[j] != F[2])
											F[n++] = CE[i][k]->F[j];
								if (n != 9)
									throw;
								for (int k = 3; k < 9; k = k + 2)
									if (F[k]->is_in(P[0]))
										swap(F[k], F[k + 1]);


								CE[i][0]->RemoveCellFromPoints();
								CE[i][1]->RemoveCellFromPoints();
								CE[i][2]->RemoveCellFromPoints();
								CE[i][0]->CellRemFace();
								CE[i][1]->CellRemFace();
								CE[i][2]->CellRemFace();

								F[0]->FaceRemPoint();
								F[0]->FaceAddPoint(P[2]);
								F[0]->FaceAddPoint(P[3]);
								F[0]->FaceAddPoint(P[4]);


								CE[i][0]->CellAddFace(F[0]);
								CE[i][0]->CellAddFace(F[3]);
								CE[i][0]->CellAddFace(F[5]);
								CE[i][0]->CellAddFace(F[7]);

								CE[i][1]->CellAddFace(F[0]);
								CE[i][1]->CellAddFace(F[4]);
								CE[i][1]->CellAddFace(F[6]);
								CE[i][1]->CellAddFace(F[8]);

								CE[i][0]->AddCellToPoints();
								CE[i][1]->AddCellToPoints();

								D.push_back(CE[i][2]);
								D.push_back(F[1]);
								D.push_back(F[2]);

							}

							clear();


							return 32;
						}

					}

				}
				if (C_count[i] == 2) {
					double vol;
					if (CE[i][0]->CType == CELL::tetra && CE[i][1]->CType == CELL::tetra) {
						FACE* s[2] = { nullptr };
						for (int k = 0; k < 4; k++)
							if (CE[i][0]->F[k]->state() == 0)
								s[0] = CE[i][0]->F[k];
						for (int k = 0; k < 4; k++)
							if (CE[i][1]->F[k]->state() == 0)
								s[1] = CE[i][1]->F[k];
						POINT* p[2] = { nullptr };
						s[0]->join(s[1], p);
						POINT* pv[4] = { nullptr };
						pv[0] = p[0];
						pv[1] = p[1];

						for (int k = 0; k < 3; k++)
							if (s[0]->P[k] != p[0] && s[0]->P[k] != p[1])
								pv[2] = s[0]->P[k];
						for (int k = 0; k < 3; k++)
							if (s[1]->P[k] != p[0] && s[1]->P[k] != p[1])
								pv[3] = s[1]->P[k];
						vol = volume_(pv);
						
					}

					if (CE[i][0]->CType == CELL::tetra && CE[i][1]->CType == CELL::tetra && E[i].IsOnBound() == 1 && vol==0) {
						POINT* P[5] = { E[i].P[0],E[i].P[1],nullptr,nullptr,nullptr };
						double a[4];
						find(CE[i][0], point);
						int m = 2;
						FACE* Fp = nullptr;
						for (int k = 0; k < 4; k++)
							for (int j = 0; j < 4; j++)
								if (CE[i][0]->F[k] == CE[i][1]->F[j])
									Fp = CE[i][0]->F[k];
						for (int k = 0; k < 4; k++)
							if (Fp->is_in(Point[k]) == 0)
								P[m++] = Point[k];
						if (m != 3)
							throw;
						find(CE[i][1], point);
						for (int k = 0; k < 4; k++)
							if (Fp->is_in(Point[k]) == 0)
								P[m++] = Point[k];
						if (m != 4)
							throw;
						for (int k = 0; k < 3; k++)
							if (Fp->P[k] != P[0] && Fp->P[k] != P[1])
								P[4] = Fp->P[k];
						a[0] = CE[i][0]->volume();
						a[1] = CE[i][1]->volume();
						POINT* p[4] = { P[0],P[2],P[3],P[4] };
						a[2] = volume_(p);
						p[0] = P[1];
						a[3] = volume_(p);
						double A = a[0] + a[1];
						double B = a[2] + a[3];
						if (abs((A - B) / A) < ERRORE1 && a[2] / A > ERRORE2 && a[3] / A > ERRORE2) {
							{

								POINT* P[5] = { E[i].P[0],E[i].P[1],nullptr,nullptr,nullptr };
								FACE* F[6];
								FACE* Fp = nullptr;
								for (int k = 0; k < 4; k++)
									for (int j = 0; j < 4; j++)
										if (CE[i][0]->F[k] == CE[i][1]->F[j]) {
											Fp = CE[i][0]->F[k];
											break;
										}
								find(CE[i][0], point);
								for (int k = 0; k < 4; k++)
									if (Fp->is_in(Point[k]) == 0)
										P[2] = Point[k];

								find(CE[i][1], point);
								for (int k = 0; k < 4; k++)
									if (Fp->is_in(Point[k]) == 0)
										P[3] = Point[k];

								for (int k = 0; k < 3; k++)
									if (Fp->P[k] != P[0] && Fp->P[k] != P[1])
										P[4] = Fp->P[k];
								int m = 0;
								for (int k = 0; k < 4; k++)
									if (CE[i][0]->F[k] != Fp)
										F[m++] = CE[i][0]->F[k];
								if (m != 3)
									throw;
								for (int k = 0; k < 4; k++)
									if (CE[i][1]->F[k] != Fp)
										F[m++] = CE[i][1]->F[k];
								if (m != 6)
									throw;

								for (int k = 0; k<3; k++)
									if (F[k]->is_in(P[0]) == 1 && F[k]->is_in(P[1]) == 1) {
										swap(F[k], F[0]);
										break;
									}
								for (int k = 3; k<6; k++)
									if (F[k]->is_in(P[0]) == 1 && F[k]->is_in(P[1]) == 1) {
										swap(F[k], F[3]);
										break;
									}
								if (F[1]->is_in(P[0]) != 1)
									swap(F[1], F[2]);
								if (F[4]->is_in(P[0]) != 1)
									swap(F[4], F[5]);


								CE[i][0]->RemoveCellFromPoints();
								CE[i][1]->RemoveCellFromPoints();
								CE[i][0]->CellRemFace();
								CE[i][1]->CellRemFace();
								Fp->FaceRemPoint();
								F[0]->FaceRemPoint();
								F[3]->FaceRemPoint();

								Fp->FaceAddPoint(P[2]);
								Fp->FaceAddPoint(P[3]);
								Fp->FaceAddPoint(P[4]);

								F[0]->FaceAddPoint(P[2]);
								F[0]->FaceAddPoint(P[3]);
								F[0]->FaceAddPoint(P[0]);

								F[3]->FaceAddPoint(P[2]);
								F[3]->FaceAddPoint(P[3]);
								F[3]->FaceAddPoint(P[1]);

								CE[i][0]->CellAddFace(Fp);
								CE[i][0]->CellAddFace(F[0]);
								CE[i][0]->CellAddFace(F[1]);
								CE[i][0]->CellAddFace(F[4]);

								CE[i][1]->CellAddFace(Fp);
								CE[i][1]->CellAddFace(F[3]);
								CE[i][1]->CellAddFace(F[2]);
								CE[i][1]->CellAddFace(F[5]);

								CE[i][0]->AddCellToPoints();
								CE[i][1]->AddCellToPoints();



							}

							return 22;
						}

					}

				}
				if (C_count[i] == 4) {
					int t = 3;
					if (CE[i][0]->CType == CELL::tetra && CE[i][1]->CType == CELL::tetra && CE[i][2]->CType == CELL::tetra && CE[i][3]->CType == CELL::tetra && E[i].IsOnBound() == 0) {
						POINT* P[6] = { E[i].P[0],E[i].P[1],nullptr,nullptr,nullptr,nullptr };
						double a[12];
						int m = 2;

						a[0] = CE[i][0]->volume();
						a[1] = CE[i][1]->volume();
						a[2] = CE[i][2]->volume();
						a[3] = CE[i][3]->volume();
						find(CE[i][0], point);
						for (int k = 0; k < 4; k++)
							if (Point[k] != P[0] && Point[k] != P[1])
								P[m++] = Point[k];
						if (m != 4)
							throw;

						find(CE[i][1], point);
						if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
							swap(CE[i][1], CE[i][2]);
							find(CE[i][1], point);
						}
						if (P[3] != Point[0] && P[3] != Point[1] && P[3] != Point[2] && P[3] != Point[3]) {
							swap(CE[i][1], CE[i][3]);
							find(CE[i][1], point);
						}

						for (int k = 0; k < 4; k++)
							if (Point[k] != P[0] && Point[k] != P[1] && Point[k] != P[2] && Point[k] != P[3])
								P[m++] = Point[k];
						if (m != 5)
							throw;
						find(CE[i][2], point);
						if (P[4] != Point[0] && P[4] != Point[1] && P[4] != Point[2] && P[4] != Point[3]) {
							swap(CE[i][2], CE[i][3]);
							find(CE[i][2], point);
						}
						for (int k = 0; k < 4; k++)
							if (Point[k] != P[0] && Point[k] != P[1] && Point[k] != P[2] && Point[k] != P[3] && Point[k] != P[4])
								P[m++] = Point[k];

						if (m != 6) {
							throw;
						}



						POINT* p[4] = { P[0],P[2],P[3],P[4] };
						a[4] = volume_(p);
						p[0] = P[1];
						a[5] = volume_(p);
						p[2] = P[5];
						a[6] = volume_(p);
						p[0] = P[0];
						a[7] = volume_(p);
						p[3] = P[3];
						a[8] = volume_(p);
						p[0] = P[1];
						a[9] = volume_(p);
						p[1] = P[4];
						a[10] = volume_(p);
						p[0] = P[0];
						a[11] = volume_(p);
						double A = a[0] + a[1] + a[2] + a[3];
						double B = a[4] + a[5] + a[6] + a[7];
						double D = a[8] + a[9] + a[10] + a[11];
						//cout << A << "\t" << B << "\t" << D << endl;
						t = 0;
						if (abs((A - B) / A) < ERRORE1 && a[4] / A > ERRORE2 && a[5] / A > ERRORE2 && a[6] / A > ERRORE2 && a[7] / A > ERRORE2)
							t = 1;
						if (abs((A - D) / A) < ERRORE1 && a[8] / A > ERRORE2 && a[9] / A > ERRORE2 && a[10] / A > ERRORE2 && a[11] / A > ERRORE2)
							t = 2;
						if (abs((A - B) / A) < ERRORE1 && a[4] / A > ERRORE2 && a[5] / A > ERRORE2 && a[6] / A > ERRORE2 && a[7] / A > ERRORE2 && abs((A - D) / A) < ERRORE1 && a[8] / A > ERRORE2 && a[9] / A > ERRORE2 && a[10] / A > ERRORE2 && a[11] / A > ERRORE2)
							t = 4;
						{


							if (t == 1 || t == 4) {


								FACE* F[12];


								int f_counter = 0;
								for (int k = 0; k < 4; k++)
									if (CE[i][0]->F[k]->is_in(P[0]) && CE[i][0]->F[k]->is_in(P[1]))
										F[f_counter++] = CE[i][0]->F[k];
								for (int k = 0; k < 4; k++)
									if (CE[i][2]->F[k]->is_in(P[0]) && CE[i][2]->F[k]->is_in(P[1]))
										F[f_counter++] = CE[i][2]->F[k];
								if (f_counter != 4)
									throw;
								for (int k = 0; k < 4; k++)
									for (int j = 0; j < 4; j++)
										if (CE[i][k]->F[j] != F[0] && CE[i][k]->F[j] != F[1] && CE[i][k]->F[j] != F[2] && CE[i][k]->F[j] != F[3])
											F[f_counter++] = CE[i][k]->F[j];
								if (f_counter != 12)
									throw;

								for (int k = 4; k < 11; k = k + 2)
									if (F[k]->is_in(P[0]) != 1)
										swap(F[k], F[k + 1]);

								for (int k = 0; k < 4; k++)
									CE[i][k]->RemoveCellFromPoints();
								for (int k = 0; k < 4; k++)
									CE[i][k]->CellRemFace();
								for (int k = 0; k < 4; k++)
									F[k]->FaceRemPoint();

								F[0]->FaceAddPoint(P[2]);
								F[0]->FaceAddPoint(P[0]);
								F[0]->FaceAddPoint(P[4]);

								F[1]->FaceAddPoint(P[2]);
								F[1]->FaceAddPoint(P[3]);
								F[1]->FaceAddPoint(P[4]);

								F[2]->FaceAddPoint(P[2]);
								F[2]->FaceAddPoint(P[1]);
								F[2]->FaceAddPoint(P[4]);

								F[3]->FaceAddPoint(P[2]);
								F[3]->FaceAddPoint(P[5]);
								F[3]->FaceAddPoint(P[4]);

								CE[i][0]->CellAddFace(F[0]);
								CE[i][0]->CellAddFace(F[1]);
								CE[i][0]->CellAddFace(F[4]);
								CE[i][0]->CellAddFace(F[6]);

								CE[i][1]->CellAddFace(F[1]);
								CE[i][1]->CellAddFace(F[2]);
								CE[i][1]->CellAddFace(F[5]);
								CE[i][1]->CellAddFace(F[7]);

								CE[i][2]->CellAddFace(F[2]);
								CE[i][2]->CellAddFace(F[3]);
								CE[i][2]->CellAddFace(F[9]);
								CE[i][2]->CellAddFace(F[11]);

								CE[i][3]->CellAddFace(F[3]);
								CE[i][3]->CellAddFace(F[0]);
								CE[i][3]->CellAddFace(F[8]);
								CE[i][3]->CellAddFace(F[10]);

								for (int k = 0; k < 4; k++)
									CE[i][k]->AddCellToPoints();


								return 44;
							}
							if (t == 2) {


								FACE* F[12];


								int f_counter = 0;
								for (int k = 0; k < 4; k++)
									if (CE[i][0]->F[k]->is_in(P[0]) && CE[i][0]->F[k]->is_in(P[1]))
										F[f_counter++] = CE[i][0]->F[k];
								for (int k = 0; k < 4; k++)
									if (CE[i][2]->F[k]->is_in(P[0]) && CE[i][2]->F[k]->is_in(P[1]))
										F[f_counter++] = CE[i][2]->F[k];
								if (f_counter != 4)
									throw;
								for (int k = 0; k < 4; k++)
									for (int j = 0; j < 4; j++)
										if (CE[i][k]->F[j] != F[0] && CE[i][k]->F[j] != F[1] && CE[i][k]->F[j] != F[2] && CE[i][k]->F[j] != F[3])
											F[f_counter++] = CE[i][k]->F[j];
								if (f_counter != 12)
									throw;

								for (int k = 4; k < 11; k = k + 2)
									if (F[k]->is_in(P[0]) != 1)
										swap(F[k], F[k + 1]);

								for (int k = 0; k < 4; k++)
									CE[i][k]->RemoveCellFromPoints();
								for (int k = 0; k < 4; k++)
									CE[i][k]->CellRemFace();
								for (int k = 0; k < 4; k++)
									F[k]->FaceRemPoint();

								F[0]->FaceAddPoint(P[3]);
								F[0]->FaceAddPoint(P[0]);
								F[0]->FaceAddPoint(P[5]);

								F[1]->FaceAddPoint(P[3]);
								F[1]->FaceAddPoint(P[4]);
								F[1]->FaceAddPoint(P[5]);

								F[2]->FaceAddPoint(P[3]);
								F[2]->FaceAddPoint(P[1]);
								F[2]->FaceAddPoint(P[5]);

								F[3]->FaceAddPoint(P[3]);
								F[3]->FaceAddPoint(P[2]);
								F[3]->FaceAddPoint(P[5]);

								CE[i][0]->CellAddFace(F[0]);
								CE[i][0]->CellAddFace(F[1]);
								CE[i][0]->CellAddFace(F[6]);
								CE[i][0]->CellAddFace(F[8]);

								CE[i][1]->CellAddFace(F[1]);
								CE[i][1]->CellAddFace(F[2]);
								CE[i][1]->CellAddFace(F[7]);
								CE[i][1]->CellAddFace(F[9]);

								CE[i][2]->CellAddFace(F[2]);
								CE[i][2]->CellAddFace(F[3]);
								CE[i][2]->CellAddFace(F[5]);
								CE[i][2]->CellAddFace(F[11]);

								CE[i][3]->CellAddFace(F[3]);
								CE[i][3]->CellAddFace(F[0]);
								CE[i][3]->CellAddFace(F[4]);
								CE[i][3]->CellAddFace(F[10]);

								for (int k = 0; k < 4; k++)
									CE[i][k]->AddCellToPoints();


								return 44;
							}
						}
					}


				}
			}
			return 0;
		}

	}
	if (state == 1) {
		return 1;
	}
	if (state == 3) {
		return -1;
	}
}
bool MESH::dlt(POINT* Pp) {

	// part 1 :
	if (Pp->IsOnBound == 1) {

		return 0;
	}
	POINT* p_a = PointStd.data();
	FACE* f_a = FaceStd.data();
	CELL* c_a = CellStd.data();
	POINT* Ptest=nullptr;
	int zeroC = 0;
	int F_counter = 0;
	FACE* F[4 * MAX_NEIB];
	CELL* dC[MAX_NEIB] = { nullptr };
	double a[MAX_NEIB] = { 0 };
	double an[MAX_NEIB] = { 0 };
	double A, B;
	bPOINT Pold;
	A = 0;
	B = 0;

	// part 2 :
	for (int j = 0; j < Pp->PointCellSize; j++) {
		if (Pp->PointCells[j]->CType != 0) {

			return 0;
		}
		a[j] = Pp->PointCells[j]->volume();
		A = A + a[j];
	}
	
	//part 3 :
	for (int j = 0; j < Pp->PointCellSize; j++) {
		
		B = 0;
		zeroC = 0;
		int n = find(Pp->PointCells[j], point);
		for(int i=0;i<4;i++)
			if (Point[i] != Pp) {
				Ptest = Point[i];
				
				break;
			}

        // part 4 :
		memcpy(Pold.X, Pp->X, 3 * sizeof(double));
		EDGE Etest{ Pp,Ptest };
		int m = find(&Etest, cell);
		memcpy(dC, Cell, m * sizeof(CELL*));
		memcpy(Pp->X, Ptest->X, 3 * sizeof(double));
		
		// part 5 :
		for (int k = 0; k < Pp->PointCellSize; k++) {
			if (Pp->PointCells[k]->CType != 0) {
				memcpy(Pp->X, Pold.X, 3 * sizeof(double));
				goto label;
			}
			an[k] = Pp->PointCells[k]->volume();
			B = B + an[k];
		}

		// part 6 :
		if (abs(A - B) / A < ERRORE1) {
			for (int k = 0; k < m; k++)
				if (dC[k]->volume() / A > ERRORE2) {

					memcpy(Pp->X, Pold.X, 3 * sizeof(double));
					
					goto label;
				}

            // part 7 :
			for (int k = 0; k < Pp->PointCellSize; k++) {
				if (Pp->PointCells[k]->volume() / A < ERRORE2)
					zeroC++;
			}
			if (zeroC != m) {
				memcpy(Pp->X, Pold.X, 3 * sizeof(double));

				goto label;
			}
				
		}
		else {
			memcpy(Pp->X, Pold.X, 3 * sizeof(double));

			goto label;
		}

		// part 8 :
		for (int i = 0; i < m; i++)
			dC[i]->RemoveCellFromPoints();


		// part 9 :
		for (int i = 0; i < m; i++) {
			for (int k = 4 * i; k < 4 * i + 4; k++) {
				F[k] = dC[i]->F[k - 4 * i];
			}
			for (int k = 4 * i; k < 4 * i + 4; k++) {
				if (F[k]->area() != 0) {
					swap(F[k], F[4 * i]);
					break;
				}
			}
			for (int k = 4 * i+1; k < 4 * i + 4; k++) {
				if (F[k]->area() != 0) {
					swap(F[k], F[4 * i + 1]);
					break;
				}
			}
			if (F[4 * i]->is_in(Pp) == 1) {
				swap(F[4 * i], F[4 * i + 1]);

			}

			if (F[4 * i]->is_in(Pp) == 1)
				throw;


			// part 10 :

			find(F[4 * i+1], cell);
			for (int k = 0; k < 4; k++) {
				if (Cell[0]->F[k] == F[4 * i + 1])
					Cell[0]->F[k] = F[4 * i];
			}
			
		}

		// part 11 :
		for (int i = 0; i < Pp->PointCellSize; i++) {
			Ptest->PointAddCell(Pp->PointCells[i]);
		}

		// part 12 :
		for (int i = 0; i < Pp->PointCellSize; i++) {
			for(int k=0;k<4;k++)
				for(int l=0;l<3;l++)
					if (Pp->PointCells[i]->F[k]->P[l] == Pp) {
						Pp->PointCells[i]->F[k]->P[l] = Ptest;
					}
		}
		
		// part 13 :
		for (int i = 0; i < m; i++)
			D.push_back(dC[i]);
		for (int i = 0; i < m; i++)
			for (int k = 4 * i + 1; k < 4 * i + 4; k++) {
				D.push_back(F[k]);
			}

        // part 14 :
		for (int i = 0; i < D.size(); i++)
			for (int k = i + 1; k < D.size(); k++)
				if (D[i] == D[k]) {
					D.erase(D.begin() + k);
				}
		
		// part 15 :
		D.push_back(Pp);
		return 1;
	label:;
		
		
	}
		
}

void MESH::analyse() {

	// part 1 :
	ZNum = 1;

	// part 2 :
	for (int i = 0; i < NC; i++)
		CellStd[i].CellZNum = 0;

	// part 3 :
	for (int j = 0; j < NC; j++) {
		int n = 0;
		int t;
		if (CellStd[j].CellZNum == 0) {
			CellStd[j].CellZNum = ZNum;
			C_stack[n++] = &CellStd[j];

			// part 4 :
			while (n != 0) {
				C_stack[0]->CellZNum = ZNum;

				// part 5 :
				for (int i = 0; i < C_stack[0]->CellFaceSize; i++) {
					if (C_stack[0]->F[i]->state() == 1) {
						t = find(C_stack[0]->F[i], cell);
						if (Cell[0] != C_stack[0] && Cell[0]->CellZNum == 0)
							C_stack[n++] = Cell[0];
						if (Cell[1] != C_stack[0] && Cell[1]->CellZNum == 0 && t == 2)
							C_stack[n++] = Cell[1];
					}
				}

				// part 6 :
				swap(C_stack[n - 1], C_stack[0]);
				C_stack[n - 1] = nullptr;
				n--;
			}

			// part 7 :
			ZNum++;
		}
	}
}
void MESH::fill_zone(vector<ZONE>& V) {

	// part 1 :
	V.clear();
	V.resize(ZNum - 1);
	set<FACE*> F;
	set<POINT*> P;

	// part 2 :
	for (int i = 0; i < NC; i++) {
		V[CellStd[i].CellZNum - 1].ZCells.push_back(&CellStd[i]);
	}

	// part 3 :
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V[i].ZCells.size(); j++)
			for (int k = 0; k < V[i].ZCells[j]->CellFaceSize; k++)
				F.emplace(V[i].ZCells[j]->F[k]);
		copy(F.begin(), F.end(), back_inserter(V[i].ZFace));
		F.clear();
	}

	// part 4 :
	int m;
	for (int i = 0; i < V.size(); i++) {
		for (int j = 0; j < V[i].ZCells.size(); j++) {
			m = find(V[i].ZCells[j], point);
			for (int k = 0; k < m; k++)
				P.emplace(Point[k]);
		}
		copy(P.begin(), P.end(), back_inserter(V[i].ZPoint));
		P.clear();
	}

	// part 5 :
	for (int i = 0; i < V.size(); i++)
		for (int j = 0; j < V[i].ZFace.size(); j++) {
			if (V[i].ZFace[j]->state() == 0)
				V[i].boundary.push_back(V[i].ZFace[j]);
			if (V[i].ZFace[j]->state() == 2) {
				V[i].interface.push_back(V[i].ZFace[j]);
				V[i].ZFace[j]->IsOnInterface = 1;
				V[i].ZFace[j]->P[0]->IsOnInterface = 1;
				V[i].ZFace[j]->P[1]->IsOnInterface = 1;
				V[i].ZFace[j]->P[2]->IsOnInterface = 1;
			}
		}

		// part 6 :
	for (int i = 0; i < V.size(); i++)
		if (V[i].ZCells[0]->CellFaceSize != 4)
			V[i].ZType = ZONE::hybrid;

		// part 7 :
	for (int i = 0; i < V.size(); i++) {
		cout << endl << "zone number: " << i + 1 << "\tzone type: " << V[i].ZType << endl;
		cout << "cell number: " << V[i].ZCells.size() << endl;
		cout << "face number: " << V[i].ZFace.size() << endl;
		cout << "point number: " << V[i].ZPoint.size() << endl;
	}
}
bool MESH::add(vector<CELL*>& layer) {
	//part 1:
	if (layer[0]->CType != CELL::prism)
		return false;

	// part 2:
	for (int i = 0; i < layer.size(); i++)
		for (int j = 0; j < layer[i]->CellFaceSize; j++)
			if (layer[i]->F[j]->FType == 4)
				layer[i]->F[j]->FaceIsLabl = 0;

	// part 3 :
	int NPold = NP;
	int NFold = NF;
	vector<POINT*> P[3];
	P[0].resize(layer.size(), nullptr);
	P[1].resize(layer.size(), nullptr);
	P[2].resize(layer.size(), nullptr);
	vector<FACE*> F[4];
	F[0].resize(layer.size(), nullptr);
	F[1].resize(layer.size(), nullptr);
	F[2].resize(layer.size(), nullptr);
	F[3].resize(layer.size(), nullptr);


	// part 4:
	PointStd.resize(NP + 3);
	bPOINT bP;
	bP = mid(layer[0]->F[0]->P[0], layer[0]->F[1]->P[0]);
	memcpy(PointStd[NP].X, bP.X, 3 * sizeof(double));
	P[0][0] = &PointStd[NP];
	bP = mid(layer[0]->F[0]->P[1], layer[0]->F[1]->P[1]);
	memcpy(PointStd[NP + 1].X, bP.X, 3 * sizeof(double));
	P[1][0] = &PointStd[NP + 1];
	bP = mid(layer[0]->F[0]->P[2], layer[0]->F[1]->P[2]);
	memcpy(PointStd[NP + 2].X, bP.X, 3 * sizeof(double));
	P[2][0] = &PointStd[NP + 2];
	NP = NP + 3;

	// part 5:
	FaceStd.push_back({ P[0][0],P[1][0],P[2][0] });
	F[0][0] = &FaceStd[NF];
	NF++;
	FaceStd.push_back({ layer[0]->F[0]->P[0],layer[0]->F[0]->P[1] ,P[1][0],P[0][0] });
	F[1][0] = &FaceStd[NF];
	NF++;
	FaceStd.push_back({ layer[0]->F[0]->P[1],layer[0]->F[0]->P[2] ,P[2][0],P[1][0] });
	F[2][0] = &FaceStd[NF];
	NF++;
	FaceStd.push_back({ layer[0]->F[0]->P[2],layer[0]->F[0]->P[0] ,P[0][0],P[2][0] });
	F[3][0] = &FaceStd[NF];
	NF++;
	
	layer[0]->F[2]->FaceIsLabl = 1;
	layer[0]->F[3]->FaceIsLabl = 1;
	layer[0]->F[4]->FaceIsLabl = 1;

	//part 6 :
	for (int i = 1; i < layer.size(); i++) {
		
		if (layer[i]->CType == CELL::prism) {
			EDGE E;
			//int labcount = 0;
			//int labnum[3] = { 0,0,0 };
			//for (int j = 2; j < 5; j++)
				//if (layer[i]->F[j]->FaceIsLabl == 1) 
				//	labnum[labcount++] = j;
			
			bPOINT test[3];
			int testnum[3] = { -1,-1,-1 };
			test[0] = mid(layer[i]->F[0]->P[0], layer[i]->F[1]->P[0]);
			test[1] = mid(layer[i]->F[0]->P[1], layer[i]->F[1]->P[1]);
			test[2] = mid(layer[i]->F[0]->P[2], layer[i]->F[1]->P[2]);
			bool flag[3] = { 0,0,0 };

			// part 7 :
			for (int k = 0; k < 3; k++)
				for (int j = NPold - 1; j < NP; j++)
					if (PointStd[j].tobPOINT() == test[k]) {
						flag[k] = 1;
						testnum[k] = j;
					}

			// part 8 :
			for(int k=0;k<3;k++)
				if (flag[k] == 0) {
					PointStd.resize(NP + 1);
					memcpy(PointStd[NP].X, test[k].X, 3 * sizeof(double));
					P[k][i] = &PointStd[NP];
					NP++;
				}
				else {
					P[k][i] = &PointStd[testnum[k]];
				}

			FaceStd.push_back({ P[0][i],P[1][i],P[2][i] });
			F[0][i] = &FaceStd[NF];
			NF++;

			// part 9 :
			flag[0] = 0;
			flag[1] = 0;
			flag[2] = 0;
			FACE ftest[3];
			int fnum[3] = { -1,-1,-1 };
			ftest[0] = { layer[i]->F[0]->P[0],layer[i]->F[0]->P[1],P[1][i],P[0][i] };
			ftest[1] = { layer[i]->F[0]->P[1],layer[i]->F[0]->P[2],P[2][i],P[1][i] };
			ftest[2] = { layer[i]->F[0]->P[2],layer[i]->F[0]->P[0],P[0][i],P[2][i] };

			// part 10 :
			for (int k = 0; k < 3; k++)
				for (int j = NFold - 1; j < NF; j++)
					if (FaceStd[j].is_joined(&ftest[k])==4) {
						flag[k] = 1;
						fnum[k] = j;
					}
			for (int k = 0; k < 3; k++)
				if (flag[k] == 0) {
					FaceStd.push_back(ftest[k]);
					F[k+1][i] = &FaceStd[NF];
					NF++;
				}
				else {
					F[k+1][i] = &FaceStd[fnum[k]];
				}
		
		}
		/*
		if (layer[i]->CType == CELL::pyramid) {
			for (int k = i - 1; k == 0; k--) {
				POINT* Pjunk[2] = { nullptr };
				for (int j = 2; j < 5; j++)
					if (layer[i]->F[0] == layer[k]->F[j]) {
						if (j == 2) {
							Pjunk[0] = P[0][k];
							Pjunk[1] = P[1][k];
						}
						if (j == 3) {
							Pjunk[0] = P[1][k];
							Pjunk[1] = P[2][k];
						}
						if (j == 4) {
							Pjunk[0] = P[2][k];
							Pjunk[1] = P[0][k];
						}
						P[0][i] = Pjunk[0];
						P[1][i] = Pjunk[1];
						find(layer[i], point);
						P[2][i] = Point[4];
							
						k = 0;
						break;

					}
			}
			FaceStd.push_back({ P[0][i],P[1][i],P[2][i] });
			NF++;

		}
		if (layer[i]->CType == CELL::tetra) {
			int labcount = 0;
			int labnum[2] = { 0,0 };
			for (int j = 0; j < 4; j++)
				if (layer[i]->F[j]->FaceIsLabl == 1)
					labnum[labcount++] = j;
			if (labcount == 0)
				throw;
			find(layer[i]->F[labnum[0]], cell);
			if (Cell[0] == layer[i])
				swap(Cell[0], Cell[1]);
			int n = -1;
			for (int k = i - 1; k == 0; k--) {
				if (Cell[0] == layer[k] && layer[k]->CType == CELL::pyramid) {
					n = k;
					break;
				}
			}
			if (n == -1)
				throw;
			EDGE Etest = { layer[i]->F[labnum[0]]->P[0],layer[i]->F[labnum[0]]->P[1] };
			bP = Etest.mid();
			if (bP.X[0] == P[0][n]->X[0] && bP.X[1] == P[0][n]->X[1] && bP.X[2] == P[0][n]->X[2]) {
				P[0][i] = P[0][n];
			}
			else if (bP.X[0] == P[1][n]->X[0] && bP.X[1] == P[1][n]->X[1] && bP.X[2] == P[1][n]->X[2]) {
				P[0][i] = P[1][n];
			}
			else
				throw;
			find(layer[i], point);
			for (int k = 0; k < 4; k++)
				if (Point[k] != layer[i]->F[labnum[0]]->P[0] && Point[k] != layer[i]->F[labnum[0]]->P[1]) {
					P[1][i] = Point[k];
					break;
				}
			for (int k = 0; k < 4; k++)
				if (Point[k] != layer[i]->F[labnum[0]]->P[0] && Point[k] != layer[i]->F[labnum[0]]->P[1] && Point[k] != P[1][i]) {
					P[2][i] = Point[k];
					break;
				}
			FaceStd.push_back({ P[0][i],P[1][i],P[2][i] });
			NF++;
		}*/
		
		
		
	}
	
	// part 11 :
	for (int i = 0; i < layer.size(); i++)
		layer[i]->RemoveCellFromPoints();

	// part 12 :
	for(int i=0;i<layer.size();i++)
		if (layer[i]->CType == CELL::prism) {
			CELL Cn;
			Cn.CellAddFace(layer[i]->F[0]);
			Cn.CellAddFace(F[0][i]);
			Cn.CellAddFace(F[1][i]);
			Cn.CellAddFace(F[2][i]);
			Cn.CellAddFace(F[3][i]);
			Cn.CType = CELL::prism;
			Cn.CellZNum = layer[i]->CellZNum;
			CellStd.push_back(Cn);
			NC++;
			layer[i]->F[0] = F[0][i];
			*(layer[i]->F[2]) = { P[0][i],P[1][i],layer[i]->F[1]->P[1],layer[i]->F[1]->P[0] };
			*(layer[i]->F[3]) = { P[1][i],P[2][i],layer[i]->F[1]->P[2],layer[i]->F[1]->P[1] };
			*(layer[i]->F[4]) = { P[2][i],P[0][i],layer[i]->F[1]->P[0],layer[i]->F[1]->P[2] };
		}

	// part 13:
	for (int i = 0; i < layer.size(); i++)
		if (P[0][i] == P[1][i] || P[1][i] == P[2][i] || P[2][i] == P[0][i])
			throw;
	for (int i = 0; i < layer.size();i++) {
		CellStd[NC - i - 1].AddCellToPoints();
		layer[i]->AddCellToPoints();
	}

	// part 14 :
	for (int i = 0; i < NF; i++)
		if (FaceStd[i].state() == 0) {
			FaceStd[i].IsOnBound = 1;
			for (int j = 0; j < FaceStd[i].FacePointSize; j++)
				FaceStd[i].P[j]->IsOnBound = 1;
		}
return 0;

}


bool MESH::extend(CELL* C, vector<CELL*>& layer) {

	// part 1 :
	layer.clear();
	if (C->CType == CELL::prism) {
		int n = 0;
		C_stack[n++] = C;
		C_stack[0]->CellIsLabl = 1;

		// part 2 :
		while (n != 0) {
			if (C_stack[0]->CType == CELL::prism) {
				for (int i = 2; i < 5; i++) {
					if (find(C_stack[0]->F[i], cell) == 2 ) {
						if (Cell[0] == C_stack[0])
							swap(Cell[0], Cell[1]);

						// part 3 :
						if ((Cell[0]->CType == CELL::prism || Cell[0]->CType == CELL::pyramid) && Cell[0]->CellIsLabl==0) {
							Cell[0]->CellIsLabl = 1;
							C_stack[n++] = Cell[0];
							
						}
					}
				}
			}
			//part 4:
			if (C_stack[0]->CType == CELL::pyramid) {
				find(C_stack[0]->F[0], cell);
				if (Cell[0]->CType == CELL::pyramid)
					swap(Cell[0], Cell[1]);
				EDGE E[3];
				E[0] = { Cell[0]->F[0]->P[0],Cell[0]->F[1]->P[0] };
				E[1] = { Cell[0]->F[0]->P[1],Cell[0]->F[1]->P[1] };
				E[2] = { Cell[0]->F[0]->P[2],Cell[0]->F[1]->P[2] };


				int count = find(&E[0], cell);
				for (int i = 0; i < count; i++)
					if (Cell[i]->CType == CELL::tetra && Cell[i]->CellIsLabl == 0) {
						Cell[i]->CellIsLabl = 1;
						C_stack[n++] = Cell[i];
					}
				count = find(&E[1], cell);
				for (int i = 0; i < count; i++)
					if (Cell[i]->CType == CELL::tetra && Cell[i]->CellIsLabl == 0) {
						Cell[i]->CellIsLabl = 1;
						C_stack[n++] = Cell[i];
					}
				count = find(&E[2], cell);
				for (int i = 0; i < count; i++)
					if (Cell[i]->CType == CELL::tetra && Cell[i]->CellIsLabl == 0) {
						Cell[i]->CellIsLabl = 1;
						C_stack[n++] = Cell[i];
					}
			}

			// part 5 :
			layer.push_back(C_stack[0]);
			C_stack[0] = C_stack[n - 1];
			C_stack[n - 1] = nullptr;
			n--;

		}

		// part 6 :
		for (int i = 0; i < layer.size(); i++)
			layer[i]->CellIsLabl = 0;
		return true;

	}
	return false;
}


void MESH::Cell_order() {
	for (int i = 0; i < mesh.NC; i++) {
		if (CellStd[i].CType == CELL::prism) {
			CELL* C = &CellStd[i];

			if (C->F[2]->is_in(C->F[0]->P[2]) == 1)
				C->F[0]->push();
			if (C->F[2]->is_in(C->F[0]->P[2]) == 1)
				C->F[0]->push();
			if (C->F[2]->is_in(C->F[0]->P[2]) == 1)
				throw;

			if (C->F[2]->is_in(C->F[1]->P[2]) == 1)
				C->F[1]->push();
			if (C->F[2]->is_in(C->F[1]->P[2]) == 1)
				C->F[1]->push();
			if (C->F[2]->is_in(C->F[1]->P[2]) == 1)
				throw;

			if (C->F[3]->is_in(C->F[0]->P[0]) == 1)
				swap(C->F[0]->P[0], C->F[0]->P[1]);
			if (C->F[3]->is_in(C->F[1]->P[0]) == 1)
				swap(C->F[1]->P[0], C->F[1]->P[1]);

			if (C->F[2]->is_in(C->F[0]->P[2]) == 1)
				throw;
			if (C->F[2]->is_in(C->F[1]->P[2]) == 1)
				throw;
			if (C->F[3]->is_in(C->F[0]->P[0]) == 1)
				throw;
			if (C->F[3]->is_in(C->F[1]->P[0]) == 1)
				throw;

		}
		if (CellStd[i].CType == CELL::cube) {
			CELL* C = &CellStd[i];
			POINT* Ptest[2] = { nullptr };

			C->F[0]->join(C->F[2], Ptest);
			while (!((C->F[0]->P[0] == Ptest[0] && C->F[0]->P[1] == Ptest[1]) || (C->F[0]->P[1] == Ptest[0] && C->F[0]->P[0] == Ptest[1])))
				C->F[0]->push();
			C->F[1]->join(C->F[2], Ptest);
			while (!((C->F[1]->P[0] == Ptest[0] && C->F[1]->P[1] == Ptest[1]) || (C->F[1]->P[1] == Ptest[0] && C->F[1]->P[0] == Ptest[1])))
				C->F[1]->push();


			C->F[0]->join(C->F[5], Ptest);
			if ((Ptest[0] == C->F[0]->P[1] && Ptest[1] == C->F[0]->P[2]) || (Ptest[1] == C->F[0]->P[1] && Ptest[0] == C->F[0]->P[2]))
				swap(C->F[4], C->F[5]);

			C->F[1]->join(C->F[5], Ptest);
			if ((Ptest[0] == C->F[1]->P[1] && Ptest[1] == C->F[1]->P[2]) || (Ptest[1] == C->F[1]->P[1] && Ptest[0] == C->F[1]->P[2])) {
				swap(C->F[1]->P[0], C->F[1]->P[1]);
				swap(C->F[1]->P[2], C->F[1]->P[3]);
			}

		}

	}
}




void MESH::writeplt(const char* file_name) {
	ofstream file;
	file.open(file_name);
	if (file.is_open() == 1) {
		file << "Variables=" << "\"X\"" << "," << " \"Y\" " << "," << " \"Z\" " << endl;
		file << "Zone" << " " << "T='Itr'  " << " C=RED  " << "N=" << " " << NP << "  E=" << "  " << NC << endl;
		file << "Datapacking= Point" << "	Zonetype=FEbrick" << endl;
		for (int i = 0; i < NP; i++)
			file << PointStd[i].X[0] << " " << PointStd[i].X[1] << " " << PointStd[i].X[2] << endl;


		for (int i = 0; i < NC; i++) {

			if (CellStd[i].CType == CELL::tetra) {

				POINT* P[4] = { CellStd[i].F[0]->P[0],CellStd[i].F[0]->P[1],CellStd[i].F[0]->P[2],nullptr };
				for (int j = 0; j < 3; j++) {
					if (CellStd[i].F[1]->P[j] != P[0] && CellStd[i].F[1]->P[j] != P[1] && CellStd[i].F[1]->P[j] != P[2]) {
						P[3] = CellStd[i].F[1]->P[j];
						break;
					}
				}
				file << A2N(P[0]) << " " << A2N(P[1]) << "  " << A2N(P[2]) << " " << A2N(P[3]) << " " << A2N(P[3]) << "	" << A2N(P[3]) << "	" << A2N(P[3]) << "	" << A2N(P[3]) << endl;
			}
			else if (CellStd[i].CType == CELL::pyramid) {
				if (CellStd[i].F[0]->FacePointSize != 4) {
					throw;
				}
				POINT* P[5] = { CellStd[i].F[0]->P[0],CellStd[i].F[0]->P[1],CellStd[i].F[0]->P[2],CellStd[i].F[0]->P[3],nullptr };
				for (int j = 0; j < 3; j++) {
					if (CellStd[i].F[1]->P[j] != P[0] && CellStd[i].F[1]->P[j] != P[1] && CellStd[i].F[1]->P[j] != P[2] && CellStd[i].F[1]->P[j] != P[3]) {
						P[4] = CellStd[i].F[1]->P[j];
						break;
					}
				}

				file << A2N(P[0]) << "	" << A2N(P[1]) << "  " << A2N(P[2]) << "	" << A2N(P[3]) << "	" << A2N(P[4]) << "	" << A2N(P[4]) << "	" << A2N(P[4]) << "	" << A2N(P[4]) << endl;
			}
			else if (CellStd[i].CType == CELL::prism) {

				POINT* P[6] = { CellStd[i].F[0]->P[0] ,CellStd[i].F[0]->P[1] ,CellStd[i].F[0]->P[2],CellStd[i].F[1]->P[0],CellStd[i].F[1]->P[1],CellStd[i].F[1]->P[2] };
				file << A2N(P[0]) << "	" << A2N(P[1]) << "  " << A2N(P[2]) << "	" << A2N(P[2]) << "	" << A2N(P[3]) << "	" << A2N(P[4]) << "	" << A2N(P[5]) << "	" << A2N(P[5]) << endl;
			}
			else if (CellStd[i].CType == CELL::cube) {

				POINT* P[8] = { CellStd[i].F[0]->P[0] ,CellStd[i].F[0]->P[1] ,CellStd[i].F[0]->P[2],CellStd[i].F[0]->P[3],CellStd[i].F[1]->P[0],CellStd[i].F[1]->P[1],CellStd[i].F[1]->P[2],CellStd[i].F[1]->P[3] };
				file << A2N(P[0]) << "	" << A2N(P[1]) << "  " << A2N(P[2]) << " " << A2N(P[3]) << "	" << A2N(P[4]) << "	" << A2N(P[5]) << "	" << A2N(P[6]) << "	" << A2N(P[7]) << endl;
			}
		}


	}
	int Boundary_Face = 0;
	for (int i = 0; i < mesh.NF; i++) {
		if (FaceStd[i].IsOnBound == 1 && FaceStd[i].FacePointSize == 3) {
			Boundary_Face++;
		}
	}

	if (file.is_open() == 1) {
		file << "Variables=" << "\"X\"" << "," << " \"Y\" " << "," << " \"Z\" " << endl;
		file << "Zone" << " " << "T='Boundary'  " << " C=RED  " << "N=" << " " << NP << "  E=" << "  " << Boundary_Face << endl;
		file << "Datapacking= Point" << "	Zonetype=Fetriangle" << endl;
		for (int i = 0; i < NP; i++)
			file << PointStd[i].X[0] << " " << PointStd[i].X[1] << " " << PointStd[i].X[2] << endl;

		for (int i = 0; i < mesh.NF; i++) {
			if (FaceStd[i].IsOnBound == 1 && FaceStd[i].FacePointSize == 3) {
				file << A2N(FaceStd[i].P[0]) << " " << A2N(FaceStd[i].P[1]) << "  " << A2N(FaceStd[i].P[2]) << endl;

			}

		}
	}

}


bool MESH::Cell_order(vector<CELL*>& layer) {
	if (layer[0]->CType != CELL::prism)
		return false;
	POINT* Ptest[2];

	while (!((layer[0]->F[2]->P[0] == layer[0]->F[0]->P[0] && layer[0]->F[2]->P[1] == layer[0]->F[0]->P[1]) || (layer[0]->F[2]->P[1] == layer[0]->F[0]->P[0] && layer[0]->F[2]->P[0] == layer[0]->F[0]->P[1])))
		layer[0]->F[2]->push();
	while (!((layer[0]->F[3]->P[0] == layer[0]->F[0]->P[1] && layer[0]->F[3]->P[1] == layer[0]->F[0]->P[2]) || (layer[0]->F[3]->P[1] == layer[0]->F[0]->P[1] && layer[0]->F[3]->P[0] == layer[0]->F[0]->P[2])))
		layer[0]->F[3]->push();
	while (!((layer[0]->F[4]->P[0] == layer[0]->F[0]->P[2] && layer[0]->F[4]->P[1] == layer[0]->F[0]->P[0]) || (layer[0]->F[4]->P[1] == layer[0]->F[0]->P[2] && layer[0]->F[4]->P[0] == layer[0]->F[0]->P[0])))
		layer[0]->F[4]->push();

	layer[0]->F[2]->FaceIsLabl = 1;
	layer[0]->F[3]->FaceIsLabl = 1;
	layer[0]->F[4]->FaceIsLabl = 1;

	for (int i = 1; i < layer.size(); i++) {
		if (layer[i]->CType == CELL::prism) {
			int lnum=-1;
			for(int j=2;j<5;j++)
				if (layer[i]->F[j]->FaceIsLabl == 1) {
					lnum = j;
					break;
				}
			if (lnum == -1)
				throw;
			layer[i]->F[lnum]->join(layer[i]->F[1], Ptest);
			if ((Ptest[0] == layer[i]->F[lnum]->P[0] && Ptest[1] == layer[i]->F[lnum]->P[1]) || (Ptest[1] == layer[i]->F[lnum]->P[0] && Ptest[0] == layer[i]->F[lnum]->P[1]))
				swap(layer[i]->F[0], layer[i]->F[1]);

			while (!((layer[i]->F[2]->P[0] == layer[i]->F[0]->P[0] && layer[i]->F[2]->P[1] == layer[i]->F[0]->P[1]) || (layer[i]->F[2]->P[1] == layer[i]->F[0]->P[0] && layer[i]->F[2]->P[0] == layer[i]->F[0]->P[1])))
				layer[i]->F[2]->push();
			while (!((layer[i]->F[3]->P[0] == layer[i]->F[0]->P[1] && layer[i]->F[3]->P[1] == layer[i]->F[0]->P[2]) || (layer[i]->F[3]->P[1] == layer[i]->F[0]->P[1] && layer[i]->F[3]->P[0] == layer[i]->F[0]->P[2])))
				layer[i]->F[3]->push();
			while (!((layer[i]->F[4]->P[0] == layer[i]->F[0]->P[2] && layer[i]->F[4]->P[1] == layer[i]->F[0]->P[0]) || (layer[i]->F[4]->P[1] == layer[i]->F[0]->P[2] && layer[i]->F[4]->P[0] == layer[i]->F[0]->P[0])))
				layer[i]->F[4]->push();

			layer[i]->F[2]->FaceIsLabl = 1;
			layer[i]->F[3]->FaceIsLabl = 1;
			layer[i]->F[4]->FaceIsLabl = 1;

		}
		if (layer[i]->CType == CELL::pyramid) {
			for (int j = 1; j < 5; j++) {
				layer[i]->F[0]->join(layer[i]->F[j], Ptest);
				if ((Ptest[0] == layer[i]->F[0]->P[0] && Ptest[1] == layer[i]->F[0]->P[1]) || (Ptest[1] == layer[i]->F[0]->P[0] && Ptest[0] == layer[i]->F[0]->P[1])) {
					swap(layer[i]->F[j], layer[i]->F[1]);
					break;
				}
			}
			for (int j = 2; j < 5; j++) {
				layer[i]->F[0]->join(layer[i]->F[j], Ptest);
				if ((Ptest[0] == layer[i]->F[0]->P[2] && Ptest[1] == layer[i]->F[0]->P[3]) || (Ptest[1] == layer[i]->F[0]->P[2] && Ptest[0] == layer[i]->F[0]->P[3])) {
					swap(layer[i]->F[j], layer[i]->F[2]);
					break;
				}
			}
			layer[i]->F[0]->join(layer[i]->F[3], Ptest);
			if ((Ptest[0] == layer[i]->F[0]->P[3] && Ptest[1] == layer[i]->F[0]->P[0]) || (Ptest[1] == layer[i]->F[0]->P[3] && Ptest[0] == layer[i]->F[0]->P[0]))
				swap(layer[i]->F[3], layer[i]->F[4]);


			layer[i]->F[0]->join(layer[i]->F[3], Ptest);
			if (!((Ptest[0] == layer[i]->F[3]->P[0] && Ptest[1] == layer[i]->F[3]->P[1]) || (Ptest[1] == layer[i]->F[3]->P[0] && Ptest[0] == layer[i]->F[3]->P[1])))
				layer[i]->F[3]->push();
			if (!((Ptest[0] == layer[i]->F[3]->P[0] && Ptest[1] == layer[i]->F[3]->P[1]) || (Ptest[1] == layer[i]->F[3]->P[0] && Ptest[0] == layer[i]->F[3]->P[1])))
				throw;
			layer[i]->F[0]->join(layer[i]->F[4], Ptest);
			if (!((Ptest[0] == layer[i]->F[4]->P[0] && Ptest[1] == layer[i]->F[4]->P[1]) || (Ptest[1] == layer[i]->F[4]->P[0] && Ptest[0] == layer[i]->F[4]->P[1])))
				layer[i]->F[3]->push();
			if (!((Ptest[0] == layer[i]->F[4]->P[0] && Ptest[1] == layer[i]->F[4]->P[1]) || (Ptest[1] == layer[i]->F[4]->P[0] && Ptest[0] == layer[i]->F[4]->P[1])))
				throw;
			layer[i]->F[3]->FaceIsLabl = 1;
			layer[i]->F[4]->FaceIsLabl = 1;
		}
	}

}

bPOINT MESH::mid(POINT* P1, POINT* P2) {
	bPOINT bP;
	bP.X[0] = (P1->X[0] + P2->X[0]) / 2;
	bP.X[1] = (P1->X[1] + P2->X[1]) / 2;
	bP.X[2] = (P1->X[2] + P2->X[2]) / 2;
	return bP;
}