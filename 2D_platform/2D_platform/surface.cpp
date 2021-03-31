#include "stdafx.h"
#include "surface.h"

// part 1 :
void SURFACE::order() {

	
	vector<EDGE*> En;
	vector<CELL*> Cn[2];
	int m = 0;
	En.resize(E.size() - 1);
	Cn[0].resize(E.size() - 1);
	Cn[1].resize(E.size() - 1);

	
	int counter = 0;
	for (int i = E.size() - 1; i >= 0; i--) {
		if (E[i] != E[0]) {
			En[counter] = E[i];
			Cn[0][counter] = C[0][i];
			Cn[1][counter++] = C[1][i];
			E[i]->EdgIsLabl = 0;
		}

		else {
			En[counter] = E[i];
			Cn[0][counter] = C[0][i];
			Cn[1][counter++] = C[1][i];
			E[i]->EdgIsLabl = 0;
			break;
		}

	}

	for (int i = 1; i < E.size(); i++) {
		if (E[i] != E[0]) {
			En[counter] = E[i];
			Cn[0][counter] = C[0][i];
			Cn[1][counter++] = C[1][i];
			E[i]->EdgIsLabl = 0;
		}
		else
			break;
	}

	for (int i = 0; i < Cn[0].size() - 1; i++) {
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				if (Cn[0][i]->E[j] == Cn[0][i + 1]->E[k])
					m = 1;
		if (m != 1) {
			swap(Cn[0][i + 1], Cn[1][i + 1]);

		}
		m = 0;
	}

	E = En;
	C[0] = Cn[0];
	C[1] = Cn[1];
}

// part 2 :
// Definition flip Method in surface Class : change order of specified surface
void SURFACE::flip() {


	SURFACE N;
	N.E.resize(E.size());
	N.C[0].resize(E.size());
	N.C[1].resize(E.size());


	for (int i = 0; i < E.size(); i++) {
		N.E[i] = E[E.size() - i - 1];
		N.C[0][i] = C[0][E.size() - i - 1];
		N.C[1][i] = C[1][E.size() - i - 1];
	}
	*this = N;
}

// part 3 :
void SURFACE::clear() {
	E.clear();
	C[0].clear();
	C[1].clear();
	
}