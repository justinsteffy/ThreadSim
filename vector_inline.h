/* vector_inline.h
	My own vector class, not so well written but I am used to work with it
	Initially created for Traceray ray tracer, march 2001
	Pros: 
		I am used to work with it, and too lazy to rewrite
		Every function is inline, for optimization purposes
	Cons:
		Somehow I have problems defining binary operators even using textbook functions, so I used mostly unary operators
		Update: Hey, they are working, maybe it was my version of visual studio, I dont know.
		no more cons than, la bella vita
*/

#ifndef _VECTORINLINE
#define _VECTORINLINE

#include <math.h>
#include <iostream>

class vector3 {
	float	data[3];
public:
	int		size;
	
	inline vector3(int m=3){size=m;data[0]=data[1]=data[2]=0.0;};
	inline vector3(float a,float b,float c){size=3;data[0]=a;data[1]=b;data[2]=c;};

	inline vector3 &operator=(vector3 &a);
	inline vector3 &operator=(float &a);
	inline short operator==(vector3 &a);
	inline short operator!=(vector3 &a);
	inline float &operator[](int index){return data[index];};
	inline float operator[](int index) const{return data[index];};
	inline float length(){return sqrt(pow(data[0],2)+pow(data[1],2)+pow(data[2],2));};
	inline vector3 &floor();
	inline vector3 &ceiling();
	inline vector3 &round();

	inline vector3 &operator+=(vector3 &b);
	inline vector3 &operator-=(vector3 &b);
	inline vector3 &operator*=(float index);
	inline vector3 &operator/=(float index);
	inline vector3 &operator*=(vector3 &b);
	inline vector3 &operator/=(vector3 &b);
	
	inline vector3 &cross(vector3 &b);
	inline vector3 &normalize();
	inline vector3 &normalize(float b);
	inline float dot(vector3 &b);
	inline vector3 &vector3D(float a, float b, float c);

	friend vector3 operator*(float f, vector3 &a);
	friend vector3 operator/(float f, vector3 &a);
	friend vector3 operator*(vector3 &b, vector3 &a);
	friend vector3 operator/(vector3 &b, vector3 &a);
	friend vector3 operator/(vector3 &b, float a);
	friend vector3 operator*(vector3 &a, float f);
	friend vector3 operator+(vector3&a, vector3 &b);
	friend vector3 operator-(vector3&a, vector3 &b);

	friend std::ostream& operator<<(std::ostream &os, vector3 &b);
	friend std::istream& operator>>(std::istream &os, vector3 &b);

};

static
std::ostream& operator<<(std::ostream &os, vector3 &b)
{
	return os << b.data[0] <<" "<<b.data[1]<<" "<<b.data[2];
}

static
std::istream& operator>>(std::istream &os, vector3 &b)
{
    return os >> b.data[0] >>b.data[1]>>b.data[2];
}

inline vector3 &vector3::normalize(float b)
{
	normalize();
	for (int i=0;i<3;i++)
		data[i]*=b;
	return *this;
}

inline float minimum(float a,float b)
{
	return (a<=b)? a:b;
}

inline float maximum(float a,float b)
{
	return (a>=b)? a:b;
}

inline vector3 minimum(vector3 &a,vector3 &b)
{
	vector3 result;
		for (int i=0;i<3;i++)
			result[i]=minimum(a[i],b[i]);
	return result;
}

inline vector3 maximum(vector3 &a,vector3 &b)
{
	vector3 result;
		for (int i=0;i<3;i++)
			result[i]=maximum(a[i],b[i]);
	return result;
}

inline vector3 normalize(vector3 &p)
{
	vector3 result=p;
	return result.normalize();
}

inline vector3 floor(vector3 &p)
{
	vector3 result=p;
	return result.floor();
}

inline vector3 ceiling(vector3 &p)
{
	vector3 result=p;
	return result.ceiling();
}

inline vector3 round(vector3 &p)
{
	vector3 result=p;
	return result.round();
}

inline short vector3::operator==(vector3 &b)
{
	bool result=true;
	for (int i=0;i<size;i++)
		result=result&&(data[i]==b.data[i]);
	return result;
}

