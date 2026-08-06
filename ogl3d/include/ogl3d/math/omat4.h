#pragma once
#include <ogl3d/oprerequisites.h>
#include <ogl3d/math/ovec4.h>
#include <cmath>


class OMat4
{
public:
	OMat4()
	{
		setIdentity();
	}
	void setIdentity()
	{
		::memset(mat, 0, sizeof(f32) * (16));
		mat[0][0] = 1;
		mat[1][1] = 1;
		mat[2][2] = 1;
		mat[3][3] = 1;
	}

	void setScale(const Ovec4& scale)
	{
		setIdentity();
		mat[0][0] = scale.x;
		mat[1][1] = scale.y;
		mat[2][2] = scale.z;
	}

	// Corrected spelling: setTranslation
	void setTranslation(const Ovec4& translation)
	{
		setIdentity();
		mat[3][0] = translation.x;
		mat[3][1] = translation.y;
		mat[3][2] = translation.z;
	}

	// Rotation helpers (angles in radians)
	void setRotationX(f32 angle)
	{
		setIdentity();
		f32 c = std::cos(angle);
		f32 s = std::sin(angle);
		mat[1][1] = c;
		mat[1][2] = -s;
		mat[2][1] = s;
		mat[2][2] = c;
	}

	void setRotationY(f32 angle)
	{
		setIdentity();
		f32 c = std::cos(angle);
		f32 s = std::sin(angle);
		mat[0][0] = c;
		mat[0][2] = s;
		mat[2][0] = -s;
		mat[2][2] = c;
	}

	void setRotationZ(f32 angle)
	{
		setIdentity();
		f32 c = std::cos(angle);
		f32 s = std::sin(angle);
		mat[0][0] = c;
		mat[0][1] = -s;
		mat[1][0] = s;
		mat[1][1] = c;
	}

	// Compose rotations Rz * Ry * Rx from Euler angles (x,y,z)
	void setRotationEuler(const Ovec4& euler)
	{
		OMat4 rx, ry, rz;
		rx.setRotationX(euler.x);
		ry.setRotationY(euler.y);
		rz.setRotationZ(euler.z);
		// order: Rz * Ry * Rx
		*this = rz;
		*this *= ry;
		*this *= rx;
	}

	void operator *=(const OMat4& matrix)
	{
		OMat4 out;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				out.mat[i][j] =
					mat[i][0] * matrix.mat[0][j] +
					mat[i][1] * matrix.mat[1][j] +
					mat[i][2] * matrix.mat[2][j] +
					mat[i][3] * matrix.mat[3][j];
			}
		}
		*this = out;
	}



public:
	f32 mat[4][4] = {};
};