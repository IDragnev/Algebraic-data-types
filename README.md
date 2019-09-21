# Tuple and Variant
## Implementation of fully-featured C++17 variant and tuple with additional [algorithms](https://github.com/IDragnev/Tuple/blob/master/Tuple/TupleAlgorithms.h) for tuple, such as:  
 - insertBack  
 - insertFront 
 - select
 - reverse  
 - take  
 - drop  
 - sortByType   
 - replicate  
 - apply
 - foldl
 - transform
 - forEach  
 - concatenate  
and more.  

Tuples also support the pipe syntax:
```C++
  const auto sum = [](const auto& tuple) { return foldl(tuple, 0, std::plus{}); };
  const auto s = makeTuple(1, 2, 3, 4, 5)
                 | take<3>
                 | sum;
  //s == 6
```
Examples and details can be found in the [tests of tuple](https://github.com/IDragnev/Tuple-and-Variant/blob/master/tests/tuple.cpp) and [tests of variant](https://github.com/IDragnev/Tuple-and-Variant/blob/master/tests/variant.cpp).    
Structured bindings are not supported due to some ambiguity in the get function.

