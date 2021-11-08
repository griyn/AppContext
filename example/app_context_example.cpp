#include "app_context.h"
#include <iostream>

class Person {
public:
    virtual void call() {
        std::cout << "im a person" << std::endl;
    }
};

APPCONTEXT_REGISTER(Person, Person);

class Archer : public Person {
public:
    virtual void call() override {
        std::cout << "im a archer" << std::endl;
    }
};

APPCONTEXT_REGISTER_WITH_NAME(Archer, Person, Archer);

void call(Person* p) {
    if (p) p->call();
    else std::cout << "null" << std::endl;
}

int main() {
    auto p = garden::AppContext::instance().get<Person>("Tom");
    call(p.get()); // 用未注册名字获取，失败返回null

    p = garden::AppContext::instance().get<Person>("Person");
    call(p.get()); // 成功

    auto a = garden::AppContext::instance().get<Person>("Archer");
    call(a.get());
    return 0;
};
