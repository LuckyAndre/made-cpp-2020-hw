#ifndef compose_h
#define compose_h

// для одного аргумента
template <class F>
auto compose(const F& f) {
    return [f](auto x){return f(x);};
}

// для двух аргументов
template <class F, class G>
auto compose(const F& f, const G& g) {
    return [f,g](auto x){return f(g(x));};
}

// для большего числа аргументов
// окончание рекурсии - вызов варианта для двух аргументов
template <class F, class... Fs>
auto compose(const F& f, const Fs& ...fs) {
    return compose(f, compose(fs...));
}

#endif /* compose_h */
