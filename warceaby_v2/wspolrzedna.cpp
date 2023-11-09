#include "wspolrzedna.h"

wspolrzedna::wspolrzedna(int i, int j): i(i), j(j)
{
	w = (char)(i + 'a') + (char)(j + '1');
}

wspolrzedna::wspolrzedna(string s) : w(s)
{
	j = (int)s[0] - 'a';
	i = (int)s[1] - '1';
	if (s.length() == 3)
	{
		i += 1;
		i *= 10;
		i += (int)s[2] - '0';
		i -= 1;
	}
		
}

wspolrzedna::wspolrzedna()
{
	i = -1;
	j = -1;
	w = "00";
}