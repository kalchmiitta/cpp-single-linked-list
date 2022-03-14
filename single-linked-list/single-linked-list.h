#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>

template <typename Type>
class SingleLinkedList {
    
    // Узел списка
    struct Node {
        Node () = default;
        
        Node (const Type &val, Node *next)
                : value(val), next_node(next) {
        }
        
        Type value;
        Node *next_node = nullptr;
    };
    
    template<typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка был доступ к приватной области итератора
        friend class SingleLinkedList;
        
        explicit BasicIterator (Node *node)
                : node_(node) {}
    
    public:
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType *;
        // Тип ссылки на итерируемое значение
        using reference = ValueType &;
        
        BasicIterator () = default;
        
        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator (const BasicIterator<Type> &other) noexcept: node_(other.node_) {
        }
        
        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator &operator= (const BasicIterator &rhs) = default;
        
        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator== (const BasicIterator<const Type> &rhs) const noexcept {
            return node_ == rhs.node_;
        }
        
        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!= (const BasicIterator<const Type> &rhs) const noexcept {
            return node_ != rhs.node_;
        }
        
        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator== (const BasicIterator<Type> &rhs) const noexcept {
            return node_ == rhs.node_;
        }
        
        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!= (const BasicIterator<Type> &rhs) const noexcept {
            return node_ != rhs.node_;
        }
        
        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator &operator++ () noexcept {
            
            this->node_ = (*this->node_).next_node;
            return *this;
        }
        
        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++ (int) noexcept {
            BasicIterator tmp(this->node_);
            this->node_ = (*this->node_).next_node;
            return tmp;
        }
        
        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator* () const noexcept {
            return node_->value;
        }
        
        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator-> () const noexcept {
            return &(operator*());
        }
    
    private:
        Node *node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type &;
    using const_reference = const value_type &;
    
    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;
    
    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin () noexcept {
        return Iterator(head_.next_node);
    }
    
    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end () noexcept {
        return Iterator();
    }
    
    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin () const noexcept {
        return cbegin();
    }
    
    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end () const noexcept {
        return cend();
    }
    
    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin () const noexcept {
        return {ConstIterator(head_.next_node)};
    }
    
    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend () const noexcept {
        return ConstIterator();
    }
    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin () noexcept {
        return Iterator(&head_);
    }
    
    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin () const noexcept {
        return ConstIterator(const_cast<Node *>(&head_));
    }
    
    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin () const noexcept {
        return cbefore_begin();
    }


public:
    
    SingleLinkedList () {
        Node head_;
        size_ = 0u;
    }
    
    SingleLinkedList (std::initializer_list<Type> values) {
        
        SingleLinkedList current_list;
        MakeCopy(current_list, values);
        this->swap(current_list);
        
    }
    
    SingleLinkedList (const SingleLinkedList &other) {
        
        SingleLinkedList current_list;
        current_list = other;
        this->swap(current_list);
        
    }
    
    SingleLinkedList &operator= (const SingleLinkedList &rhs) {
        
        if (this != &rhs) {
            SingleLinkedList current_list;
            MakeCopy(current_list, rhs);
            this->swap(current_list);
        }
        
        return *this;
    }
    
    ~SingleLinkedList () {
        Clear();
    }

// Вставляет элемент value в начало списка за время O(1)
    void PushFront (const Type &value) {
        head_.next_node = new Node(value, head_.next_node);
        ++ size_;
    }

// Очищает список за время O(N)
    void Clear () noexcept {
        while (head_.next_node != nullptr) {
            Node *new_head = head_.next_node->next_node;
            delete head_.next_node;
            head_.next_node = new_head;
        }
        size_ = 0;
    }
    
    [[nodiscard]] size_t GetSize () const noexcept {
        return size_;
    }
    
    [[nodiscard]] bool IsEmpty () const noexcept {
        bool result = size_ ? false : true;
        return result;
    }

    
    void PopFront () noexcept {
        if (head_.next_node != nullptr) {
            Node *curr = head_.next_node;
            head_.next_node = curr->next_node;
            delete curr;
            -- size_;
        }
    }
    
    Iterator EraseAfter (ConstIterator pos) noexcept {
        Node *curr = pos.node_->next_node;
        pos.node_->next_node = curr->next_node;
        delete curr;
        -- size_;
        return Iterator(pos.node_->next_node);
    }
    
    Iterator InsertAfter (ConstIterator pos, const Type &value) {
        Node *new_node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = new_node;
        ++ size_;
        return Iterator(pos.node_->next_node);
    }
    
private:
// Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
    
    template<typename Collection>
    void MakeCopy (SingleLinkedList &res_list, const Collection &collection) {
        
        auto curr_iter = collection.begin();
        const auto collection_end = collection.end();
        
        size_t size = 0;
        if (curr_iter != collection_end) {
            
            res_list.head_.next_node = new Node(*curr_iter, nullptr);
            ++ size;
            ++ curr_iter;
            
            Node *new_node = res_list.head_.next_node;
            
            while (curr_iter != collection_end) {
                
                new_node->next_node = new Node(*curr_iter, nullptr);
                ++ size;
                new_node = new_node->next_node;
                ++ curr_iter;
            }
            res_list.size_ = size;
        }
        
    }

public:

    // Обменивает содержимое списков за время O(1)
    void swap (SingleLinkedList &other) noexcept {
        
        Node *tmp = other.head_.next_node;
        other.head_.next_node = this->head_.next_node;
        this->head_.next_node = tmp;
        
        size_t tmp_size = other.size_;
        other.size_ = this->size_;
        this->size_ = tmp_size;
        
    }
    
};


template<typename Type>
void swap (SingleLinkedList<Type> &lhs, SingleLinkedList<Type> &rhs) noexcept {
    lhs.swap(rhs);
}

template<typename Type>
bool operator== (const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<typename Type>
bool operator!= (const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return ! std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<typename Type>
bool operator< (const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<typename Type>
bool operator<= (const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return (lhs < rhs || lhs == rhs);
}

template<typename Type>
bool operator> (const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return ! (lhs <= rhs);
}

template<typename Type>
bool operator>= (const SingleLinkedList<Type> &lhs, const SingleLinkedList<Type> &rhs) {
    return ! (lhs < rhs);
}
