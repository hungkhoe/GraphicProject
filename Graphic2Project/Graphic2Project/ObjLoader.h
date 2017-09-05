#pragma once
#include "define.h"
class Object
{

	vector<Vert> model;
	vector<unsigned int> index;

public:

	Object() = default;
	~Object() = default;

	vector<Vert> & GetModel() { return model; }
	vector<unsigned int> & GetIndex() { return index; }

	bool Load(const char * filepath);
};

