#ifndef CAMERA
#define CAMERA

#include <Src/Core/Mat.h>
#include <Src/Core/Shader.h>
#include <Src/Core/Block.h>

template<typename T, T (*norm)(const T&)>
struct Norm
{
    T val;
    Norm(): val() {}
    Norm(const T& x): val(x) {}
    T& operator = (const T& x) { return val = norm(x); }
    T& operator +=(const T& x) { return val = norm(val+=x); }
    T& operator -=(const T& x) { return val = norm(val-=x); }
    T& operator *=(const T& x) { return val = norm(val*=x); }
    T& operator /=(const T& x) { return val = norm(val/=x); }
    operator T () const { return val; }
};

struct Euler
{
    static float PitchNorm(const float &x) { return (x>1.57f? 1.57f:x)<-1.57f? -1.57f:x; }
    static float AngleNorm(const float &x) { return x - round(x/2/M_PI)*2*M_PI; }
    Norm<float, PitchNorm> pch;
    Norm<float, AngleNorm> yaw;
    Norm<float, AngleNorm> rol;
    Mat& ToMat()
    {
        return mMat = Mat::Mul(Mat::Euler(0.0f, 0.0f, pch), Mat::Euler(yaw, 0.0f, 0.0f));
    }
private:
    Mat mMat;
};

class Camera
{
private:
    Euler mEu;
    Mat mRot;
    Mat mPos;
    Mat mView;
    Mat mProj;
public:
    static vector<unique_ptr<Object>>::iterator select;
    static Vec3 selDir;
    static Camera *main;
    Vec3 Dir() { return mRot.Dir(); }
    Vec3 Up()  { return mRot.Up();  }
    Vec3 Rig() { return mRot.Rig(); }
    Vec3 Pos() { return {mPos[0][3], mPos[1][3], mPos[2][3]}; }
    Camera(Vec3 pos, float fov, float ratio): mPos(Mat::Trans(-pos)), mProj(Mat::Proj(fov, ratio, 0.1, 100))
    {
        main = this;
    }
    Camera(const Camera& cam) = delete;
    void operator = (const Camera& cam) = delete;
    Mat& View() { return mView; }
    Mat& Proj() { return mProj; }
    void Move(Vec3 d) { mPos[0][3] -= d.x, mPos[1][3] -= d.y, mPos[2][3] -= d.z; }
    void Rotate(float y, float p) { mEu.yaw+=y; mEu.pch+=p; }
    void Update(Shader& sh)
    {
        mView = (mRot = mEu.ToMat())*mPos;
        sh.use();
        sh.set("proj", Proj());
        sh.set("view", View());
    }
    void Intersect()
    {
        Vec3 dir = Dir();
        Vec3 a = {1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z};
        Vec3 pos = Pos(); float d, mind = 5.0f;
        for(auto it = Manager.world.begin(); it < Manager.world.end(); it++) if(*it)
            if((d = (*it)->FastIntersect(pos, a)) < mind)
            {
                mind = d, select = it;
                selDir = pos + dir*mind - (*it)->pos; // not working :P
                selDir.normalize();
            }
        if(mind == 5.0f) select = Manager.world.end();
    }
}; Camera *Camera::main = nullptr;
vector<unique_ptr<Object>>::iterator Camera::select;
Vec3 Camera::selDir;

#endif
