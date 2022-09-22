# Reflect Factory
Create instance from name.
反射工厂，通过字符串名字创建对应对象。

## Register
注册
```c++
class Person {
public:
    virtual void call() {
        std::cout << "im a person" << std::endl;
    }
};
APPCONTEXT_REGISTER(Person, Person); // Register in factory. -> (Child, Parent)

class Archer : public Person {
public:
    virtual void call() override {
        std::cout << "im a archer" << std::endl;
    }
};
APPCONTEXT_REGISTER_WITH_NAME(Archer, Person, Archer); // Register in factory with name. -> (Child, Parent, AnyStringName)
```

## Get
获得
```c++
auto p = garden::AppContext::instance().get<Person>("Person");
p->call();

auto a = garden::AppContext::instance().get<Person>("Archer");
a->call();
```
