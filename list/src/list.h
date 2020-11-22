#pragma once
#include <iterator>
#include <memory> // для allocator_traits

namespace task {

template<class T, class Alloc = std::allocator<T>>
class list {

private:
    // Node будет использоваться в iterator, поэтому объявляем private до public
    class Node { // элемент списка
    public:
        T data; // данные
        Node *next; // указатель на следующий элемент
        Node *prev; //указатель на предыдущий элемент
        // конструкторы
        Node(Node* n, Node* p) : next(n), prev(p) {};
        Node(const T& d, Node* n, Node* p) : data(d), next(n), prev(p) {}; // данные копируются
        Node(T&& d, Node* n, Node* p) : data(std::move(d)), next(n), prev(p) {}; // данные перемещаются
    };
    // аллокатор для типа Node
    using allocator_type = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;
    allocator_type allocator;
    Node *first; // первый элемент списка
    Node *last; // последний элемент списка
    size_t size_; // размер списка
    
public:
    class iterator {
    public:
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator();
        iterator(const iterator&);
        iterator& operator=(const iterator&);

        iterator& operator++();
        iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        iterator& operator--();
        iterator operator--(int);

        bool operator==(iterator other) const;
        bool operator!=(iterator other) const;

        friend class list; // делаем класс list дружественным, чтобы использовать ptr из list
    private:
        Node *ptr; // указатель на элемент списка
    };

    class const_iterator {
    public:
        // все то же самое, что и для обычного, но указатель константа
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::bidirectional_iterator_tag;
        
        const_iterator();
        const_iterator(const const_iterator&);
        const_iterator(const iterator&);
        const_iterator& operator=(const const_iterator&);
        
        const_iterator& operator++();
        const_iterator operator++(int);
        reference operator*() const;
        pointer operator->() const;
        const_iterator& operator--();
        const_iterator operator--(int);
        
        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;
        friend class list;
    private:
        const Node *ptr;
    };

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


    list();
    explicit list(const Alloc& alloc);
    list(size_t count, const T& value, const Alloc& alloc = Alloc());
    explicit list(size_t count, const Alloc& alloc = Alloc());

    ~list();

    list(const list& other);
    list(list&& other);
    list& operator=(const list& other);
    list& operator=(list&& other);

    Alloc get_allocator() const;


    T& front();
    const T& front() const;

    T& back();
    const T& back() const;


    iterator begin();
    iterator end();

    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    reverse_iterator rend();

    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;


    bool empty() const;
    size_t size() const;
    size_t max_size() const;
    void clear();

    iterator insert(const_iterator pos, const T& value);
    iterator insert(const_iterator pos, T&& value);
    iterator insert(const_iterator pos, size_t count, const T& value);

    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);


    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    void push_front(const T& value);
    void push_front(T&& value);
    void pop_front();

    template <class... Args>
    iterator emplace(const_iterator pos, Args&&... args);

    template <class... Args>
    void emplace_back(Args&&... args);

    template <class... Args>
    void emplace_front(Args&&... args);

    void resize(size_t count);
    void swap(list& other);


