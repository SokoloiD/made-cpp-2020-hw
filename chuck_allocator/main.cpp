#include "CustomMemAllocator.h"
#include <iostream>
#include <vector>

class TestMemObject {
public:
    TestMemObject(int x) : x_(x) {
        std::cout << "constructor  for :" << x << " at " << this << std::endl;
    }
    TestMemObject(const TestMemObject &other) {
        x_ = other.x_;
        std::cout << "copy for:" << x_ << " at " << this << std::endl;
    }

    ~TestMemObject() {
        std::cout << "~ for :" << x_ << " at " << this << std::endl;
    }
    TestMemObject &operator=(const TestMemObject &other) {
        x_ = other.x_;
        std::cout << " operator = for " << x_ << std::endl;
        return *this;
    }
    int getValue() { return x_; }

private:
    int x_;
};
void test() {
    CustomMemAllocator<TestMemObject> allocator1;
    auto testObject1 = allocator1.allocate(1);
    allocator1.construct(testObject1, 1);
    allocator1.destroy(testObject1);
    allocator1.deallocate(testObject1, 1);

    CustomMemAllocator<TestMemObject> allocator2;
    allocator2 = allocator1;
    auto testObject2 = allocator2.allocate(1);
    allocator2.construct(testObject2, 2);
    allocator2.destroy(testObject2);
    allocator2.deallocate(testObject2, 1);

    std::vector<TestMemObject, CustomMemAllocator<TestMemObject>> vec;

    vec.push_back(TestMemObject(12));
    vec.push_back(TestMemObject(13));
    vec.push_back(TestMemObject(14));
}
int main() {
    test();
    return 0;
}