inline short vector3::operator!=(vector3 &b)
{
	bool result=false;
	for (int i=0;i<size;i++)
		result=result||(data[i]!=b.data[i]);
	return result;
}


inline vector3 &vector3::normalize()
{
	int i;
	float scalar=0.0;
	for (i=0;i<size;i++)
		scalar+=data[i]*data[i];
	scalar=sqrt(scalar);
	if (scalar!=0.0)
		for (i=0;i<size;i++)
			data[i]/=scalar;
	return *this;
}

inline float vector3::dot(vector3 &b)
{
	float result=0;
	for (int i=0;i<size;i++)
		result+=data[i]*b[i];
	return result;
}

inline vector3 &vector3::cross(vector3 &b)
{
	vector3 tmp;
	for (int i=0;i<size;i++)
		tmp[i]=data[(i+1)%size]*b[(i+2)%size]-data[(i+2)%size]*b[(i+1)%size];
	*this=tmp;
	return	*this;
}

inline vector3 &vector3::operator=(vector3 &a)
{
	for (int i=0;i<size;i++)
		data[i]=a[i];
	return *this;
}

inline vector3 &vector3::operator=(float &a)
{
	for (int i=0;i<size;i++)
		data[i]=a;
	return *this;
}

inline vector3 &vector3::operator*=(float index)
{
	for (int i=0;i<size;i++)
		data[i]*=index;
	return *this;
}

inline vector3 &vector3::operator/=(float index)
{
	for (int i=0;i<size;i++)
		data[i]/=index;
	return *this;
}

inline vector3 &vector3::operator*=(vector3 &b)
{
	for (int i=0;i<size;i++)
		data[i]*=b.data[i];
	return *this;
}

inline vector3 &vector3::operator/=(vector3 &b)
{
	for (int i=0;i<size;i++)
		data[i]/=b.data[i];
	return *this;
}

inline vector3 &vector3::operator+=(vector3 &b)
{
	for (int i=0;i<size;i++)
		data[i]+=b[i];	
	return *this;
}

inline vector3 &vector3::operator-=(vector3 &b)
{
	for (int i=0;i<size;i++)
		data[i]-=b[i];	
	return *this;
}

inline vector3 &vector3::vector3D(float a, float b, float c)
{
	size=3;
	data[0]=a;
	data[1]=b;
	data[2]=c;
	return *this;
}

inline vector3 operator*(vector3& a, float f)
{
	vector3 result=a;
	result *=f;
	return result;
}

inline vector3 operator*(float f, vector3& a)
{
	vector3 result=a;
	result*=f;
	return result;
}

inline vector3 operator/(float f, vector3& a)
{
	vector3 result;
	for (int i=0;i<a.size;i++)
		result[i]=f/a[i];
	return result;
}

inline vector3 operator*(vector3 &b, vector3& a)
{
	vector3 result;
	for (int i=0;i<a.size;i++)
		result[i]=a[i]*b[i];
	return result;
}

inline vector3 operator/(vector3 &b, vector3& a)
{
	vector3 result;
	for (int i=0;i<a.size;i++)
		result[i]=b[i]/((a[i]!=0) ? a[i]:1E-50);
	return result;
}

inline vector3 operator/(vector3 &b, float a)
{
	vector3 result;
	for (int i=0;i<b.size;i++)
		result[i]=b[i]/((a!=0) ? a:1E-50);
	return result;
}

inline vector3 operator+(vector3 &a, vector3 &b)
{
	vector3 result=a;
	result+=b;
	return result;
}

inline vector3 operator-(vector3 &a,vector3 &b)
{
	vector3 result=a;
	result-=b;
	return result;
}

inline vector3 &vector3::floor()
{
	int j;
	for (int i=0;i<3;i++)
		data[i]=j=data[i];
	return *this;
}

inline vector3 &vector3::ceiling()
{
	int j;
	for (int i=0;i<3;i++)
		data[i]=j=data[i]+0.9999999;
	return *this;
}

inline vector3 &vector3::round()
{
	int j;
	for (int i=0;i<3;i++)
		data[i]=j=data[i]+0.5;
	return *this;
}


#endif