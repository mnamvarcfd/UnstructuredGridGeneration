#pragma once
#include "MESH.h"

vector<void*> D;
POINT* Point[100]{ nullptr };
EDGE* Edge[100]{ nullptr };
CELL* Cell[100]{ nullptr };
FACE* Face[100]{ nullptr };
MESH mesh;
int t = 0; //debug parameter