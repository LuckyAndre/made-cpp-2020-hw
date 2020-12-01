#ifndef function_h
#define function_h

#include <type_traits>

template <class>
class function;

template <class R, class... Args>
class function<R(Args...)> {
private:
    // базовый класс callable объекта
    class callable_base {
    public:
        callable_base() { };
        virtual ~callable_base() = default;
        virtual R invoke(Args...) = 0;
        virtual std::unique_ptr<callable_base> clone() = 0;
    };
    // наследник - свободная функция
    template <class FunctionT>
    class callable: public callable_base {
    private:
        FunctionT func;
    public:
        callable(const FunctionT& f) : func(f) { };
        ~callable() override = default;
        R invoke(Args... args) override {
            return func(args...);
        }
        std::unique_ptr<callable_base> clone() override {
            return std::make_unique<callable<FunctionT>>(func);
        }
    };
    // наследник - функция-член
    template <class FunctionT, class ClassT, class... RestArg>
    class member_callable: public callable_base {
    private:
        typedef FunctionT std::remove_reference_t<ClassT>::* member_type;
        member_type func;
    public:
        member_callable(member_type f) : func(f) { };
        ~member_callable() override = default;
        R invoke(ClassT obj, RestArg... args) override {
            if constexpr (std::is_member_function_pointer<member_type>::value)
                return (obj.*func)(args...);
            else
                return (obj.*func);
        }
        std::unique_ptr<callable_base> clone() override {
            return std::make_unique<member_callable<FunctionT, Args ...>>(func);
        }
    };
    std::unique_ptr<callable_base> callable_ptr;
public:
    // конструктор по умолчанию
    function() : callable_ptr() {}
    
    // конструктор, принимающий свободную функцию/лямбду
    template <class FunctionT>
    function(FunctionT f) {
        callable_ptr = std::make_unique<callable<FunctionT>>(f);
    }
    
    // конструктор, принимающий указатель на функцию-член/данные-член
    template <class FunctionT, class ClassT>
    function(FunctionT ClassT::* f) {
        callable_ptr = std::make_unique<member_callable<FunctionT, Args ...>>(f);
    }
    
    // конструктор копирования
    function(const function& other) {
        callable_ptr = other.callable_ptr->clone();
    }
    
    // конструктор перемещения
    function(function&& other) {
        callable_ptr = std::move(other.callable_ptr);
    }
    
    // деструктор
    ~function() { };
    
    // присваивание
    
    template <class FunctionT>
    function& operator=(FunctionT f) {
        callable_ptr = std::make_unique<callable<FunctionT>>(f);
        return *this;
    }
    
    template <class FunctionT, class ClassT>
    function& operator=(FunctionT ClassT::* f) {
        callable_ptr = std::make_unique<member_callable<FunctionT, Args ...>>(f);
        return *this;
    }
    
    function& operator=(const function& other) {
        callable_ptr = other.callable_ptr;
        return *this;
    }
    
    function& operator=(function&& other) {
        callable_ptr = std::move(other.callable_ptr);
        return *this;
    }
    
    // обмен
    void swap(function& other) {
        callable_ptr.swap(other.callable_ptr);
    }
    
    // явное преобразование к bool
    explicit operator bool() const {
        return callable_ptr != nullptr;
    }
    
    // вызов функции
    R operator()(Args... args) const {
        return callable_ptr->invoke(args...);
    }
};

#endif /* function_h */
