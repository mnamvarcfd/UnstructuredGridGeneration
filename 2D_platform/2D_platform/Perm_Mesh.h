#pragma once
#include <vector>
#include <array>

class Perm_Mesh {
public:
	int mesh_dim = 2;
	int NP = 0;
	int NC = 0;
	int NF = 0;
	int NR = 0;
	std::vector<int> regions_BC;
	std::vector<std::array<int, 6>> edge_data;
	std::vector<std::array<double, 3>> point_coordinate;
	Perm_Mesh() = default;
	Perm_Mesh(const char*);

};