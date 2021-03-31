#pragma once
#include "mesh.h"

// Global Variables
vector<void*> D;
EDGE* Edge[100]{ nullptr };
CELL* Cell[100]{ nullptr };
POINT* Point[100]{ nullptr };
MESH mesh;
int t = 0; //debug parameter