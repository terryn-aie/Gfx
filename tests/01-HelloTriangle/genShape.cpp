#include "graphics\RenderObjects.h"
#include "graphics\Vertex.h"
#include "glm\glm.hpp"

#include "genShape.h"
#include <cmath>

Geometry makeNGon(size_t N, float r)
{	
	unsigned vsize = N+1;
	unsigned isize = N*3;
	Vertex *verts = new Vertex[vsize];
	unsigned *idxs = new unsigned[isize];

	// circle = 360 or 2PI
	// angle of each triangle = 360 / N
	float a = (2 * 3.14159265359) / N;

	verts[N] = { { 0,0,0,1 },{ 1,1,1,1 } };

	for (int i = 0; i < N; ++i)
	{
		float x = cos(i*a)*r; // angle to vector
		float y = sin(i*a)*r;
		
		verts[i] = { {x,y,0,1},{ x,y,0,1 }};
	
		idxs[i*3+0] = N;
		idxs[i*3+1] = i;
		idxs[i*3+2] = (i+1)%N;
	}

	Geometry ret = makeGeometry(verts,vsize,idxs,isize);

	delete[] verts;
	delete[] idxs;

	return ret;
}





Geometry makeCheckerboard(int dim, float size)
{
	unsigned vsize = (dim + 1)*(dim + 1);
	unsigned isize = 3 * 2 * dim * dim;
	Vertex *verts = new Vertex[vsize];
	unsigned *idxs = new unsigned[isize];

	float step =  dim / size;

	for (int i = 0; i < dim+1; ++i)
	{
		float y = i * step;

		for (int j = 0; j < dim+1; ++j)
		{
			float x = j * step;
		}
	}

	Geometry ret = makeGeometry(verts, vsize, idxs, isize);

	delete[] verts;
	delete[] idxs;

	return ret;
}