    void merge(list& other);
    void splice(const_iterator pos, list& other);
    void remove(const T& value);
    void reverse();
    void unique();
    void sort();
};
    
    // iterator
    
    template <class T, class Alloc>
    list<T, Alloc>::iterator::iterator() {
        ptr = nullptr;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::iterator::iterator(const iterator& other) {
        ptr = other.ptr;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator=(const task::list<T, Alloc>::iterator& other) {
        ptr = other.ptr;
        return *this;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator++() {
        ptr = ptr->next;
        return *this;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator++(int) {
        iterator it = *this;
        ptr = ptr->next;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator::reference list<T, Alloc>::iterator::operator*() const {
        return ptr->data;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator::pointer list<T, Alloc>::iterator::operator->() const {
        return &(ptr->data);
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator& list<T, Alloc>::iterator::operator--() {
        ptr = ptr->prev;
        return *this;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::iterator::operator--(int) {
        iterator it = *this;
        ptr = ptr->prev;
        return it;
    }
    
    template <class T, class Alloc>
    bool list<T, Alloc>::iterator::operator==(iterator other) const {
        return ptr == other.ptr;
    }
    
    template <class T, class Alloc>
    bool list<T, Alloc>::iterator::operator!=(iterator other) const {
        return ptr != other.ptr;
    }
    
    // const_iterator
    
    template <class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator() {
        ptr = nullptr;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator(const const_iterator& other) {
        ptr = other.ptr;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::const_iterator::const_iterator(const iterator& other) {
        ptr = other.ptr;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator=(const task::list<T, Alloc>::const_iterator& other) {
        ptr = other.ptr;
        return *this;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator++() {
        ptr = ptr->next;
        return *this;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator++(int) {
        const_iterator it = *this;
        ptr = ptr->next;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator::reference list<T, Alloc>::const_iterator::operator*() const {
        return ptr->data;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator::pointer list<T, Alloc>::const_iterator::operator->() const {
        return &(ptr->data);
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator& list<T, Alloc>::const_iterator::operator--() {
        ptr = ptr->prev;
        return *this;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::const_iterator::operator--(int) {
        iterator it = *this;
        ptr = ptr->prev;
        return it;
    }
    
    template <class T, class Alloc>
    bool list<T, Alloc>::const_iterator::operator==(const_iterator other) const {
        return ptr == other.ptr;
    }
    
    template <class T, class Alloc>
    bool list<T, Alloc>::const_iterator::operator!=(const_iterator other) const {
        return ptr != other.ptr;
    }
    
    // list
    
    template <class T, class Alloc>
    list<T, Alloc>::list() : size_(0)
    {
        // выделяем память для первого и последнего элементов
        // конструктор T для этих элементов не вызываем, данные они хранить не будут
        first = allocator.allocate(1);
        last = allocator.allocate(1);
        first->prev = nullptr;
        first->next = last;
        last->prev = first;
        last->next = nullptr;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::list(const Alloc& alloc) : list<T, Alloc>()
    {
        allocator = alloc;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::list(size_t count, const T& value, const Alloc& alloc) : list<T, Alloc>(alloc)
    {
        // заполняем список count значениями value
        while (size_ < count) {
            push_back(value);
        }
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::list(size_t count, const Alloc& alloc) : list<T, Alloc>(alloc)
    {
        // заполняем список count значениями по умолчанию
        resize(count);
    }
    
    template <class T, class Alloc>
    list<T, Alloc>::~list()
    {
        // очищаем список и освобождаем память, занятую первым и последним элементом
        clear();
        allocator.deallocate(first, 1);
        allocator.deallocate(last, 1);
    }
    
    // конструктор копирования
    template <class T, class Alloc>
    list<T, Alloc>::list(const list& other) : list<T, Alloc>()
    {
        for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
            push_back(*it);
        }
    }
    
    // конструктор перемещения
    template <class T, class Alloc>
    list<T, Alloc>::list(list&& other) : list<T, Alloc>()
    {
        // перемещаем данные из второго списка в первый
        first->next = other.first->next;
        first->next->prev = first;
        last->prev = other.last->prev;
        last->prev->next = last;
        size_ = other.size_;
        other.first->next = other.last;
        other.last->prev = other.first;
        other.size_ = 0;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator=(const list& other)
    {
        clear(); // предварительно очищаем список
        for (const_iterator it = other.cbegin(); it != other.cend(); it++) {
            push_back(*it);
        }
        return *this;
    }
    
    template <class T, class Alloc>
    list<T, Alloc>& list<T, Alloc>::operator=(list&& other)
    {
        clear(); // предварительно очищаем список
        first->next = other.first->next;
        first->next->prev = first;
        last->prev = other.last->prev;
        last->prev->next = last;
        size_ = other.size_;
        other.first->next = other.last;
        other.last->prev = other.first;
        other.size_ = 0;
        return *this;
    }
    
    template <class T, class Alloc>
    Alloc list<T, Alloc>::get_allocator() const
    {
        return allocator;
    }
    
    // доступ к элементам
    
    template <class T, class Alloc>
    T& list<T, Alloc>::front()
    {
        return first->next->data;
    }
    
    template <class T, class Alloc>
    const T& list<T, Alloc>::front() const
    {
        return first->next->data;
    }
    
    template <class T, class Alloc>
    T& list<T, Alloc>::back()
    {
        return last->prev->data;
    }
    
    template <class T, class Alloc>
    const T& list<T, Alloc>::back() const
    {
        return last->prev->data;
    }
    
    // получение итераторов
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::begin()
    {
        iterator it;
        it.ptr = first->next;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::end()
    {
        iterator it;
        it.ptr = last;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cbegin() const
    {
        const_iterator it;
        it.ptr = first->next;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_iterator list<T, Alloc>::cend() const
    {
        const_iterator it;
        it.ptr = last;
        return it;
    }
    
    template <class T, class Alloc>
    typename task::list<T, Alloc>::reverse_iterator list<T, Alloc>::rbegin()
    {
        reverse_iterator it;
        it.ptr = last->prev;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::reverse_iterator list<T, Alloc>::rend()
    {
        reverse_iterator it;
        it.ptr = first;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crbegin() const
    {
        const_reverse_iterator it(cend());
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::const_reverse_iterator list<T, Alloc>::crend() const
    {
        const_reverse_iterator it(cbegin());
        return it;
    }
    
    // размер контейнера
    
    template <class T, class Alloc>
    bool list<T, Alloc>::empty() const
    {
        return size_ == 0;
    }
    template <class T, class Alloc>
    size_t list<T, Alloc>::size() const
    {
        return size_;
    }
    template <class T, class Alloc>
    size_t list<T, Alloc>::max_size() const
    {
        return allocator.max_size();
    }
    
    // внесение изменений
    
    //очистка списка
    template <class T, class Alloc>
    void list<T, Alloc>::clear()
    {
        // удаляем все элементы
        while (!empty()) {
            pop_front();
        }
    }
    
    // добавление элементов
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, const T& value)
    {
        Node *prev = pos.ptr->prev; // элемент, после которого вставляем новый
        Node *node = allocator.allocate(1); // новый элемент
        allocator.construct(node, value, prev->next, prev);
        prev->next->prev = node;
        prev->next = node;
        
        // создаем итератор на вставленный элемент
        iterator it;
        it.ptr = node;
        
        ++size_;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, T&& value)
    {
        Node *prev = pos.ptr->prev; // элемент, после которого вставляем новый
        Node *node = allocator.allocate(1); // новый элемент
        allocator.construct(node, std::move(value), prev->next, prev);
        prev->next->prev = node;
        prev->next = node;
        
        // создаем итератор на вставленный элемент
        iterator it;
        it.ptr = node;
        
        ++size_;
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(const_iterator pos, size_t count, const T& value)
    {
        // добавляем нужное количество новых элементов
        iterator it;
        for (int i = 0; i < count; i++) {
            it = insert(pos, value);
        }
        return it;
    }
    
    // удаление элементов
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator pos)
    {
        Node *to_del = const_cast<Node*>(pos.ptr); // указатель на удаляемый элемент
        iterator it; // итератор на элемент, следующий за удаляемым
        it.ptr = to_del->next;
        
        // исключаем элемент из списка и уничтожаем
        it.ptr->prev = to_del->prev;
        to_del->prev->next = to_del->next;
        --size_;
        allocator.destroy(to_del);
        allocator.deallocate(to_del, 1);
        
        return it;
    }
    
    template <class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const_iterator first, const_iterator last)
    {
        // удаляем элементы от first до last
        const_iterator c_it = first;
        while (c_it != last) {
            c_it = erase(c_it);
        }
        iterator it;
        it.ptr = const_cast<Node*>(c_it.ptr);
        return it;
    }
    
    // добавление в конец списка
    
    template <class T, class Alloc>
    void list<T, Alloc>::push_back(const T& value)
    {
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, last, last->prev);
        last->prev = last->prev->next = node;
        ++size_;
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::push_back(T&& value)
    {
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), last, last->prev);
        last->prev = last->prev->next = node;
        ++size_;
    }
    
    // удаление из конца списка
    
    template <class T, class Alloc>
    void list<T, Alloc>::pop_back()
    {
        // только если список не пустой, исключаем и уничтожаем последний элемент
        if (!empty()) {
            Node *node = last->prev;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            allocator.destroy(node);
            allocator.deallocate(node, 1);
            --size_;
        }
    }
    
    // добавление в начало списка
    
    template <class T, class Alloc>
    void list<T, Alloc>::push_front(const T& value)
    {
        Node *node = allocator.allocate(1);
        allocator.construct(node, value, first->next, first);
        first->next = first->next->prev = node;
        ++size_;
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::push_front(T&& value)
    {
        Node *node = allocator.allocate(1);
        allocator.construct(node, std::move(value), first->next, first);
        first->next = first->next->prev = node;
        ++size_;
    }
    
    template <class T, class Alloc>
    void list<T, Alloc>::pop_front()
    {
        // только если список не пустой, исключаем и уничтожаем первый элемент
        if (!empty()) {
            Node *node = first->next;
            node->prev->next = node->next;
            node->next->prev = node->prev;
            allocator.destroy(node);
            allocator.deallocate(node, 1);
            --size_;
        }
    }
    
    // вставка элементов с вызовом конструктора на месте
    
    template <class T, class Alloc>
    template <class... Args>
    typename list<T, Alloc>::iterator list<T, Alloc>::emplace(typename list<T, Alloc>::const_iterator pos, Args&&... args)
    {
        Node *prev = pos.ptr->prev; // элемент, после которого вставляем новый
        
        // создаем новый элемент
        Alloc data_allocator; // для вызова конструктора T
        Node *node = allocator.allocate(1); // память для элемента
        T *data = data_allocator.allocate(1); // память для данных
        data_allocator.construct(data, std::forward<Args>(args)...); // вызов конструктора
        allocator.construct(node, std::move(*data), prev->next, prev); // вызов конструктора элемента
        data_allocator.destroy(data);
        data_allocator.deallocate(data, 1);
        
        // ставим элемент на нужную позицию
        prev->next->prev = node;
        prev->next = node;
        
        // создаем итератор на вставленный элемент
        iterator it;
        it.ptr = pos.ptr->prev;
        
        ++size_;
        return it;
    }
    
    template <class T, class Alloc>
    template <class... Args>
    void list<T, Alloc>::emplace_back(Args&&... args)
    {
        emplace(end(), std::forward<Args>(args)...);
    }
    
    template <class T, class Alloc>
    template <class... Args>
    void list<T, Alloc>::emplace_front(Args&&... args)
    {
        emplace(begin(), std::forward<Args>(args)...);
    }
    
    // изменение размера списка
    template <class T, class Alloc>
    void list<T, Alloc>::resize(size_t count)
    {
        // добавление элементов, если новый размер больше
        while (size_ < count) {
            Node *node = allocator.allocate(1);
            allocator.construct(node, last, last->prev);
            last->prev = last->prev->next = node;
            ++size_;
        }
        
        //удаление элементов, если новый размер меньше
        while (size_ > count) {
            pop_back();
        }
    }
    
    // обмен списков местами
    template <class T, class Alloc>
    void list<T, Alloc>::swap(list& other)
    {
        //обмениваем первые и последние элементы и размеры
        Node *node = first;
        first = other.first;
        other.first = node;
        node = other.last;
        other.last = last;
        last = node;
        size_t s = size_;
        size_ = other.size_;
        other.size_ = s;
    }
    
    // добавление списка к текущему с сохранением упорядоченности
    template <class T, class Alloc>
    void list<T, Alloc>::merge(list& other)
    {
        // только если второй список не пуст
        if (!other.empty()) {
            Node *node = other.first->next; // добавляемый элемент из второго списка
            Node *current = first->next; // текущий элемент текущего списка
            while (node != other.last) { // для всех элементов второго списка
                while (current != last && node->data > current->data) { // находим позицию
                    current = current->next;
                }
                // вставляем элемент (указатели без копирования данных)
                Node *next = node->next;
                node->prev = current->prev;
                node->next = current;
                current->prev->next = node;
                current->prev = node;
                node = next;
            }
            // убираем перемещенные элементы из второго списка
            other.first->next = other.last;
            other.last->prev = other.first;
            size_ += other.size_;
            other.size_ = 0;
        }
    }
    
    // перемещение другого списка в заданную позицию
    template <class T, class Alloc>
    void list<T, Alloc>::splice(const_iterator pos, list& other)
    {
        // если второй список не пуст, вставляем его между pos->prev и pos
        if (!other.empty()) {
            Node *node = const_cast<Node*>(pos.ptr);
            node->prev->next = other.first->next;
            other.first->next->prev = node->prev;
            node->prev = other.last->prev;
            other.last->prev->next = node;
            other.first->next = other.last;
            other.last->prev = other.first;
            size_ += other.size_;
            other.size_ = 0;
        }
    }
    
    // удаление элемента по значению
    template <class T, class Alloc>
    void task::list<T, Alloc>::remove(const T& value)
    {
        // не удаляем элементы "на ходу", т.к. value может быть ссылкой на элемент списка
        list<iterator> to_del; // список итераторов на удаляемые элементы
        iterator it = begin();
        
        // проходим по списку и добавляем в список to_del итераторы на элементы для удаления
        while (it != end()) {
            if (*it == value) {
                to_del.push_back(it);
            }
            ++it;
        }
        
        // удаляем элементы
        for (auto it = to_del.begin(); it != to_del.end(); ++it) {
            erase(*it);
        }
    }
    
    // обращение порядка элементов
    
    template <class T, class Alloc>
    void task::list<T, Alloc>::reverse()
    {
        // проходим по списку и делаем предыдущие элементы следующими и наоборот
        Node *node = first->next;
        while (node != last) {
            Node *next = node->next;
            node->next = node->prev;
            node->prev = next;
            node = next;
        }
        
        // меняем первый элемент с последним
        first->prev = first->next;
        first->next = nullptr;
        last->next = last->prev;
        last->prev = nullptr;
        node = first;
        first = last;
        last = node;
    }
    
    // удаление идущих подряд уникальных элементов
    template <class T, class Alloc>
    void task::list<T, Alloc>::unique()
    {
        iterator it = begin();
        while (it != --end()) {
            if (*it == *(++it)) {
                it = erase(it);
                --it;
            }
        }
    }
    
    // сортировка (слиянием)
    template <class T, class Alloc>
    void task::list<T, Alloc>::sort()
    {
        // рекурсивный вызов при размере списка больше 1
        
        if (size_ > 1) {
            
            // разделяем список на два
            list left, right;
            Node *l_node = left.first, *r_node = right.first;
            Node *node = first->next;
            int count = 0;
            while (count < size_) {
                if (count < size_/2) {
                    l_node->next = node;
                    node->prev = l_node;
                    l_node = l_node->next;
                    left.size_++;
                }
                else {
                    r_node->next = node;
                    node->prev = r_node;
                    r_node = r_node->next;
                    right.size_++;
                }
                node = node->next;
                count++;
            }
            
            
            left.last->prev = l_node;
            l_node->next = left.last;
            right.last->prev = r_node;
            r_node->next = right.last;
            
            // удаляем левый и правый списки из текущего
            first->next = last;
            last->prev = first;
            size_ = 0;
            
            // сортируем оба списка
            left.sort();
            right.sort();
            
            left.merge(right); // слияние
            (*this) = left; // левый список становится основным
        }
    }

}  // namespace task
