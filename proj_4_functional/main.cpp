#include <iostream>
#include "function.h"
#include "compose.h"

int sum(int a, int b) {
    return a+b;
}

int sum2(int a, int b, int c, int d) {
    return a+b+c+d;
}

struct Sum {
    int a_;
    Sum(int a) : a_(a) {};
    int sum(int b) const {
        return a_+b;
    }
};

struct FunctorSum {
    int operator()(int a, int b) const
    {
        return a+b;
    }
};

void FailWithMsg(const std::string& msg, int line) {
    std::cerr << "Test failed!\n";
    std::cerr << "[Line " << line << "] "  << msg << std::endl;
    std::exit(EXIT_FAILURE);
}

#define ASSERT_TRUE(cond) \
if (!(cond)) {FailWithMsg("Assertion failed: " #cond, __LINE__);};

int main() {
    {
        // обычная функция
        function<int(int, int)> f = sum;
        for (int i = 0; i < 1000; i++) {
            ASSERT_TRUE(f(i,5*i) == i+5*i);
        }
        
        //функтор
        f = FunctorSum();
        for (int i = 0; i < 1000; i++) {
            ASSERT_TRUE(f(i,5*i) == i+5*i);
        }
    }
    
    {
        // std::bind
        function<int()> f = std::bind(sum2, 3, 4, 5, 6);
        ASSERT_TRUE(f() == 18);
    }
    
    {
        // лямбда
        function<bool()> f = []() { return true; };
        for (int i = 0; i < 1000; i++) {
             ASSERT_TRUE(f());
        }
        f = []() { return false;};
        for (int i = 0; i < 1000; i++) {
            ASSERT_TRUE(!f());
        }
        
        // swap, оператор bool, перемещение
        function<bool()> g = []() { return true; };
        f.swap(g);
        ASSERT_TRUE(f())
        ASSERT_TRUE(!g());
        
        function<bool()> p;
        ASSERT_TRUE(f)
        ASSERT_TRUE(!p);
        
        p = std::move(f);
        ASSERT_TRUE(p);
        ASSERT_TRUE(p());
        ASSERT_TRUE(!f)
    }
    
    {
        // функция-член и данные-члены
        function<int(const Sum&, int)> f = &Sum::sum;
        function<int(Sum const&)> f_data = &Sum::a_;
        for (int i = 1000; i < 10000; i++) {
            const Sum s(i);
            ASSERT_TRUE(f(s, i) == 2*i);
            ASSERT_TRUE(f(2*i, i) == 3*i);
            ASSERT_TRUE(f_data(s) == i);
        }
    }

    {
        // композиция
        const function<bool(int)> a = [] (int x) -> bool {
            return x > 10;
        };
        
        const function<int(float)> b = [] (float y) -> int {
            return int(y * y);
        };
        
        const function<float(bool)> c = [] (bool z) -> float {
            return z ? 3.1f : 3.34f;
        };
        
        auto d = compose(a, b, c, a, b, c);
        ASSERT_TRUE(d(true));
        ASSERT_TRUE(!d(false))
        
        auto f = compose(b, c);
        ASSERT_TRUE(f(true) == 9);
        ASSERT_TRUE(f(false) == 11);
        
        auto g = compose(a, b);
        ASSERT_TRUE(!g(2.5))
        ASSERT_TRUE(g(3.5));
    }
    
    return 0;
}
