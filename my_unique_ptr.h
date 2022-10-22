#ifndef MY_UNIQUE_PTR_H_INCLUDED
#define MY_UNIQUE_PTR_H_INCLUDED

#include <map>
#include <iostream>
#include <memory>

template <typename T>
class my_unique_ptr_collector{
    std::map<T*, bool> all_ptrs;

public:
    my_unique_ptr_collector() : all_ptrs{} {}
    my_unique_ptr_collector& operator=(const my_unique_ptr_collector& other_col) = delete;
    my_unique_ptr_collector(const my_unique_ptr_collector& other_col) = delete;
    my_unique_ptr_collector(my_unique_ptr_collector&& other_col) = delete;

    bool is_ptr_handled(T* what_ptr);
    void add_ptr(T* new_ptr);
    void delete_ptr(T* delete_ptr);
};

template <typename T>
static my_unique_ptr_collector<T>* make_new_collector();

template <typename T, typename Deleter = std::default_delete<T>>
class my_unique_ptr{
    T* secret_ptr;
    my_unique_ptr_collector<T>* collector_ptr;
    Deleter delete_func;

public:
    my_unique_ptr() : secret_ptr{nullptr}, collector_ptr{nullptr}, delete_func{std::default_delete<T>{}} {}

    my_unique_ptr(T* new_ptr) : secret_ptr{new_ptr}, collector_ptr{make_new_collector<T>()}, delete_func{std::default_delete<T>{}}
        { if (!collector_ptr->is_ptr_handled(secret_ptr))
              { collector_ptr->add_ptr(secret_ptr); }
        }

    my_unique_ptr(my_unique_ptr&& other_u_ptr) : secret_ptr{std::move(other_u_ptr.secret_ptr)}, collector_ptr{std::move(other_u_ptr.collector_ptr)}, delete_func{std::move(other_u_ptr.delete_func)}
        {
            other_u_ptr.secret_ptr = nullptr;
            other_u_ptr.collector_ptr = nullptr;
        }

    my_unique_ptr(T* new_ptr, Deleter d) : secret_ptr{new_ptr}, collector_ptr{make_new_collector<T>()}, delete_func{d}
        {
            if (!collector_ptr->is_ptr_handled(secret_ptr))
                { collector_ptr->add_ptr(secret_ptr); }
        }

    my_unique_ptr& operator=(const my_unique_ptr& other_ptr) = delete;
    my_unique_ptr(const my_unique_ptr& other_ptr) = delete;


    T* release();
    void reset(T* reset_ptr = nullptr);
    void swap(my_unique_ptr<T> &other);

    T* get() const { return secret_ptr; }
    Deleter& get_deleter() const { return delete_func; }

    operator bool() const { if (secret_ptr) { return true; } return false; }

    T* operator->() const { return secret_ptr; }
    T& operator*() const { return *secret_ptr; }

    ~my_unique_ptr(){
        collector_ptr->delete_ptr(secret_ptr);
        delete_func(secret_ptr);
    }
};

template <typename T, typename Deleter>
class my_unique_ptr<T[], Deleter>{
    Deleter delete_func;
    my_unique_ptr_collector<T>* collector_ptr;
    T* array_obj;

public:
    my_unique_ptr() : delete_func{std::default_delete<T[]>{}}, collector_ptr{make_new_collector<T>()}, array_obj{nullptr} {}
    my_unique_ptr(T* new_ptr) : delete_func{std::default_delete<T[]>{}}, collector_ptr{make_new_collector<T>()}, array_obj{new_ptr}
        {
            if (!collector_ptr->is_ptr_handled(new_ptr))
                { collector_ptr->add_ptr(new_ptr); }
        }

    T* release();
    void reset(T* reset_ptr = nullptr);
    void swap(my_unique_ptr<T[], Deleter> &other_u_p);

    T& operator[](size_t index) const { return array_obj[index]; }

    ~my_unique_ptr() { delete_func(array_obj); }

};

template <typename T, typename ...Obj>
my_unique_ptr<T> make_my_unique_ptr(Obj&& ... obj);

#endif // MY_UNIQUE_PTR_H_INCLUDED
