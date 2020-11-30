
#include "compose.h"
#include <functional>
#include <iostream>

using std::cout;
using std::endl;


void FailWithMsg(const std::string &msg, int line) {
    std::cerr << "Test failed!\n";
    std::cerr << "[Line " << line << "] " << msg << std::endl;
    std::exit(EXIT_FAILURE);
}


#define ASSERT_TRUE_MSG(cond, msg) \
    if (!(cond)) { FailWithMsg(msg #cond, __LINE__); };


int test_func(int a, int b) {
    return a * a + b;
}

struct Foo {
    Foo(int num) : _num(num) {}
    int mul(int i) { return _num * i; }
    int add(int i) { return _num + i; }

    int _num;
};
int f(int x) { return x + 1; }
int g(int x) { return x * 2; }
int h(int x) { return x - 1; }
double j(double x) {
    std::cout << "j" << endl;
    return x * 1.1;
}

int main() {

    // constructors tests
    Function<decltype(test_func)> test_func_object1(test_func);
    auto test_func_object2 = Function<int(int, int)>([](int a, int b) { return a - b; });
    Function<int(int, int)> test_func_object3(test_func_object2);

    ASSERT_TRUE_MSG(test_func_object1(2, 1) == 5, "Assertion failed. Init constructor: ");
    ASSERT_TRUE_MSG(test_func_object2(2, 1) == 1, "Assertion failed. Init constructor w lambda: ");
    ASSERT_TRUE_MSG(test_func_object3(2, 1) == 1, "Assertion failed. Copy constructor: ");

    // std::swap tests
    std::swap(test_func_object1, test_func_object2);
    ASSERT_TRUE_MSG(test_func_object1(2, 1) == 1, "Assertion failed. std::swap : ");
    ASSERT_TRUE_MSG(test_func_object2(2, 1) == 5, "Assertion failed. std::swap : ");
    ASSERT_TRUE_MSG(test_func_object3(2, 1) == 1, "Assertion failed. std::swap : ");

    //  Function::swap tests
    test_func_object3.swap(test_func_object2);
    ASSERT_TRUE_MSG(test_func_object1(2, 1) == 1, "Assertion failed. swap method :");
    ASSERT_TRUE_MSG(test_func_object2(2, 1) == 1, "Assertion failed. swap method :");
    ASSERT_TRUE_MSG(test_func_object3(2, 1) == 5, "Assertion failed. swap method :");

    //  std::move tests
    Function<int(int, int)> test_func_object4 = std::move(test_func_object3);
    ASSERT_TRUE_MSG(!test_func_object3, "Assertion failed. move constructor or bool() method :");
    ASSERT_TRUE_MSG(test_func_object4, "Assertion failed. move constructor  or bool() method :");
    ASSERT_TRUE_MSG(test_func_object4(2, 1) == 5, "Assertion failed. move constructor method :");


    // member wrapper method  tests
    Function<int(Foo, int)> test_member_func_1 = &Foo::mul;
    Function<int(Foo, int)> test_member_func_2 = &Foo::add;
    Foo foo(2), bar(15);

    ASSERT_TRUE_MSG(test_member_func_1(foo, 10) == 20, "Assertion failed. Member Function  call:");
    ASSERT_TRUE_MSG(test_member_func_1(bar, 2) == 30, "Assertion failed. Member Function  call:");
    ASSERT_TRUE_MSG(test_member_func_2(foo, 10) == 12, "Assertion failed. Member Function  call:");
    ASSERT_TRUE_MSG(test_member_func_2(bar, 2) == 17, "Assertion failed. Member Function  call:");

    std::swap(test_member_func_1, test_member_func_2);
    ASSERT_TRUE_MSG(test_member_func_1(foo, 10) == 12, "Assertion failed. std::swap Member Function  call:");
    ASSERT_TRUE_MSG(test_member_func_1(bar, 2) == 17, "Assertion failed. std::swap Member Function  call:");
    ASSERT_TRUE_MSG(test_member_func_2(foo, 10) == 20, "Assertion failed. std::swap Member Function  call:");
    ASSERT_TRUE_MSG(test_member_func_2(bar, 2) == 30, "Assertion failed. std::swap Member Function  call:");

    test_member_func_1 = std::move(test_member_func_2);
    ASSERT_TRUE_MSG(!test_member_func_2, "Assertion failed. move constructor or bool() method for member function :");
    ASSERT_TRUE_MSG(test_member_func_1, "Assertion failed. move constructor or bool() method for member function :");
    ASSERT_TRUE_MSG(test_member_func_1(foo, 10) == 20, "Assertion failed. move constructor method for member function:");


    // compose tests
    auto fa = Function<float(int)>([](int x) {
        return float(x + 5);
    });

    auto fb = Function<int(float)>([](float y) {
        return int(y * y);
    });

    //auto composed = compose(fb, fb, fb); //Compose  type check failed
    auto composed = compose(fb, fa, fb);

    //ASSERT_TRUE_MSG(composed(4) == 441, "Assertion failed. Compose function");  //Compose  type check failed
    ASSERT_TRUE_MSG(composed(float(4)) == 441, "Assertion failed. Compose function");
    return 0;
}
