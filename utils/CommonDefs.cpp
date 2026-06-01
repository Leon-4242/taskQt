#include "utils/CommonDefs.h"

Mesh::Triangle::Triangle (int a, int b, int c, double depth): a(a), b(b), c(c), depth(depth)
{}

Mesh::Triangle::Triangle (const Mesh::Triangle &tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
}

Mesh::Triangle::Triangle (Mesh::Triangle && tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
	tr.a = 0; tr.b = 0; tr.c = 0; tr.depth = 0;
}

Mesh::Triangle & Mesh::Triangle::operator= (const Mesh::Triangle &tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
	return *this;
}

Mesh::Triangle & Mesh::Triangle::operator= (Mesh::Triangle && tr)
{
	a = tr.a; b = tr.b; c = tr.c; depth = tr.depth;
	tr.a = 0; tr.b = 0; tr.c = 0; tr.depth = 0;
	return *this;
}
