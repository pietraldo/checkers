#include "ruch.h"

bool ruch::operator==(ruch& b2)
{
	return (b2.i1==i1 && b2.j1==j1 && b2.i2==i2 && b2.j2==j2);
}
bool bicie::operator==(ruch& b2)
{
	return ruch::operator==(b2);
}