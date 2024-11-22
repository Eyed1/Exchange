#include <iostream>
using namespace std;

class A {
public:
    virtual void f() final { // Mark the function as final
        cout << "This is function f in class A" << endl;
    }
};

class B : public A {
public:
    // Even without 'override', this will cause a compilation error
    void f() { // Attempting to redefine f
        cout << "Trying to redefine function f in class B" << endl;
    }
};

int main() {
    A a;
    a.f();

     //Uncommenting the following lines will cause a compilation error
    B b;
    b.f();

    return 0;
}
