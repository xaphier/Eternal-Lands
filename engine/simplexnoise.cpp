/****************************************************************************
 *            simplexnoise.cpp
 *
 * Author: 2011  Daniel Jungmann <el.3d.source@googlemail.com>
 * Copyright: See COPYING file that comes with this distribution
 ****************************************************************************/

#include "simplexnoise.hpp"

namespace eternal_lands
{

	unsigned char SimplexNoise::perm[512] =
	{
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
		7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
		203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
		20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48,
		27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
		230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65,
		25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18,
		169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
		173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147,
		118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17,
		182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
		154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39,
		253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104,
		218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191,
		179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192,
		214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50,
		45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24,
		72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180,
		151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233,
		7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219,
		203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
		20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48,
		27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133,
		230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65,
		25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18,
		169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100, 109, 198,
		173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147,
		118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17,
		182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2, 44,
		154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39,
		253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104,
		218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191,
		179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192,
		214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50,
		45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24,
		72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};

	static glm::vec2 grad2lut[8] =
	{
		glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec2(-1.0f, 0.0f), glm::vec2(1.0f, 1.0f),
		glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(1.0f, -1.0f)
	};

	static glm::vec3 grad3lut[16] =
	{
		glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0f, -1.0f, -1.0f)
	};

	static glm::vec4 grad4lut[32] =
	{
		glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), glm::vec4(0.0f, 1.0f, 1.0f, -1.0f),
		glm::vec4(0.0f, 1.0f, -1.0f, 1.0f), glm::vec4(0.0f, 1.0f, -1.0f, -1.0f),
		glm::vec4(0.0f, -1.0f, 1.0f, 1.0f), glm::vec4(0.0f, -1.0f, 1.0f, -1.0f),
		glm::vec4(0.0f, -1.0f, -1.0f, 1.0f), glm::vec4(0.0f, -1.0f, -1.0f, -1.0f),
		glm::vec4(1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(1.0f, 0.0f, 1.0f, -1.0f),
		glm::vec4(1.0f, 0.0f, -1.0f, 1.0f), glm::vec4(1.0f, 0.0f, -1.0f, -1.0f),
		glm::vec4(-1.0f, 0.0f, 1.0f, 1.0f), glm::vec4(-1.0f, 0.0f, 1.0f, -1.0f),
		glm::vec4(-1.0f, 0.0f, -1.0f, 1.0f), glm::vec4(-1.0f, 0.0f, -1.0f, -1.0f),
		glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 0.0f, -1.0f),
		glm::vec4(1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(1.0f, -1.0f, 0.0f, -1.0f),
		glm::vec4(-1.0f, 1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, 1.0f, 0.0f, -1.0f),
		glm::vec4(-1.0f, -1.0f, 0.0f, 1.0f), glm::vec4(-1.0f, -1.0f, 0.0f, -1.0f),
		glm::vec4(1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(1.0f, 1.0f, -1.0f, 0.0f),
		glm::vec4(1.0f, -1.0f, 1.0f, 0.0f), glm::vec4(1.0f, -1.0f, -1.0f, 0.0f),
		glm::vec4(-1.0f, 1.0f, 1.0f, 0.0f), glm::vec4(-1.0f, 1.0f, -1.0f, 0.0f),
		glm::vec4(-1.0f, -1.0f, 1.0f, 0.0f), glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f)
	};

	static unsigned char simplex[64][4] =
	{
		{0, 1, 2, 3}, {0, 1, 3, 2}, {0, 0, 0, 0}, {0, 2, 3, 1},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 2, 3, 0},
		{0, 2, 1, 3}, {0, 0, 0, 0}, {0, 3, 1, 2}, {0, 3, 2, 1},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {1, 3, 2, 0},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{1, 2, 0, 3}, {0, 0, 0, 0}, {1, 3, 0, 2}, {0, 0, 0, 0},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {2, 3, 0, 1}, {2, 3, 1, 0},
		{1, 0, 2, 3}, {1, 0, 3, 2}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{0, 0, 0, 0}, {2, 0, 3, 1}, {0, 0, 0, 0}, {2, 1, 3, 0},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{2, 0, 1, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{3, 0, 1, 2}, {3, 0, 2, 1}, {0, 0, 0, 0}, {3, 1, 2, 0},
		{2, 1, 0, 3}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0},
		{3, 1, 0, 2}, {0, 0, 0, 0}, {3, 2, 0, 1}, {3, 2, 1, 0}
	};

	float SimplexNoise::grad(const int hash, const float value)
	{
		float grad;
		int h;

		h = hash & 15;
		grad = 1.0f + (h & 7);

		if (h & 8)
		{
			grad = -grad;
		}

		return grad * value;
	}

	float SimplexNoise::grad(const int hash, const glm::vec2 &value)
	{
		int h;

		h = hash & 7;

		return glm::dot(grad2lut[h], value);
	}

	float SimplexNoise::grad(const int hash, const glm::vec3 &value)
	{
		int h;

		h = hash & 15;

		return glm::dot(grad3lut[h], value);
	}

	float SimplexNoise::grad(const int hash, const glm::vec4 &value)
	{
		int h;

		h = hash & 31;

		return glm::dot(grad4lut[h], value);
	}

	float SimplexNoise::noise(const float value)
	{
		float tx[2], n, t;
		int i[2], j;

		i[0] = std::floor(value);
		i[1] = i[0] + 1;
		tx[0] = value - i[0];
		tx[1] = tx[0] - 1.0f;

		n = 0.0f;

		for (j = 0; j < 2; j++)
		{
			t = 1.0f - tx[j] * tx[j];
			t *= t;
			n += t * t * grad(perm[i[j] & 0xff], tx[j]);
		}

		return 0.395f * n;
	}

	const float F2 = 0.5f * (std::sqrt(3.0f) - 1.0f);
	const float G2 = (3.0f - std::sqrt(3.0f)) / 6.0f;

	float SimplexNoise::noise(const glm::vec2 &value)
	{
		float tx[3], ty[3];
		float s, xs, ys;
		float t, n;
		int ti[3], tj[3];
		int i, j, k;
		int ii, jj;

		s = (value[0] + value[1]) * F2;
		xs = value[0] + s;
		ys = value[1] + s;
		i = std::floor(xs);
		j = std::floor(ys);

		t = (i + j) * G2;
		tx[0] = value[0] - (i - t);
		ty[0] = value[1] - (j - t);

		ti[0] = 0;
		tj[0] = 0;
		ti[2] = 1;
		tj[2] = 1;

		if (tx[0] > ty[0])
		{
			ti[1] = 1;
			tj[1] = 0;
		}
		else
		{
			ti[1] = 0;
			tj[1] = 1;
		}

		for (k = 1; k < 3; k++)
		{
			tx[k] = tx[0] - ti[k] + G2 * k;
			ty[k] = ty[0] - tj[k] + G2 * k;
		}

		ii = i % 256;
		jj = j % 256;

		n = 0.0;

		for (k = 0; k < 3; k++)
		{
			t = 0.5f - tx[k] * tx[k] - ty[k] * ty[k];

			if (t > 0.0f)
			{
				t *= t;
				n += t * t * grad(perm[ii + ti[k] + perm[jj + tj[k]]],
					glm::vec2(tx[k], ty[k]));
			}
		}

		return 40.0f * n;
	}

	const float F3 = 1.0f / 3.0f;
	const float G3 = 1.0f / 6.0f;

	float SimplexNoise::noise(const glm::vec3 &value)
	{
		float tx[4], ty[4], tz[4];
		float n, t;
		float s, xs, ys, zs;
		int ti[4], tj[4], tk[4];
		int i, j, k, l;
		int ii, jj, kk;

		s = (value[0] + value[1] + value[2]) * F3;
		xs = value[0] + s;
		ys = value[1] + s;
		zs = value[2] + s;

		i = std::floor(xs);
		j = std::floor(ys);
		k = std::floor(zs);

		t = (i + j + k) * G3;

		tx[0] = value[0] - (i - t);
		ty[0] = value[1] - (j - t);
		tz[0] = value[2] - (k - t);

		ti[0] = 0;
		tj[0] = 0;
		tk[0] = 0;
		ti[3] = 1;
		tj[3] = 1;
		tk[3] = 1;

		if (tx[0] >= ty[0])
		{
			if (ty[0] >= tz[0])
			{
				ti[1] = 1;
				tj[1] = 0;
				tk[1] = 0;
				ti[2] = 1;
				tj[2] = 1;
				tk[2] = 0;
			}
			else
			{
				if (tx[0] >= tz[0])
				{
					ti[1] = 1;
					tj[1] = 0;
					tk[1] = 0;
					ti[2] = 1;
					tj[2] = 0;
					tk[2] = 1;
				}
				else
				{
					ti[1] = 0;
					tj[1] = 0;
					tk[1] = 1;
					ti[2] = 1;
					tj[2] = 0;
					tk[2] = 1;
				}
			}
		}
		else
		{
			if (ty[0] < tz[0])
			{
				ti[1] = 0;
				tj[1] = 0;
				tk[1] = 1;
				ti[2] = 0;
				tj[2] = 1;
				tk[2] = 1;
			}
			else
			{
				if (tx[0] < tz[0])
				{
					ti[1] = 0;
					tj[1] = 1;
					tk[1] = 0;
					ti[2] = 0;
					tj[2] = 1;
					tk[2] = 1;
				}
				else
				{
					ti[1] = 0;
					tj[1] = 1;
					tk[1] = 0;
					ti[2] = 1;
					tj[2] = 1;
					tk[2] = 0;
				}
			}
		}

		for (l = 1; l < 4; l++)
		{
			tx[l] = tx[0] - ti[l] + G3 * l;
			ty[l] = ty[0] - tj[l] + G3 * l;
			tz[l] = tz[0] - tk[l] + G3 * l;
		}

		ii = i % 256;
		jj = j % 256;
		kk = k % 256;

		n = 0.0f;

		for (l = 0; l < 4; l++)
		{
			t = 0.6f - tx[l] * tx[l] - ty[l] * ty[l] - tz[l] * tz[l];
			if (t > 0.0f)
			{
				t *= t;
				n += t * t * grad(perm[ii + ti[l] + perm[jj + tj[l] + perm[kk + tk[l]]]],
					glm::vec3(tx[l], ty[l], tz[l]));
			}
		}

		return 32.0f * n;
	}

	const float F4 = (std::sqrt(5.0f) - 1.0f) / 4.0f;
	const float G4 = (5.0f - std::sqrt(5.0f)) / 20.0f;

	float SimplexNoise::noise(const glm::vec4 &value)
	{
		float tx[5], ty[5], tz[5], tw[5];
		float n, t;
		float s, xs, ys, zs, ws;
		int ti[5], tj[5], tk[5], tl[5];
		int c1, c2, c3, c4, c5, c6, c;
		int i, j, k, l, m;
		int ii, jj, kk, ll;

		s = (value[0] + value[1] + value[2] + value[3]) * F4;
		xs = value[0] + s;
		ys = value[1] + s;
		zs = value[2] + s;
		ws = value[3] + s;
		i = std::floor(xs);
		j = std::floor(ys);
		k = std::floor(zs);
		l = std::floor(ws);

		t = (i + j + k + l) * G4;

		tx[0] = value[0] - (i - t);
		ty[0] = value[1] - (j - t);
		tz[0] = value[2] - (k - t);
		tw[0] = value[3] - (l - t);

		c1 = (tx[0] > ty[0]) ? 32 : 0;
		c2 = (tx[0] > tz[0]) ? 16 : 0;
		c3 = (ty[0] > tz[0]) ? 8 : 0;
		c4 = (tx[0] > tw[0]) ? 4 : 0;
		c5 = (ty[0] > tw[0]) ? 2 : 0;
		c6 = (tz[0] > tw[0]) ? 1 : 0;
		c = c1 + c2 + c3 + c4 + c5 + c6;

		ti[0] = 0;
		tj[0] = 0;
		tk[0] = 0;
		tl[0] = 0;
		ti[4] = 1;
		tj[4] = 1;
		tk[4] = 1;
		tl[4] = 1;

		for (m = 1; m < 4; m++)
		{
			ti[m] = simplex[c][0] >= 4 - m ? 1 : 0;
			tj[m] = simplex[c][1] >= 4 - m ? 1 : 0;
			tk[m] = simplex[c][2] >= 4 - m ? 1 : 0;
			tl[m] = simplex[c][3] >= 4 - m ? 1 : 0;
		}

		for (m = 1; m < 5; m++)
		{
			tx[m] = tx[0] - ti[m] + G4 * m;
			ty[m] = ty[0] - tj[m] + G4 * m;
			tz[m] = tz[0] - tk[m] + G4 * m;
			tw[m] = tw[0] - tl[m] + G4 * m;
		}

		ii = i % 256;
		jj = j % 256;
		kk = k % 256;
		ll = l % 256;

		n = 0.0f;

		for (m = 0; m < 5; m++)
		{
			t = 0.6f - tx[m] * tx[m] - ty[m] * ty[m] - tz[m] * tz[m] - tw[m] * tw[m];

			if (t > 0.0f)
			{
				t *= t;
				n += t * t * grad(perm[ii + ti[m] + perm[jj + tj[m] + perm[kk + tk[m] + perm[ll + tl[m]]]]],
					glm::vec4(tx[m], ty[m], tz[m], tw[m]));
			}
		}

		return 27.0f * n;
	}

}
