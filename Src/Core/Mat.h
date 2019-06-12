#ifndef MAT
#define MAT

#include <vector>
#include <cmath>
#include <iostream>
#include <cassert>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

using namespace std;

struct Vec2
{
    float x, y;
    Vec2() : x(0.0f), y(0.0f) {}
    Vec2(float x, float y) : x(x), y(y) {}
    string str() { return "(" + to_string(x) + ", " + to_string(y) + ")";  } 
    friend ostream& operator << (ostream& os, Vec2 a) { return os << a.str(); }
};

struct Vec3
{
    float x, y, z;
    Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vec3(double x, double y, double z) : x(x), y(y), z(z) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator * (Vec3 a) { return {y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x}; }
    Vec3 operator + (Vec3 a) { return {x+a.x, y+a.y, z+a.z}; }
    Vec3 operator - (Vec3 a) { return {x-a.x, y-a.y, z-a.z}; }
    Vec3 operator * (float a) { return {x*a, y*a, z*a}; }
    Vec3 operator / (float a) { return {x/a, y/a, z/a}; }
    Vec3 operator - () { return {-x, -y, -z}; }
    void operator += (Vec3 a) { x+=a.x; y+=a.y; z+=a.z; }
    void operator -= (Vec3 a) { x-=a.x; y-=a.y; z-=a.z; }
    void operator = (const Vec3& a) { x=a.x; y=a.y; z=a.z; }

    friend float dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
    float len() { return sqrt(x*x + y*y + z*z); }
    Vec3 norm() { return *this / len(); }
    void normalize() { float l = len(); x/=l; y/=l; z/=l; }
    string str() { return "(" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")"; }
    friend ostream& operator << (ostream& os, Vec3 a) { return os << a.str(); }
};

struct Vec4
{
    float w, x, y, z;
    Vec4() : w(0.0f), x(0.0f), y(0.0f), z(0.0f) {}
    Vec4(float w, float x, float y, float z): w(w), x(x), y(y), z(z) {}
    Vec4 operator * (Vec4 a)
    {
        return {
            w*a.w - x*a.x - y*a.y + z*a.z,
            w*a.x + x*a.w + y*a.z - z*a.y,
            w*a.y - x*a.z + y*a.w + z*a.x,
            w*a.z + x*a.y - y*a.x + z*a.w
        };
    }
    Vec4 operator + (Vec4 a) { return {w+a.w, x+a.x, y+a.y, z+a.z}; }
    Vec4 operator - (Vec4 a) { return {w-a.w, x-a.x, y-a.y, z-a.z}; }
    Vec4 operator * (float a) { return {w*a, x*a, y*a, z*a}; }
    Vec4 operator / (float a) { return {w/a, x/a, y/a, z/a}; }
    Vec4 operator - () { return {-w, -x, -y, -z}; }
    Vec4 conj() { return {w, -x, -y, -z}; }
    void operator += (Vec4 a) { w+=a.w; x+=a.x; y+=a.y; z+=a.z; }
    void operator -= (Vec4 a) { w-=a.w; x-=a.x; y-=a.y; z-=a.z; }
    void operator = (const Vec4& a) { w=a.w; x=a.x; y=a.y; z=a.z; }

    float len() { return sqrt(w*w + x*x + y*y + z*z); }
    Vec4 norm() { return *this / len(); }
    void normalize() { float l = len(); w/=l; x/=l; y/=l; z/=l; }
    string str() { return "(" + to_string(w) +", ["+ to_string(x) + ", " + to_string(y) + ", " + to_string(z) + "])"; }
    friend ostream& operator << (ostream& os, Vec4 a) { return os << a.str(); }
    static Vec4 FromVec3(Vec3 a)
    {
        //      {0, 0, 1} * a = {-a.y, a.x, 0}
        //      {0, 1, 0} * a = {a.z, 0, -a.x};
        return Vec4(a.len() + a.y, -a.y, a.x, 0).norm();
    }
    void Look(Vec3 a) { w = a.len() + a.y; x = a.y; y = a.x; z = 0; normalize(); }
    Vec3 vec() { return {x, y, z}; }
    Vec3 Rig() { return {1 - 2*y*y - 2*z*z, 2*x*y - 2*z*w, 2*x*z + 2*y*w}; }
    Vec3 Up()  { return {2*x*y + 2*z*w, 1 - 2*x*x - 2*z*z, 2*y*z - 2*x*w}; }
    Vec3 Dir() { return {2*x*z - 2*y*w, 2*y*z + 2*x*w, 1 - 2*x*x - 2*y*y}; }
}; typedef Vec4 Quat;

