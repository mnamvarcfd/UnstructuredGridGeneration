#include "stdafx.h"
#include "ZONE.h"



bool ZONE::extend(CELL* C, FACE* F, vector<CELL*>& layer) {
	layer.clear();
	int n = 0;
	FACE* LF[4];
	int check;
	int m = 0;
	int F_C = 0;
	CELL* Ce[2];
	int Ce_C = 0;
	POINT* P[2];
	FACE* OF;
	if (C->CType == CELL::prism) {
		if (F->FacePointSize != 3)
			return false;
		if (F != C->F[0] && F != C->F[1])
			return false;
		if (F != C->F[0])
			swap(C->F[0], C->F[1]);

		memcpy(LF, C->F+2, 3 * sizeof(FACE*));
		F_stack[0] = LF[0];

		for (int i = 2; i < 5; i++)
			while (!(F->is_in(C->F[i]->P[0]) && F->is_in(C->F[i]->P[1])))
				C->F[i]->push();

		m = 0;
		CELL* Ce[2];
		int t;
		if (C->CellIsLabl == false) {
			C->CellIsLabl = true;
			C_stack[n++] = C;
			while (n != 0) {
				PR:


				if (C_stack[0]->CType == CELL::cube)
					goto CU;

				if (C_stack[0]->F[0]->is_in(F_stack[0]->P[0]) == 0)
					swap(C_stack[0]->F[0], C_stack[0]->F[1]);

				memcpy(LF, C_stack[0]->F + 2, 3 * sizeof(FACE*));
				for (int i = 0; i < 3; i++)
					while (!(C_stack[0]->F[0]->is_in(LF[i]->P[0]) && C_stack[0]->F[0]->is_in(LF[i]->P[1])))
						LF[i]->push();

				for (int i = 0; i < C_stack[0]->CellFaceSize; i++) {
					if (C_stack[0]->F[i]->FacePointSize==4) {
						{
							FACE** f;
							m = 0;
							int F_size;
							for (int k = 0; k < C_stack[0]->F[i]->P[0]->PointCellSize; k++) {
								f = (C_stack[0]->F[i]->P[0]->PointCells[k])->F;
								for (int j = 0; j<6; j++)
									if (*(f + j) == C_stack[0]->F[i]) {
										Ce[m++] = C_stack[0]->F[i]->P[0]->PointCells[k];
										break;
									}
								//if (m == 2)
								//break;
							}
							if (m > 2 || m == 0)
								throw;
							
						}
						if (Ce[0] != C_stack[0] && Ce[0]->CellIsLabl == 0) {
							C_stack[n++] = Ce[0];
							F_stack[n - 1] = C_stack[0]->F[i];
							C_stack[n - 1]->CellIsLabl = true;
						}
						if (Ce[1] != C_stack[0] && Ce[1]->CellIsLabl == 0 && m==2){
							C_stack[n++] = Ce[1];
							F_stack[n - 1] = C_stack[0]->F[i];
							C_stack[n - 1]->CellIsLabl = true;
						}
					}
				}
				swap(C_stack[n - 1], C_stack[0]);
				layer.push_back(C_stack[n - 1]);
				C_stack[n - 1] = nullptr;
				//C_stack[n - 1]->is_labeled = false;
				n--;
			}
			for (int i = 0; i < layer.size(); i++)
				layer[i]->CellIsLabl = 0;
			return true;
		}
	}
	if (C->CType == CELL::cube) {
		
		if (C->is_in(F) == 0)
			return 0;
		
		OF = C->across_F(F);
		

		

		for (int i = 0; i < 6; i++)
			if (C->F[i] != F && C->F[i] != OF)
				LF[F_C++] = C->F[i];

		if (F_C != 4)
			throw;

		if (LF[0]->is_joined(LF[1]) == 0)
			swap(LF[2], LF[1]);
		if (LF[0]->is_joined(LF[1]) != 2)
			throw;
		if (LF[2]->is_joined(LF[3]) != 2)
			throw;

		for (int i = 0; i < 4; i++)
			while (F->is_in(LF[i]->P[0]) != 1 || F->is_in(LF[i]->P[1]) != 1)
				LF[i]->push();
		C->CellIsLabl = 1;
		C_stack[0] = C;
		F_stack[0] = LF[0];
		n++;
		while (n != 0) {
			if (C_stack[0]->CType == CELL::prism)
				goto PR;


			CU:


			if (C_stack[0]->CType == CELL::cube) {
				LF[0] = F_stack[0];
				LF[1] = C_stack[0]->across_F(F_stack[0]);
				for (int i = 0; i < 6;i++)
					if (!((C_stack[0]->F[i]->is_in(LF[0]->P[0]) && C_stack[0]->F[i]->is_in(LF[0]->P[1])) || (C_stack[0]->F[i]->is_in(LF[0]->P[2]) && C_stack[0]->F[i]->is_in(LF[0]->P[3]))) && C_stack[0]->F[i]!=LF[0] && C_stack[0]->F[i]!=LF[1]) {
						LF[2] = C_stack[0]->F[i];
						break;
					}
				LF[3] = C_stack[0]->across_F(LF[2]);


				check = LF[0]->join(LF[2], P);
				if (check != 2)
					throw;
				while ((LF[2]->P[1] == P[0] && LF[2]->P[2] == P[1]) || (LF[2]->P[1] == P[1] && LF[2]->P[2] == P[0]))
					LF[2]->push();

				check = LF[0]->join(LF[3], P);
				if (check != 2)
					throw;
				while ((LF[3]->P[1] == P[0] && LF[3]->P[2] == P[1]) || (LF[3]->P[1] == P[1] && LF[3]->P[2] == P[0]))
					LF[3]->push();

				check = LF[1]->join(LF[3], P);
				if (check != 2)
					throw;
				while ((LF[1]->P[1] == P[0] && LF[1]->P[2] == P[1]) || (LF[1]->P[1] == P[1] && LF[1]->P[2] == P[0]))
					LF[3]->push();


				for (int k = 0; k<4; k++) {
					FACE** f;
					int m = 0;
					int F_size;
					for (int i = 0; i < LF[k]->P[0]->PointCellSize; i++) {
						f = (LF[k]->P[0]->PointCells[i])->F;
						for (int j = 0; j<6; j++)
							if (*(f + j) == LF[k]) {
								Ce[m++] = LF[k]->P[0]->PointCells[i];
								break;
							}
						//if (m == 2)
						//break;
					}
					if (m > 2 || m == 0)
						throw;
					if (m == 2) {
						if (Ce[0] == C)
							swap(Ce[0], Ce[1]);
						if (Ce[0]->CType != CELL::tetra && Ce[0]->CellIsLabl == 0) {
							C_stack[n] = Ce[0];
							F_stack[n++] = LF[k];
							Ce[0]->CellIsLabl = 1;
						}

					}
				}
				swap(C_stack[n - 1], C_stack[0]);
				layer.push_back(C_stack[n - 1]);
				swap(F_stack[n - 1], F_stack[0]);
				C_stack[n - 1] = nullptr;
				F_stack[n - 1] = nullptr;
				//C_stack[n - 1]->is_labeled = false;
				n--;
				
			}
			if (C_stack[0]->CType == CELL::pyramid) {
				swap(C_stack[n - 1], C_stack[0]);
				layer.push_back(C_stack[n - 1]);
				swap(F_stack[n - 1], F_stack[0]);
				C_stack[n - 1] = nullptr;
				F_stack[n - 1] = nullptr;
				n--;
			}
		}

		for (int i = 0; i < layer.size(); i++)
			layer[i]->CellIsLabl = 0;
		return true;


	}
	return false;

}