#define _USE_MATH_DEFINES
#include <math.h>
#include "MyPrimitive.h"
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
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;
	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fRadius;           //set radius 
	float incr = (2 * M_PI) / a_nSubdivisions; //set subdivision increment in radians
	float center = a_fHeight / 2;              //set it to center on origin
	vector3 point0(0, -center, 0);			   //set origin
	vector3 pointTop(0, a_fHeight/2, 0);       //set highest point
	float* x = new float[a_nSubdivisions + 1]; //create array for set of x coordinates
	float* z = new float[a_nSubdivisions + 1]; //create array for set of z coordinates

	for (int i = 1; i <= a_nSubdivisions; i++) {
		x[i] = cos(incr * i) * fValue; //increment subdivision and set x pos
		z[i] = sin(incr * i) * fValue; //increment subdivision and set z pos

		if (i == a_nSubdivisions) { //check if it is the last increment
			//draw triangle from point to previous point
			AddVertexPosition(point0);
			AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
			AddVertexPosition(vector3(x[i], -center, z[i]));

			//draw triangle to topmost point
			AddVertexPosition(vector3(x[i], -center, z[i]));
			AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
			AddVertexPosition(pointTop);

			//do for first position
			AddVertexPosition(point0);
			AddVertexPosition(vector3(x[i], -center, z[i]));
			AddVertexPosition(vector3(x[1], -center, z[1]));

			AddVertexPosition(vector3(x[1], -center, z[1]));
			AddVertexPosition(vector3(x[i], -center, z[i]));
			AddVertexPosition(pointTop);
		}
		else if (i > 1) { //every other increment
			AddVertexPosition(point0);
			AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
			AddVertexPosition(vector3(x[i], -center, z[i]));
			
			AddVertexPosition(vector3(x[i], -center, z[i]));
			AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
			AddVertexPosition(pointTop);
		}
	}

	delete x;
	delete z;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;
	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f * a_fRadius;           //set radius 
	float incr = (2 * M_PI) / a_nSubdivisions; //set subdivision increment in radians
	float center = a_fHeight / 2;              //set it to center on origin
	vector3 point0(0, -center, 0);			   //set origin
	vector3 pointTop(0, center, 0);       //set highest point
	float* x = new float[a_nSubdivisions + 1]; //create array for set of x coordinates
	float* z = new float[a_nSubdivisions + 1]; //create array for set of z coordinates

	for (int i = 1; i <= a_nSubdivisions; i++) {
		x[i] = cos(incr * i) * fValue; //increment subdivision and set x pos
		z[i] = sin(incr * i) * fValue; //increment subdivision and set z pos

		if (i == a_nSubdivisions) { //check if it is the last increment
			//draw triangle from point to previous point
			AddVertexPosition(point0);
			AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
			AddVertexPosition(vector3(x[i], -center, z[i]));

			//draw triangle at max height
			AddVertexPosition(pointTop);
			AddVertexPosition(vector3(x[i], center, z[i]));
			AddVertexPosition(vector3(x[i - 1], center, z[i - 1]));

			//draw a quad between the points
			AddQuad(vector3(x[i], -center, z[i]), vector3(x[i - 1], -center, z[i - 1]), vector3(x[i], center, z[i]), vector3(x[i - 1], center, z[i - 1]));

			//do for first position
			AddVertexPosition(point0);
			AddVertexPosition(vector3(x[i], -center, z[i]));
			AddVertexPosition(vector3(x[1], -center, z[1]));

			AddVertexPosition(pointTop);
			AddVertexPosition(vector3(x[1], center, z[1]));
			AddVertexPosition(vector3(x[i], center, z[i]));

			AddQuad(vector3(x[1], -center, z[1]), vector3(x[i], -center, z[i]), vector3(x[1], center, z[1]), vector3(x[i], center, z[i]));
		}
		else if (i > 1) { //every other increment
			AddVertexPosition(point0);
			AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
			AddVertexPosition(vector3(x[i], -center, z[i]));

			AddVertexPosition(pointTop);
			AddVertexPosition(vector3(x[i], center, z[i]));
			AddVertexPosition(vector3(x[i - 1], center, z[i - 1]));

			AddQuad(vector3(x[i], -center, z[i]), vector3(x[i - 1], -center, z[i - 1]), vector3(x[i], center, z[i]), vector3(x[i - 1], center, z[i - 1]));
		}
	}

	delete x;
	delete z;

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;
	if (a_fInnerRadius < 0.01f)
		a_fInnerRadius = 0.01f;
	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	Release();
	Init();

	//Your code starts here
	float outerValue = 0.5f * a_fOuterRadius;  //set outer radius
	float innerValue = 0.5f * a_fInnerRadius;  //set inner radius 
	float center = a_fHeight / 2;			   //set center on origin
	float incr = (2 * M_PI) / a_nSubdivisions; //set subdivision increment in radians
	float* xO = new float[a_nSubdivisions + 1]; //create array for set of outer x coordinates
	float* zO = new float[a_nSubdivisions + 1]; //create array for set of outer z coordinates
	float* xI = new float[a_nSubdivisions + 1]; //create array for set of inner x coordinates
	float* zI = new float[a_nSubdivisions + 1]; //create array for set of inner z coordinates

	for (int i = 1; i <= a_nSubdivisions; i++) {
		xO[i] = cos(incr * i) * outerValue; //increment subdivision and set x outer pos
		zO[i] = sin(incr * i) * outerValue; //increment subdivision and set z outer pos
		xI[i] = cos(incr * i) * innerValue; //increment subdivision and set x inner pos
		zI[i] = sin(incr * i) * innerValue; //increment subdivision and set z inner pos

		if (i == a_nSubdivisions) { //check if it is the last increment
			//draw quads for tops and bottoms
			AddQuad(vector3(xO[i - 1], -center, zO[i - 1]), vector3(xO[i], -center, zO[i]), vector3(xI[i - 1], -center, zI[i - 1]), vector3(xI[i], -center, zI[i]));
			AddQuad(vector3(xO[i], center, zO[i]), vector3(xO[i - 1], center, zO[i - 1]), vector3(xI[i], center, zI[i]), vector3(xI[i - 1], center, zI[i - 1]));

			//connect the quads 
			AddQuad(vector3(xO[i], -center, zO[i]), vector3(xO[i - 1], -center, zO[i - 1]), vector3(xO[i], center, zO[i]), vector3(xO[i - 1], center, zO[i - 1]));
			AddQuad(vector3(xI[i - 1], -center, zI[i - 1]), vector3(xI[i], -center, zI[i]), vector3(xI[i - 1], center, zI[i - 1]), vector3(xI[i], center, zI[i]));

			//do for first section
			AddQuad(vector3(xO[i], -center, zO[i]), vector3(xO[1], -center, zO[1]), vector3(xI[i], -center, zI[i]), vector3(xI[1], -center, zI[1]));
			AddQuad(vector3(xO[1], center, zO[1]), vector3(xO[i], center, zO[i]), vector3(xI[1], center, zI[1]), vector3(xI[i], center, zI[i]));

			AddQuad(vector3(xO[1], -center, zO[1]), vector3(xO[i], -center, zO[i]), vector3(xO[1], center, zO[1]), vector3(xO[i], center, zO[i]));
			AddQuad(vector3(xI[i], -center, zI[i]), vector3(xI[1], -center, zI[1]), vector3(xI[i], center, zI[i]), vector3(xI[1], center, zI[1]));

		}
		else if (i > 1) { //every other increment
			AddQuad(vector3(xO[i - 1], -center, zO[i - 1]), vector3(xO[i], -center, zO[i]), vector3(xI[i - 1], -center, zI[i - 1]), vector3(xI[i], -center, zI[i]));
			AddQuad(vector3(xO[i], center, zO[i]), vector3(xO[i - 1], center, zO[i - 1]), vector3(xI[i], center, zI[i]), vector3(xI[i - 1], center, zI[i - 1]));

			AddQuad(vector3(xO[i], -center, zO[i]), vector3(xO[i - 1], -center, zO[i - 1]), vector3(xO[i], center, zO[i]), vector3(xO[i - 1], center, zO[i - 1]));
			AddQuad(vector3(xI[i - 1], -center, zI[i - 1]), vector3(xI[i], -center, zI[i]), vector3(xI[i - 1], center, zI[i - 1]), vector3(xI[i], center, zI[i]));
		}
	}

	delete xO;
	delete zO;
	delete xI;
	delete zI;


	//Your code ends here
	CompileObject(a_v3Color);
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
	float fValue = 0.5f * a_fRadius;		   //set radius 
	float center = fValue / 2;				   //set it to center on origin
	vector3 pointS(0, -center, 0);			   //set south pole
	vector3 pointN(0, center, 0);			   //set north pole
	float latIncr = (fValue * 2) / a_nSubdivisions;  //set latitude increments
	float incr = (2 * M_PI) / a_nSubdivisions; //set subdivision increment in radians
	float divHeight = latIncr;
	bool odd = false;
	int latDiv = a_nSubdivisions / 2;
	std::cout << latDiv << std::endl;
	if (latDiv % 2 != 0)
		odd = true;
	float* x = new float[a_nSubdivisions + 1]; //create array for set of x coordinates
	float* z = new float[a_nSubdivisions + 1]; //create array for set of z coordinates
	float* y = new float[a_nSubdivisions + 1];

	for (int j = 0; j < latDiv; j++) {
		divHeight = latIncr * (j + 1);
		if (j == latDiv && odd) {

		}
		else if (j == 0) {
			for (int i = 1; i <= a_nSubdivisions; i++) {
				x[i] = cos(incr * i) * fValue; //increment subdivision and set x pos
				z[i] = sin(incr * i) * fValue; //increment subdivision and set z pos

				if (i == a_nSubdivisions) { //check if it is the last increment
					//draw triangle from point to previous point
					/*AddVertexPosition(pointS);
					AddVertexPosition(vector3(x[i - 1], -center, z[i - 1]));
					AddVertexPosition(vector3(x[i], -center, z[i]));

					//do for first position
					AddVertexPosition(pointS);
					AddVertexPosition(vector3(x[i], -center, z[i]));
					AddVertexPosition(vector3(x[1], -center, z[1]));*/
				}
				else if (i == 1) { //every other increment
					AddVertexPosition(pointS);
					AddVertexPosition(vector3(x[i - 1], -center + divHeight, z[i - 1]));
					AddVertexPosition(vector3(x[i], -center + divHeight, z[i]));

					std::cout << " x: " << x << std::endl;
					std::cout << " z: " << z << std::endl;
					std::cout << " y: " << -center + divHeight << std::endl;
					std::cout << "div height:" << divHeight << std::endl;
				}
			}
		}
		else {

		}
	}



	//Your code ends here
	CompileObject(a_v3Color);
}