#include <iostream>

using namespace std;

class Complex
{
private:
    int a, b;

public:
    Complex(int a, int b) : a(a), b(b)
    {
        printf("Constructor: a=%d, b=%d\n", a, b);
    };
    Complex(const Complex &origin) {
        a = origin.a;
        b = origin.b;
        cout<<"Copy constructor called"<<endl;
    }

    void display()
    {
        cout << a << "+" << b << "i" << endl;
    };
    // 除后置 ++ -- 外，运算符重载函数参数个数为运算符参数个数-1
    // 双目运算符1个参数代表右值
    //
    Complex operator+(const Complex &c);
    Complex operator-(const Complex &c);
    // 成员函数运算符重载不可使用静态函数

    // 前置单目运算符 无参数
    Complex operator-()
    {
        return Complex(-a, -b);
    }

    // 前置++ 运算符没有参数
    Complex& operator++ ();
    // 为加以区分同样一元的后置++(--), 添加一个int参数, 此参数无作用, 仅用于区分重载函数
    Complex operator++ (int);
    
    friend bool operator==(const Complex& a, const Complex& b);
    friend ostream& operator<<(ostream& out, const Complex& c);

};

/**
 * 运算符重载为类内非静态成员函数
 */ 
Complex Complex::operator+(const Complex &c)
{
    return Complex(a + c.a, b + c.b);
}

Complex Complex::operator-(const Complex &c)
{
    return Complex(a - c.a, b - c.b);
}

// 注意返回值为引用
Complex& Complex::operator++() {
    // 前置自增运算
    a++;
    return *this;
}

Complex Complex::operator++(int) {
    // 后置自增运算
    // 好设计是在后置中调用重载的前置运算,
    // 以便在自增运算逻辑做出修改后同步更新到后置中
    Complex old = (*this); // 拷贝构造，返回副本
    ++(*this);
    return old;
}

/**
 * 运算符重载为类外函数
 * 用于解决 运算符作用于不同类的情况
 * 比如： 基本类型 + 自定义类型
 *        Class A + Class B ...
 * 
 * 规则：
 *  函数形参代表自左至右排列的各操作数
 *  参数个数＝原操作个数
 *  至少有一个自定义类型的参数（不能重载对于基本类型的运算）
 *  后置单目运算符需要参数列表中添加一个int,不需要形参名
 *  
 * 如果在运算符重载函数中需要操作某类对象的私有成员，可以将该函数声明为该类的友元
 */

// << 运算符只能重载为非成员函数, 因为左值是另一个类(比如count <std::ostream>)

// 重载 ==关系运算
// 需要声明为Complex的友元
bool operator==(const Complex& a, const Complex& b) {
    // 注意参数是 常引用
    return a.a == b.a && a.b == b.b;
}

ostream& operator<<(ostream& out, const Complex& a) {
    out<< a.a << "+" << a.b <<"i";
    return out;
}

int main(int argc, char const *argv[])
{
    Complex x(2, 3), y(3, 4);
    Complex z = x - y;
    z.display();
    z.operator+(x).display(); // 运算符重载函数可以像普通成员函数一样调用

    ++x;
    x++;

    cout<<x<<endl;
    cout<< (x == y) <<endl;

    return 0;
}
