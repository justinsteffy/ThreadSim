#include <math.h>		
#include <stdlib.h>
//I am using my own matrix structure, it is used for creating rotation matrices and apply to vectors
//inverse and transpose functions are adapted (copy/paste) from Donald House' code.

#ifndef Z_MATRIX3
#define Z_MATRIX3

#include <stdio.h>
#include "vector_inline.h"

#define PI 3.141527

class matrix{
public:
	float data[3][3];
	inline matrix(){for (int i=0;i<3;i++) for (int j=0;j<3;j++) data[i][j]=0.0;};
	inline matrix &rotate(vector3 &b);
	inline vector3 &tranformate(vector3 &b);
	inline matrix &identity();
	inline matrix &zero();
	inline matrix &inertia(vector3 &b);
	inline matrix& operator*=(matrix &b);
	inline matrix& operator=(matrix &b);
	inline matrix operator*(const matrix &b);
	inline vector3 operator*(const vector3 &b);
	inline matrix &operator*=(float b);
	inline matrix& operator+=(matrix &b);
	inline matrix operator+(const matrix &b);
	inline matrix& operator-=(matrix &b);
	inline matrix operator-(const matrix &b);
	inline matrix &inverse();
	inline matrix &transpose();
};

inline matrix &matrix::operator *=(float b)
{
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			data[i][j]*=b;
	return *this;
}

inline matrix& matrix::identity()
{
	matrix a;
	*this=a;
	for (int i=0;i<3;i++) 
		data[i][i]=1.0;
	return *this;
}

inline matrix& matrix::zero()
{
	matrix a;
	*this=a;
	return *this;
}

inline matrix& matrix::inertia(vector3 &b)
{
	matrix a;
	matrix c;
	c.identity();
	for (int i=0;i<3;i++)
		for (int j=0;j<3;j++)
			a.data[i][j]=b[i]*b[j];
	c*=b[0]*b[0]+b[1]*b[1]+b[2]*b[2];
	*this=c-a;
	return *this;
}

inline matrix& matrix::operator=(matrix &b)
{
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			data[i][j]=b.data[i][j];
	return *this;
}

inline matrix matrix::operator*(const matrix &b)
{
	matrix a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			for (int k=0;k<3;k++) 
				a.data[i][j]+=data[k][j]*b.data[i][k];
	return a;
}

inline matrix matrix::operator+(const matrix &b)
{
	matrix a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			a.data[i][j]+=b.data[i][j];
	return a;
}

inline matrix matrix::operator-(const matrix &b)
{
	matrix a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			a.data[i][j]-=b.data[i][j];
	return a;
}

inline vector3 matrix::operator*(const vector3 &b)
{
	vector3 a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
				(a[i])+=(data[i][j])*(b[j]);
	return a;
}

inline matrix& matrix::operator*=(matrix &b)
{
	matrix a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			for (int k=0;k<3;k++) 
				a.data[i][j]+=data[k][j]*b.data[i][k];
	*this=a;
	return *this;
}

inline matrix& matrix::operator+=(matrix &b)
{
	matrix a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			a.data[i][j]=data[i][j]+b.data[i][j];
	*this=a;
	return *this;
}

inline matrix& matrix::operator-=(matrix &b)
{
	matrix a;
	for (int i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			a.data[i][j]=data[i][j]-b.data[i][j];
	*this=a;
	return *this;
}

inline matrix& matrix::rotate(vector3 &b)
{
	int i;
	float r[3];
	for (i=0;i<3;i++)
		r[i]=b[i]/180.0*PI;
	for (i=0;i<3;i++) 
		for (int j=0;j<3;j++) 
			data[i][j]=0.0;
	matrix rx,ry,rz;
	rx.identity();
	ry.identity();
	rz.identity();
	rx.data[1][1]=cos(r[0]);
	rx.data[2][2]=cos(r[0]);
	rx.data[1][2]=-sin(r[0]);
	rx.data[2][1]=sin(r[0]);

	ry.data[0][0]=cos(r[1]);
	ry.data[2][2]=cos(r[1]);
	ry.data[0][2]=sin(r[1]);
	ry.data[2][0]=-sin(r[1]);

	rz.data[0][0]=cos(r[2]);
	rz.data[1][1]=cos(r[2]);
	rz.data[0][1]=-sin(r[2]);
	rz.data[1][0]=sin(r[2]);
	matrix a;
	a=rx;
	a*=ry;
	a*=rz;
	*this=a;
	return *this;
}

inline vector3& matrix::tranformate(vector3 &b)
{
	int i;
	vector3 a;
	float k=0.0;
	a.vector3D(0.0,0.0,0.0);
	for (i=0;i<3;i++)
		for (int j=0;j<3;j++)
			a[j]+=b[i]*data[j][i];
	b=a;
	return b;
}

inline matrix &matrix::inverse() 
{
  double d;
  matrix invM;
  vector3 row[3];
 
  d = data[0][0]*data[1][1]*data[2][2] + data[0][1]*data[1][2]*data[2][1] +
      data[0][2]*data[2][1]*data[1][0] - data[0][2]*data[1][1]*data[2][1] - 
      data[0][1]*data[1][0]*data[2][2] - data[0][0]*data[2][1]*data[1][2];

  if(d == 0.0){
    printf("Matrix inversion error\n");
	exit(0);
  }
  
  invM.data[0][0]= (data[1][1]*data[2][2] - data[1][2]*data[2][1]) / d;
  invM.data[0][1]= (data[0][2]*data[2][1] - data[0][1]*data[2][2]) / d;
  invM.data[0][2]= (data[0][1]*data[1][2] - data[0][2]*data[1][1]) / d;
  invM.data[1][0]= (data[1][2]*data[2][0] - data[1][0]*data[2][2]) / d;
  invM.data[1][1]= (data[0][0]*data[2][2] - data[0][2]*data[2][0]) / d;
  invM.data[1][2]= (data[0][2]*data[1][0] - data[0][0]*data[1][2]) / d;
  invM.data[2][0]= (data[1][0]*data[2][1] - data[1][1]*data[2][0]) / d;
  invM.data[2][1]= (data[0][1]*data[2][0] - data[0][0]*data[2][1]) / d;
  invM.data[2][2]= (data[0][0]*data[1][1] - data[0][1]*data[1][0]) / d;

  *this=invM;
  return *this;
}

inline matrix &matrix::transpose()
{
  matrix transM;
 
  transM.data[0][0] = data[0][0];
  transM.data[1][0] = data[0][1];
  transM.data[2][0] = data[0][2];
  transM.data[0][1] = data[1][0];
  transM.data[1][1] = data[1][1];
  transM.data[2][1] = data[1][2];
  transM.data[0][2] = data[2][0];
  transM.data[1][2] = data[2][1];
  transM.data[2][2] = data[2][2];

  *this=transM;
  return *this;
}


#endif
