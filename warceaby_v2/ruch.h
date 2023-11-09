#pragma once
#include <vector>
#include "wspolrzedna.h"

class ruch
{
public:
	int i1, j1, i2, j2;
	bool operator==(ruch&);
	ruch(int i1, int j1, int i2, int j2) :i1(i1), j1(j1), i2(i2), j2(j2) {};
};

class bicie :public ruch
{
public:
	vector<wspolrzedna> zbite;
	bool operator==(ruch&);
	bicie() :ruch(-1, -1, -1, -1) {};
	bicie(int i1, int j1, int i2, int j2) :ruch(i1,j1,i2,j2) {};
};
