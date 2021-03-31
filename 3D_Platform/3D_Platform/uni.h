#pragma once

#include "CELL.h"

union A {
	FACE* f;
	CELL* c;
	POINT* p;
};