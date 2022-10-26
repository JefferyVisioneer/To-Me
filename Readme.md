Structure:
Every 'my_unique_ptr<T, Deleter>' holds pointer to static 'my_unique_ptr_collector<T, Deleter>'. Control block have map inside of it to be able to quickly find,
add and delete stored pointers in it. When I firstly thought of this realization, I was thinking that idea of map inside of control block to hold every ptr, stored by
unique pointer, is wonderful. 
But now I'd rewrite it. But in general it is quite close copy of unique_ptr by C++17 standards (I mean in standard way of using it, without high specified skills like 
multi-thread operations and other smart stuff). 
