# placeholder #

placeholder is an C++17(or C++1z) library to create functions by short syntax.

## usage ##
Include `placeholder/placeholder.hpp`

```c++
#include <placeholder/placeholder.hpp>
```

## example ##
```c++
using placeholder::_;

auto add5 = _ + 5;
add5(10); // => 15

auto eq3 = _ == 3;
eq3(3); // => true

auto even = _ % 2 == 0;
even(8); // => true
```