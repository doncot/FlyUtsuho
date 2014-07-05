#include"MyTypes.h"

namespace Inferno
{
	Matrix* MatrixIdentity(Matrix* m)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
					m->m[i][j] = 1;
				else
					m->m[i][j] = 0;
			}
		}
		return m;
	}

	Matrix* MatrixTransform2D(Matrix* m, const float x, const float y)
	{
		m->m[3][0] += x;
		m->m[3][1] += y;
		return m;
	}

}