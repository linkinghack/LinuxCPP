#include <iostream>

using namespace std;

class Base1
{
public:
    void display()
    {
        cout << "Base1.display()" << endl;
    }

    virtual void display2()
    {
        display();
    }
};

class Base2 : public Base1
{
public:
    void display()
    {
        cout << "Base2.display()" << endl;
    }

    virtual void display2() override final  // 不可再被override
    {
        display();
    }
};

class Derived : public Base1
{
public:
    void display()
    {
        cout << "Derived.display()" << endl;
    }

    virtual void display2() override
    {
        display();
    }

    //  const 可以作为函数重载的区别标识
    void hello(){
        cout<<"hello()"<<endl;
    };
    void hello() const {
        cout<<"hello() const"<<endl;
    };
};

void display(Base1 *p)
{
    p->display();
}

void display2(Base1 *p)
{
    p->display2();
}

int main(void)
{
    Base1 b1;
    Base2 b2;
    Derived d;
    d.hello();

    cout<<"Normal function: "<<endl;
    // d.Base1::display();
    display(&b1);
    display(&b2);
    display(&d);

    cout<<"virtual function: "<<endl;
    // virtual function
    display2(&b1);
    display2(&b2);
    display2(&d);

    d.hello(); // 调用非const
    return 0;
}