#define _USE_MATH_DEFINES
#include "MyPrimitive.h"
#include <math.h>
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	//3--2
	//|  |
	//0--1

	vector3 * verts;
	verts = new vector3[a_nSubdivisions * 2];

	for (int i = 0; i < a_nSubdivisions * 2; i+=2)
	{
		float angle = (M_PI * 2.0f) * ((float)(i/2) / (float)a_nSubdivisions);
		float x = cos(angle) * a_fRadius;
		float y = sin(angle) * a_fRadius;
		vector3 basePoint(x, y, 0.0f);
		vector3 topPoint(0.0f, 0.0f, a_fHeight);

		verts[i] = basePoint;
		verts[i + 1] = topPoint;
	}

	for (int i = 0; i < a_nSubdivisions * 2; i+=2)
	{
		if (i == (a_nSubdivisions * 2) - 2)
		{
			AddQuad(verts[i], verts[0], verts[1], verts[i + 1]);
		}
		else
		{
			AddQuad(verts[i], verts[i + 2], verts[i + 3], verts[i + 1]);
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);

	delete[] verts;
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here

	vector3 * verts;
	verts = new vector3[a_nSubdivisions * 2];

	for (int i = 0; i < a_nSubdivisions * 2; i += 2)
	{
		float angle = (M_PI * 2.0f) * ((float)(i / 2) / (float)a_nSubdivisions);
		float x = cos(angle) * a_fRadius;
		float y = sin(angle) * a_fRadius;
		vector3 basePoint(x, y, 0.0f);
		vector3 topPoint(x, y, a_fHeight);

		verts[i] = basePoint;
		verts[i + 1] = topPoint;
	}

	vector3 bottomCenter(0.0f, 0.0f, 0.0f);
	vector3 topCenter(0.0f, 0.0f, a_fHeight);

	for (int i = 0; i < a_nSubdivisions * 2; i += 2)
	{
		if (i == (a_nSubdivisions * 2) - 2)
		{
			AddQuad(verts[i], verts[0], verts[i + 1], verts[1]);
			AddQuad(bottomCenter, bottomCenter, verts[i], verts[0]);
			AddQuad(verts[i + 1], verts[1], topCenter, topCenter);
		}
		else
		{
			AddQuad(verts[i], verts[i + 2], verts[i + 1], verts[i + 3]);
			AddQuad(bottomCenter, bottomCenter, verts[i], verts[i + 2]);
			AddQuad(verts[i + 1], verts[i + 3], topCenter, topCenter);
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);

	delete[] verts;
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 * outerVerts;
	outerVerts = new vector3[a_nSubdivisions * 2];
	vector3 * innerVerts;
	innerVerts = new vector3[a_nSubdivisions * 2];

	for (int i = 0; i < a_nSubdivisions * 2; i += 2)
	{
		float angle = (M_PI * 2.0f) * ((float)(i / 2) / (float)a_nSubdivisions);
		float outerX = cos(angle) * a_fOuterRadius;
		float outerY = sin(angle) * a_fOuterRadius;
		float innerX = cos(angle) * a_fInnerRadius;
		float innerY = sin(angle) * a_fInnerRadius;
		vector3 outBasePoint(outerX, outerY, 0.0f);
		vector3 outTopPoint(outerX, outerY, a_fHeight);
		vector3 inBasePoint(innerX, innerY, 0.0f);
		vector3 inTopPoint(innerX, innerY, a_fHeight);

		outerVerts[i] = outBasePoint;
		outerVerts[i + 1] = outTopPoint;
		innerVerts[i] = inBasePoint;
		innerVerts[i + 1] = inTopPoint;
	}

	for (int i = 0; i < a_nSubdivisions * 2; i += 2)
	{
		if (i == (a_nSubdivisions * 2) - 2)
		{
			AddQuad(outerVerts[i], outerVerts[0], outerVerts[i + 1], outerVerts[1]);
			AddQuad(innerVerts[i + 1], innerVerts[1], innerVerts[i], innerVerts[0]);
			AddQuad(outerVerts[i + 1], outerVerts[1], innerVerts[i + 1], innerVerts[1]);
			AddQuad(innerVerts[i], innerVerts[0], outerVerts[i], outerVerts[0]);
		}
		else
		{
			AddQuad(outerVerts[i], outerVerts[i + 2], outerVerts[i + 1], outerVerts[i + 3]);
			AddQuad(innerVerts[i + 1], innerVerts[i + 3], innerVerts[i], innerVerts[i + 2]);
			AddQuad(outerVerts[i + 1], outerVerts[i + 3], innerVerts[i + 1], innerVerts[i + 3]);
			AddQuad(innerVerts[i], innerVerts[i + 2], outerVerts[i], outerVerts[i + 2]);
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);
	delete[] outerVerts;
	delete[] innerVerts;
}
void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	AddQuad(point0, point1, point3, point2);

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	//Your code starts here
	int numVerts = (a_nSubdivisions + 2) * a_nSubdivisions;
	vector3 * verts;
	verts = new vector3[numVerts];

	int rings = a_nSubdivisions + 2;
	int columns = a_nSubdivisions;

	int index = 0;

	for (int curRing = 0; curRing < rings; curRing++)
	{
		float anglePhi = (float)M_PI * ((float)curRing / ((float)rings - 1.0f));
		float z = a_fRadius * cos(anglePhi);

		for (int curColumn = 0; curColumn < columns; curColumn++)
		{
			float angle = (float)(M_PI * 2) * ((float)curColumn / (float)columns);
			float radius = sin((float)M_PI * ((float)curRing / (float)(rings - 1.0f))) * a_fRadius;

			float x = (float)cos(angle) * radius * sin(anglePhi);
			float y = (float)sin(angle) * radius * sin(anglePhi);

			vector3 point(x, y, z);

			verts[index] = point;
			index += 1;
		}
	}

	for (int curRing = 0; curRing < rings - 1; curRing++)
	{ 
		for (int curColumn = 0; curColumn < columns; curColumn++)
		{
			index = curColumn + (columns * curRing);

			if (curColumn == columns - 1)
			{
				AddQuad(verts[index + columns], verts[(columns * curRing) + columns], verts[index], verts[columns * curRing]);
			}
			else
			{
				AddQuad(verts[index + columns], verts[index + columns + 1], verts[index], verts[index + 1]);
			}
		}
	}

	//Your code ends here
	CompileObject(a_v3Color);

	delete[] verts;
}