struct Mat
{
private:
    vector<float> data;
    struct indexer
    {
        int y; vector<float>& dat;
        indexer(int y, vector<float>& dat): y(y), dat(dat) {}
        float& operator [](int x) { return dat.at(4*y+x); }
        string str() 
        { 
            return "[" + 
            to_string((*this)[0]) + ", " + 
            to_string((*this)[1]) + ", " + 
            to_string((*this)[2]) + ", " + 
            to_string((*this)[3]) + "]\n";
        }
    };
public:
    void clear() { for(auto& x : data) x=0; }
    Mat(): data(16) { data[0]=data[5]=data[10]=data[15]=1; }
    Mat(float a): data(16, a) {}
    Mat(vector<float>& vec): data(vec) { data.resize(16); }
    Mat(const Mat& a): data(a.data) {}
    Mat(const Quat& a): data(16)
    {
        data[0] = 1 - 2*a.y*a.y - 2*a.z*a.z;
        data[1] = 2*a.x*a.y - 2*a.z*a.w;
        data[2] = 2*a.x*a.z + 2*a.y*a.w;
        data[4] = 2*a.x*a.y + 2*a.z*a.w;
        data[5] = 1 - 2*a.x*a.x - 2*a.z*a.z;
        data[6] = 2*a.y*a.z - 2*a.x*a.w;
        data[8] = 2*a.x*a.z - 2*a.y*a.w;
        data[9] = 2*a.y*a.z + 2*a.x*a.w;
        data[10]= 1 - 2*a.x*a.x - 2*a.y*a.y; 
        data[15]=1;
    }
    Mat(const Quat& a, const Vec3& b): Mat(a)
    {
        data[3 ] = b.x;
        data[7 ] = b.y;
        data[11] = b.z;
    }
    Mat& operator = (const Mat& a) { data = a.data; return *this; } 
    indexer operator [](int y) { return {y, data}; }
    friend Mat operator * (Mat& a, Mat& b)
    {
        Mat sol(0);
        for(int i=0; i<4; i++)
            for(int j=0; j<4; j++)
                for(int k=0; k<4; k++)
                    sol[i][j] += a[i][k] * b[k][j];
        return sol;
    }
    static Mat Mul (Mat a, Mat b) { return a*b; }
    float* raw() { return &data.front(); }
    static Mat Proj(float fov, float r, float n, float f)
    {
        assert(n > 1e-13 || n < -1e-13);
        float c = 1.0/tan(fov/2);
        vector<float> a = {
            c/r, 0, 0, 0,
            0, c, 0, 0,
            0, 0, (f+n)/(n-f), 2*f*n/(n-f),
            0, 0, -1, 0
        }; return a;
    }
    static Mat Rotate(float Phi, Vec3 R)
    {
        R.normalize();
        float c = cos(Phi), s = sin(Phi);
        vector<float> a = {
            c + R.x*R.x*(1-c), R.x*R.y*(1-c) - R.z*s, R.x*R.z*(1-c) + R.y*s, 0,
            R.y*R.x*(1-c) + R.z*s, c + R.y*R.y*(1-c), R.y*R.z*(1-c) - R.x*s, 0,
            R.z*R.x*(1-c) - R.y*s, R.z*R.y*(1-c) + R.x*s, c + R.z*R.z*(1-c), 0,
            0, 0, 0, 1
        }; return a;
    }
    static Mat Trans(Vec3 T)
    {
        vector<float> a = {
            1, 0, 0, T.x,
            0, 1, 0, T.y,
            0, 0, 1, T.z,
            0, 0, 0, 1
        }; return a;
    }
    static Mat Scale(Vec3 S)
    {
        vector<float> a = {
            S.x, 0, 0, 0,
            0, S.y, 0, 0,
            0, 0, S.z, 0,
            0, 0, 0, 1
        }; return a;
    }
    static Mat LookAt(Vec3 R, Vec3 U, Vec3 D)
    {
        R.normalize(); U.normalize(); D.normalize();
        vector<float> a = {
            R.x, R.y, R.z, 0,
            U.x, U.y, U.z, 0,
            D.x, D.y, D.z, 0,
            0,   0,   0,   1
        }; return a;
    }
    static Mat Euler(float yaw, float pitch, float roll)
    {
        float   sa = sin(pitch), //attitude
                ca = cos(pitch),
                sb = sin(roll),  //bank
                cb = cos(roll),
                sh = sin(yaw),   //heading
                ch = cos(yaw);
        vector<float> a = {
            ch*ca, -ch*sa*cb + sh*sb, ch*sa*sb + sh*cb, 0,
            sa, ca*cb, -ca*sb, 0,
            -sh*ca, sh*sa*cb + ch*sb, -sh*sa*sb + ch*cb, 0,
            0, 0, 0, 1
        }; return a;
    }
    Vec3 Rig() { return {data[0], data[1], data[2]}; }
    Vec3 Up()  { return {data[4], data[5], data[6]}; }
    Vec3 Dir() { return {data[8], data[9], data[10]}; }
    string str() 
    {
        return  (*this)[0].str() + 
                (*this)[1].str() + 
                (*this)[2].str() + 
                (*this)[3].str();
    } 
    friend ostream& operator << (ostream& os, Mat& a) { return os << a.str(); }
};

ostream& operator << (ostream& os, vector<float>& a)
{
    for(float x : a) os << x << " ";
    return os << endl;
}

#endif