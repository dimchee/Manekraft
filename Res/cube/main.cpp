#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Vec3
{
    float x, y, z;
    Vec3(): x(0.0), y(0.0), z(0.0) {}
    Vec3(float x, float y, float z): x(x), y(y), z(z) {}
    string str() { return to_string(x) + ", " + to_string(y) + ", " + to_string(z); }
    friend ostream& operator << (ostream& os, Vec3& a) { return os << a.str(); }
};

struct side
{
    Vec3 v[4];
};

int main()
{
    Vec3 pos[8]; // temena
    (void)pos;
    ofstream file("Test");
    file << "test" << endl;
    return 0;
}