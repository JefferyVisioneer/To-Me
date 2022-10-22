#include "my_unique_ptr.h"
#include <memory>

template <typename T>
bool my_unique_ptr_collector<T>::is_ptr_handled(T* what_ptr)
{
    try{
        all_ptrs.at(what_ptr);
    }
    catch (...){
        return false;
    }

    return true;
}

template <typename T>
void my_unique_ptr_collector<T>::add_ptr(T* new_ptr)
{
    if (!new_ptr) { return ; }

    all_ptrs[new_ptr] = true;
}

template <typename T>
void my_unique_ptr_collector<T>::delete_ptr(T* delete_ptr)
{
    if (!delete_ptr) { return ; }

    if (is_ptr_handled(delete_ptr)){
        all_ptrs.erase(delete_ptr);
    }
}


template <typename T>
static my_unique_ptr_collector<T>* make_new_collector()
{
    static my_unique_ptr_collector<T> T_collector{};
    return &T_collector;
}


template <typename T, typename Deleter>
T* my_unique_ptr<T, Deleter>::release()
{
    if (secret_ptr){
        collector_ptr->delete_ptr(secret_ptr);
    }

    auto temp_ptr {secret_ptr};
    secret_ptr = nullptr;

    return temp_ptr;
}

template <typename T, typename Deleter>
void my_unique_ptr<T, Deleter>::reset(T* reset_ptr)
{
    collector_ptr->delete_ptr(secret_ptr);
    delete_func(secret_ptr);
    secret_ptr = reset_ptr;
}

template <typename T, typename Deleter>
void my_unique_ptr<T, Deleter>::swap(my_unique_ptr<T> &other)
{
    auto temp_ptr {this->secret_ptr};
    this->secret_ptr = other.secret_ptr;
    other.secret_ptr = temp_ptr;
}

template <typename T, typename Deleter>
T* my_unique_ptr<T[], Deleter>::release()
{
    collector_ptr->delete_ptr(array_obj);

    auto temp_ptr {array_obj};
    array_obj = nullptr;

    return temp_ptr;
}

template <typename T, typename Deleter>
void my_unique_ptr<T[], Deleter>::reset(T* reset_ptr)
{
    collector_ptr->delete_ptr(array_obj);
    delete_func(array_obj);
    array_obj = reset_ptr;
}

template <typename T, typename Deleter>
void my_unique_ptr<T[], Deleter>::swap(my_unique_ptr<T[], Deleter> &other_u_p)
{
    auto temp_ptr {other_u_p.array_obj};
    other_u_p.array_obj = array_obj;
    array_obj = temp_ptr;
}


template <typename T, typename ...Obj>
my_unique_ptr<T> make_my_unique_ptr(Obj&& ... obj)
{
    return my_unique_ptr<T>(new T(std::forward<Obj>(obj)...));
}


