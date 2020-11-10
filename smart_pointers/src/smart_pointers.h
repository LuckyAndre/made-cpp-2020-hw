#ifndef smart_pointers_h
#define smart_pointers_h

namespace task {
    
    template<class T>
    class UniquePtr {
    public:
        using pointer = T*;
        using element_type = T;
        
        // конструкторы
        UniquePtr(pointer p = nullptr); //из обычного указателя
        UniquePtr(UniquePtr&& u); //перемещения
        
        // перемещающий оператор присваивания
        UniquePtr& operator=(UniquePtr&& u);
        
        // декструктор
        ~UniquePtr();

        // запрещаем копирование
        UniquePtr(const UniquePtr&) = delete;
        UniquePtr& operator=(const UniquePtr&) = delete;
        
        element_type operator*() const;
        pointer operator->() const;
        pointer get() const;
        
        pointer release();
        void reset(pointer p = pointer());
        void swap(UniquePtr& other);
    private:
        pointer ptr;
    };
    
    // вспомогательный класс счетчик для shared и weak ptr
    class Counter {
    private:
        long count; // счетчик SharedPtr
        long weak_count; // счетчик WeakPtr (+1 если count != 0)
    public:
        void add() { // увеличение shared
            if (count++ == 0) {
                weak_count++;
            }
        }
        void addWeak() { // увелиение weak
            weak_count++;
        }
        long release() { // уменьшение shared
            if (--count == 0) {
                weak_count--;
            }
            return count;
        }
        long releaseWeak() { // уменьшение weak
            return --weak_count;
        }
        long getCount() {
            return count;
        }
        long getWeakCount() {
            return weak_count;
        }
    };
    
    template<class T>
    class WeakPtr;
    
    template<class T>
    class SharedPtr {
    public:
        using pointer = T*;
        using element_type = T;
        
        // конструкторы
        SharedPtr(pointer p = nullptr); // из обычного указателя
        SharedPtr(const SharedPtr& u); // копирования
        SharedPtr(SharedPtr&& u); // перемещения
        SharedPtr(const WeakPtr<T>& w); // из WeakPtr
        
        SharedPtr& operator=(const SharedPtr& u); // копирующий оператор присваивания
        SharedPtr& operator=(SharedPtr&& u); // перемещающий оператор присваивания
        
        // деструктор
        ~SharedPtr();
        
        pointer operator->() const;
        T& operator*() const;
        pointer get() const;
        
        long use_count() const;
        
        void reset(pointer p = pointer());
        void swap(SharedPtr& other);
        
        friend WeakPtr<T>;
    private:
        pointer ptr;
        Counter *counter;
    };
  
    template<class T>
    class WeakPtr {
    public:
        using pointer = T*;
        using element_type = T;
        
        // конструкторы
        WeakPtr(); // по умолчанию
        WeakPtr(const SharedPtr<T>& u); // из SharedPtr
        WeakPtr(const WeakPtr& u); // копирования
        WeakPtr(WeakPtr&& u); // перемещения
        
        WeakPtr& operator=(const WeakPtr& u); // копирующий оператор присваивания
        WeakPtr& operator=(const SharedPtr<T>& u); // оператор присваивания SharedPtr
        WeakPtr& operator=(WeakPtr&& u); // перемещающий оператор присваивания
        
        // деструктор
        ~WeakPtr();
        
        bool expired() const;
        long use_count() const;
        
        SharedPtr<T> lock(); // получение SharedPtr
        
        void reset();
        void swap(WeakPtr& other);
        
        friend SharedPtr<T>;
    private:
        pointer ptr;
        Counter *counter;
    };
    
    // UniquePtr
    
    // конструкторы
    template<class T>
    UniquePtr<T>::UniquePtr(pointer p) : ptr(p) {
    }
    
    template<class T>
    UniquePtr<T>::UniquePtr(UniquePtr<T>&& u) : ptr(u.ptr) {
        u.ptr = nullptr;
    }
    
    // оператор присваивания
    template<class T>
    UniquePtr<T>& UniquePtr<T>::operator=(UniquePtr<T>&& u) {
        delete ptr;
        ptr = u.ptr;
        u.ptr = nullptr;
        return *this;
    }
    
    // деструктор
    template<class T>
    UniquePtr<T>::~UniquePtr() {
        delete ptr;
    }
    
    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::operator->() const {
        return ptr;
    }
    
    template<class T>
    typename UniquePtr<T>::element_type UniquePtr<T>::operator*() const {
        return *ptr;
    }
    
    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::get() const {
        return ptr;
    }
    
    template<class T>
    typename UniquePtr<T>::pointer UniquePtr<T>::release() {
        pointer p = ptr;
        ptr = nullptr;
        return p;
    }
    
    template<class T>
    void UniquePtr<T>::reset(pointer p) {
        delete ptr;
        ptr = p;
    }
    
    template<class T>
    void UniquePtr<T>::swap(UniquePtr& other) {
        pointer p = other.ptr;
        other.ptr = ptr;
        ptr = p;
    }
    
    // SharedPtr
    
    // конструкторы
    template<class T>
    SharedPtr<T>::SharedPtr(pointer p) : ptr(p) {
        // создаем и увеличиваем счетчик
        if (p != nullptr) {
            counter = new Counter();
            counter->add();
        }
        else {
            counter = nullptr;
        }
    }
    
    template<class T>
    SharedPtr<T>::SharedPtr(SharedPtr<T>&& sp) : ptr(sp.ptr), counter(sp.counter) {
        sp.ptr = nullptr;
        sp.counter = nullptr;
    }
    
