#include <string>
#include <map>
#include <memory>

namespace garden {

class AppContext {
public:
    class AppHolder;

    // 单例模式
    static AppContext& instance() {
        static AppContext _instance;
        return _instance;
    }

    // 注册工厂类型
    bool register_app(AppHolder* holder, const std::string& name) {
        auto it = _holders_by_name.emplace(name, holder); // 转移所有权
        return it.second;
    }

    // 用名字获得对应对象
    // 是实际构造对象的时间点，每次调用都会构造一次
    template <typename T>
    std::unique_ptr<T> get(const std::string& name) {
        auto it = _holders_by_name.find(name);
        if (it == _holders_by_name.end()) {
            return nullptr;
        }
        return it->second->get<T>();
    }

public:
    // 持有类型，生成对应类型的对象
    class AppHolder {
    public:
        virtual ~AppHolder() {}

        // 生成工厂对象T
        template <typename T>
        std::unique_ptr<T> get() {
            void* result = get(); 
            // 模板不能继承，这里调用子类的get(), 强制转换成返回类型
            return std::unique_ptr<T>(reinterpret_cast<T*>(result));
        }
        
    protected:
        // 用void*承接子类生成的对象
        virtual void* get() = 0;
    };

private:
    // 为了让一个map中的每个AppHolder可以get不同类型的工厂对象，不能让AppHolder本身使用模板。
    // 所以AppHolder用多态的形式让子类持有模板类型，在get是返回。
    // 而且工厂对象可以有不同的父类。
    std::map<std::string, std::unique_ptr<AppHolder> > _holders_by_name;
};

template<typename U, typename T> // <子类类型, 父类类型>
class CosAppHolder : public AppContext::AppHolder {
protected:
    virtual void* get() override {
        return static_cast<void*>(new U);
    }
};

// 注册helper
template<typename T, typename U>
class RegisterHelper {
public:
    RegisterHelper(const std::string& name, 
            AppContext& context = AppContext::instance()) {
        context.register_app(new CosAppHolder<T, U>(), name);
    }
};

} // garden

// 在main之前注册类型。注册name重复时会编译错误
// 使用(子类类型,父类类型)注册工厂,子类类型即注册名字
#define APPCONTEXT_REGISTER(Ttype, Utype) \
    static garden::RegisterHelper<Ttype, Utype> garden_AppContext_Register_##Ttype(#Ttype);

// 使用(子类类型,父类类型,注册名字)注册工厂
#define APPCONTEXT_REGISTER_WITH_NAME(Ttype, Utype, name) \
    static garden::RegisterHelper<Ttype, Utype> garden_AppContext_Register_##name(#name);
