#include"MyTypes.h"

namespace Inferno
{
	void Matrix::LoadIdentity()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (i == j)
					m[i][j] = 1;
				else
					m[i][j] = 0;
			}
		}
	}

	void Matrix::Transform2D(const float x, const float y)
	{
		m[3][0] += x;
		m[3][1] += y;
	}

	const D3DMATRIX Matrix::GetD3DMatrix() const
	{
		D3DMATRIX dm;
		
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				dm.m[i][j] = m[i][j];
			}
		}

		return dm;
	}

}