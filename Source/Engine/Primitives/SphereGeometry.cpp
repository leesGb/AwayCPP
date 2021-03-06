#include "SphereGeometry.h"
#include "CompactSubGeometry.h"
#include "MathConsts.h"

USING_AWAY_NAMESPACE

SphereGeometry::SphereGeometry(float radius, unsigned short segmentsW, unsigned short segmentsH, bool yUp)
{
	m_radius = radius;
	m_segmentsW = segmentsW;
	m_segmentsH = segmentsH;
	m_yUp = yUp;
}

void SphereGeometry::setRadius(float value)
{
	if (value != m_radius)
	{
		m_radius = value;
		invalidateGeometry();
	}
}

void SphereGeometry::setSegmentsW(unsigned short value)
{
	if (value != m_segmentsW)
	{
		m_segmentsW = value;
		invalidateGeometry();
		invalidateUVs();
	}
}

void SphereGeometry::setSegmentsH(unsigned short value)
{
	if (value != m_segmentsH)
	{
		m_segmentsH = value;
		invalidateGeometry();
		invalidateUVs();
	}
}

void SphereGeometry::setYUp(bool value)
{
	if (value != m_yUp)
	{
		m_yUp = value;
		invalidateGeometry();
	}
}

void SphereGeometry::buildGeometry(CompactSubGeometry* target)
{
	int numVertices = (m_segmentsH + 1) * (m_segmentsW + 1);
	int numIndices = (m_segmentsH - 1) * m_segmentsW * 6;
	int stride = target->getVertexStride();
	int skip = stride - 9;

	float* vertices;
	unsigned short* indices;
	if (numVertices == target->getVertexCount())
	{
		vertices = target->getVertexData();
		indices = target->getIndexData();
	}
	else
	{
		vertices = new float[numVertices * stride];
		indices = new unsigned short[numIndices];
		invalidateGeometry();
	}

	unsigned short i, j, a, b, c, d;
	int startIndex, index = target->getVertexOffset(), triIndex = 0;
	float horangle, z, ringradius, verangle, x, y, normLen, tanLen, t1, t2, comp1, comp2;
	for (j = 0; j <= m_segmentsH; j++)
	{
		startIndex = index;

		horangle = MathConsts::PI * j / m_segmentsH;
		z = -m_radius * std::cos(horangle);
		ringradius = m_radius * std::sin(horangle);

		for (i = 0; i <= m_segmentsW; i++)
		{
			verangle = MathConsts::TWO_PI * i / m_segmentsW;
			x = ringradius * std::cos(verangle);
			y = ringradius * std::sin(verangle);
			normLen = 1 / std::sqrt(x * x + y * y + z * z);
			tanLen = std::sqrt(y * y + x * x);

			if (m_yUp)
			{
				t1 = 0;
				t2 = tanLen > .007f ? x / tanLen : 0;
				comp1 = -z;
				comp2 = y;
			}
			else
			{
				t1 = tanLen > .007f ? x / tanLen : 0;
				t2 = 0;
				comp1 = y;
				comp2 = z;
			}

			if (i == m_segmentsW)
			{
				vertices[index++] = vertices[startIndex];
				vertices[index++] = vertices[startIndex + 1];
				vertices[index++] = vertices[startIndex + 2];
				vertices[index++] = vertices[startIndex + 3] + (x * normLen) * .5f;
				vertices[index++] = vertices[startIndex + 4] + (comp1 * normLen) * .5f;
				vertices[index++] = vertices[startIndex + 5] + (comp2 * normLen) * .5f;
				vertices[index++] = tanLen > .007f ? -y / tanLen : 1;
				vertices[index++] = t1;
				vertices[index++] = t2;
			}
			else
			{
				vertices[index++] = x;
				vertices[index++] = comp1;
				vertices[index++] = comp2;
				vertices[index++] = x * normLen;
				vertices[index++] = comp1 * normLen;
				vertices[index++] = comp2 * normLen;
				vertices[index++] = tanLen > .007f ? -y / tanLen : 1;
				vertices[index++] = t1;
				vertices[index++] = t2;
			}

			if (i > 0 && j > 0)
			{
				a = (m_segmentsW + 1) * j + i;
				b = (m_segmentsW + 1) * j + i - 1;
				c = (m_segmentsW + 1) * (j - 1) + i - 1;
				d = (m_segmentsW + 1) * (j - 1) + i;

				if (j == m_segmentsH)
				{
					vertices[index - 9] = vertices[startIndex];
					vertices[index - 8] = vertices[startIndex + 1];
					vertices[index - 7] = vertices[startIndex + 2];

					indices[triIndex++] = a;
					indices[triIndex++] = c;
					indices[triIndex++] = d;
				}
				else if (j == 1)
				{
					indices[triIndex++] = a;
					indices[triIndex++] = b;
					indices[triIndex++] = c;
				}
				else
				{
					indices[triIndex++] = a;
					indices[triIndex++] = b;
					indices[triIndex++] = c;
					indices[triIndex++] = a;
					indices[triIndex++] = c;
					indices[triIndex++] = d;
				}
			}

			index += skip;
		}
	}

	target->updateData(vertices, numVertices);
	target->updateIndexData(indices, numIndices);
}

void SphereGeometry::buildUVs(CompactSubGeometry* target)
{
	float* data = target->getUVData();
	int skip = target->getUVStride() - 2;
	int index = target->getUVOffset();
	for (unsigned short j = 0; j <= m_segmentsH; j++)
	{
		for (unsigned short i = 0; i <= m_segmentsW; i++)
		{
			data[index++] = (float)i / m_segmentsW;
			data[index++] = (float)j / m_segmentsH;
			index += skip;
		}
	}

	target->updateData(data, target->getVertexCount());
}