    template<class T>
    SharedPtr<T>::SharedPtr(const SharedPtr<T>& sp) : ptr(sp.ptr), counter(sp.counter) {
        // увеличиваем счетчик при создании нового SharedPtr
        if (counter != nullptr)
            counter->add();
    }
    
    template<class T>
    SharedPtr<T>::SharedPtr(const WeakPtr<T>& wp) {
        if (wp.use_count() == 0) {
            counter = nullptr;
            ptr = nullptr;
        }
        else {
            counter = wp.counter;
            counter->add();
            ptr = wp.ptr;
        }
    }
    
    // операторы присваивания
    
    template<class T>
    SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& sp) {
        reset(nullptr); // удаляем текущий указатель, если он есть
        ptr = sp.ptr;
        counter = sp.counter;
        sp.ptr = nullptr;
        sp.counter = nullptr;
        return *this;
    }
    
    template<class T>
    SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& sp) {
        reset(nullptr); // удаляем текущий указатель, если он есть
        counter = sp.counter;
        ptr = sp.ptr;
        // увеличиваем счетчик при создании копии
        if (counter != nullptr)
            counter->add();
        return *this;
    }
    
    // декструктор
    template<class T>
    SharedPtr<T>::~SharedPtr() {
        reset(nullptr); // уменьшаем счетчики и освобождаем память, если нужно
    }
    
    template<class T>
    typename SharedPtr<T>::pointer SharedPtr<T>::operator->() const {
        return ptr;
    }
    
    template<class T>
    T& SharedPtr<T>::operator*() const {
        return *ptr;
    }
    
    template<class T>
    typename SharedPtr<T>::pointer SharedPtr<T>::get() const {
        return ptr;
    }
    
    template<class T>
    long SharedPtr<T>::use_count() const {
        // возвращаем 0, если указатель нулевой
        if (counter)
            return counter->getCount();
        else
            return 0;
    }
    
    template<class T>
    void SharedPtr<T>::reset(pointer p) {
        // если счетчик = 0, освобождаем память
        if (counter && counter->release() == 0) {
            delete ptr;
            // сам счетчик удаляем, если нет и WeakPtr
            if (counter->getWeakCount() == 0) {
                delete counter;
            }
        }
        ptr = p;
        if (p != nullptr) {
            counter = new Counter();
            counter->add();
        }
        else {
            counter = nullptr;
        }
    }
    
    template<class T>
    void SharedPtr<T>::swap(SharedPtr& other) {
        pointer t = other.ptr;
        Counter *c = other.counter;
        other.ptr = ptr;
        other.counter = counter;
        ptr = t;
        counter = c;
    }
    
    // WeakPtr
    
    // конструкторы
    
    template<class T>
    WeakPtr<T>::WeakPtr() : ptr(nullptr), counter(nullptr) {
        
    }
    
    template<class T>
    WeakPtr<T>::WeakPtr(const SharedPtr<T>& wp) : ptr(wp.ptr), counter(wp.counter) {
        // при создании WeakPtr увеличиваем соответствующий счетчик
        if (counter)
            counter->addWeak();
    }
    
    template<class T>
    WeakPtr<T>::WeakPtr(WeakPtr<T>&& wp) : ptr(wp.ptr), counter(wp.counter) {
        wp.ptr = nullptr;
        wp.counter = nullptr;
    }
    
    template<class T>
    WeakPtr<T>::WeakPtr(const WeakPtr<T>& wp) : ptr(wp.ptr), counter(wp.counter) {
        // при создании копии увеличиваем соответствующий счетчик
        if (counter)
            counter->addWeak();
    }
    
    // операторы присваивания
    
    template<class T>
    WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& wp) {
        ptr = wp.ptr;
        counter = wp.counter;
        wp.ptr = nullptr;
        wp.counter = nullptr;
        return *this;
    }
    
    template<class T>
    WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& sp) {
        counter = sp.counter;
        ptr = sp.ptr;
        if (counter)
            counter->addWeak();
        return *this;
    }
    template<class T>
    WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& u) {
        // при необходимости удаляем счетчик текущего объекта
        if (counter && counter->releaseWeak() == 0) {
            delete counter;
        }
        counter = u.counter;
        ptr = u.ptr;
        // при создании WeakPtr увеличиваем соответствующий счетчик
        if (counter)
            counter->addWeak();
        return *this;
    }
    
    // деструтор
    template<class T>
    WeakPtr<T>::~WeakPtr() {
        if (counter && counter->releaseWeak() == 0) {
            delete counter;
        }
    }
    
    template<class T>
    bool WeakPtr<T>::expired() const {
        return use_count() == 0;
    }
    
    template<class T>
    long WeakPtr<T>::use_count() const {
        if (counter)
            return counter->getCount();
        else
            return 0;
    }
    
    template<class T>
    SharedPtr<T> WeakPtr<T>::lock() {
        // создаем и возвращаем SharedPtr
        SharedPtr<T> shared(*this);
        return shared;
    }
    
    template<class T>
    void WeakPtr<T>::reset() {
        // при необходимости удаляем счетчик текущего объекта
        if (counter && counter->releaseWeak() == 0) {
            delete counter;
        }
        ptr = nullptr;
        counter = nullptr;
    }
    
    template<class T>
    void WeakPtr<T>::swap(WeakPtr& other) {
        pointer t = other.ptr;
        Counter *c = other.counter;
        other.ptr = ptr;
        other.counter = counter;
        ptr = t;
        counter = c;
    }
    
}


#endif /* smart_pointers_h */
