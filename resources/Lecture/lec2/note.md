# lec2 :C & GDB

```c
指针类型
int *a; // pointer to int

float *b; // pointer to float

int **c; 指向 指向int的指针

char (*d)(int); // pointer to a function (int -> char) 指向函数的指针,该函数接受一个 int 参数并返回 char.
例子:
char func(int x) {
    return 'A' + x;
}
char (*d)(int) = func; // d 指向 func 函数
printf("%c", d(1)); // 输出 'B'


char (**e)(int); // pointer to pointer to function (int -> char)
指向 char (*)(int) 类型的指针,即指向函数指针的指针.
例子:
char func(int x) {
    return 'A' + x;
}
char (*d)(int) = func; // d 指向 func 函数
char (**e)(int) = &d; // e 指向 d 的地址
printf("%c", (*e)(1)); // 输出 'B'


void *f; // pointer to untyped memory
指向无类型内存的指针.

void **g; // pointer to pointer to untyped memory
同理
int x = 10;
void *f = &x; // f 指向 x 的地址
void **g = &f; // g 指向 f 的地址
printf("%d", *(int *)(*g)); // 输出 10

Pointers can be arbitrarily nested:
int ******value; // PLEAS
EXAMPLE:
int x = 10;
int *a = &x;
int **b = &a;
int ***c = &b;
int ****d = &c;
int *****e = &d;
int ******value = &e;
printf("%d", ******value); // 输出 10
```

一个例子

```c
int a = 10;
int *b = &a;  pass by ref引用传递
int c = a; Pass by value
int d = a;
*b = 20;
d = 30;
// what does this print?
printf("a = %d, *b = %d, c = %d, d = %d\n", a, *b, c, d);

解析：
a=10
b是10
c=10
d=10
修改b 也就是修改a和b  a=b=20
d=30 c不变
a=b=20 c=10 d=10
```

指针访问数组

```c
int my_array[5] = {10, 20, 30, 40, 50};
my_array 的类型是 int*
my_array 是一个指向 int 类型的指针,它的值是数组第一个元素的地址.
获取元素的value
int value = my_array[n]
int value = *(my_array + n)
获取地址
int *ptr = &my_array[n];
int *ptr = my_array+n;

Type为int的指针
my_array 的起始地址是 0x2FB0.
my_array + 1 的地址是 0x2FB0 + 4 = 0x2FB4.
my_array + 2 的地址是 0x2FB0 + 8 = 0x2FB8.
my_array + 3 的地址是 0x2FB0 + 12 = 0x2FBC.


一个很抽象的用法
int values[5] = {10, 20, 30, 40, 50};
printf(“%d\n”, 4[values]);
因为可以通过my_array+5来访问my_array[5] 而且加法是可交换的
也就是说5[my_array]和my_array[5]和my_array+5都是同一个东西
```

强制类型转换 不带检查

```c
float x = 3.7;
int x_int =(int) x 3;
```

## !重要的内存分配问题

```c
int *multiples_of(int number, int max) {
int my_local_array[max];
    for (int i = 0; i < max; i++) {
        my_local_array[i] = number * (i + 1);
}
    return &my_local_array[0];

    会出现悬空指针 因为在返回之前 数组已经被销毁了!!!!
}
```

如何解决?

```c
int *multiples_of(int number, int max) {
int *my_local_array = malloc(sizeof(int) * max);
    for (int i = 0; i < max; i++) {
        my_local_array[i] = number * (i + 1);
}
    return &my_local_array[0];

    动态分配内存,确保数组的生命周期不受函数返回的影响
    内存分配在堆上面
    分配的内存不会在函数返回时被释放
    需要手动free释放
}


```

Preprocess 的讲解

```c
#define：定义宏.

#ifdef：检查宏是否已定义.

#ifndef：检查宏是否未定义.

#if：根据条件编译代码.

#else：与 #if、#ifdef 或 #ifndef 配合使用,表示条件不成立时的代码.

#endif：结束条件编译块.在上面的 #ifdef #ifndef #if #else 后面加上

#undef：取消已定义的宏.

EXAMPLE：

#define DEBUG

#ifdef DEBUG
    printf("Debug mode is enabled.\n");
#endif

#undef DEBUG

#ifdef DEBUG
    printf("Debug mode is still enabled.\n");
#else
    printf("Debug mode is disabled.\n");
#endif
```

头文件保护机制

```c
这里有math.h geo.h  main.c

// math.h
#ifndef MATH_H
#define MATH_H

int add(int a, int b);  // 声明一个加法函数
int subtract(int a, int b);  // 声明一个减法函数

// geometry.h
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "math.h"  // 包含 math.h

double circle_area(double radius);  // 声明一个计算圆面积的函数

#endif // GEOMETRY_H


// main.c
#include "math.h"      // 包含 math.h
#include "geometry.h"  // 包含 geometry.h
按照顺序来说先检查math.h是否定义MATH_H 然后在geo.h包含了math.h 也会检查 这就导致了不会定义两遍

int main() {
    int sum = add(10, 20);  // 使用 math.h 中的函数
    double area = circle_area(5.0);  // 使用 geometry.h 中的函数
    return 0;
}
#endif // MATH_H
```

常见内存有关的函数

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // 分配 10 字节的内存
    char *ptr = malloc(10);
    if (ptr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // 使用 memset 初始化内存
    memset(ptr, 'A', 10); // 将 ptr 的前 10 字节设置为 'A'
    printf("After memset: %.*s\n", 10, ptr); // 输出: AAAAAAAAAA

    // 使用 memcpy 复制内存
    char src[] = "Hello";
    memcpy(ptr, src, 5); // 将 "Hello" 复制到 ptr 的前 5 字节
    printf("After memcpy: %.*s\n", 10, ptr); // 输出: Helloooooo

    // 使用 memmove 复制内存(支持重叠)
    memmove(ptr + 2, ptr, 5); // 将 ptr 的前 5 字节复制到 ptr + 2
    printf("After memmove: %.*s\n", 10, ptr); // 输出: HeHellooo

    // 释放内存
    free(ptr);

    return 0;
}
```

位运算

```c
unsigned short a = 0x1313, b = 0x3232;
根据16进制转2进制
a: 0001 0011 0001 0011
b: 0011 0010 0011 0010
然后AND OR XOR NOT 根据二进制运算即可
0x表示16进制

a: 0001 0011 0001 0011
~: 1110 1100 1110 1100
    E    C    E     C

    ~a == 0xECEC
```

结合位运算和逻辑运算

```c
unsigned int my_int;
my_int |= 1 << N;  将整数 my_int 的第 N 位设置为 1  比如 1 << 3  : 1000
// Clear the Nth bit of an integer
my_int &= ~(1 << N);  清除第 N 位  
// Check if any bits in MASK are set
if (my_int & MASK) { /* ... */ } 检查 my_int 中是否有任何在 MASK 中标记的位被设置为 1。
// Check if all bits in MASK are set
if ((my_int & MASK) == MASK) { /* ... */ }  检查 my_int 中是否所有在 MASK 中标记的位都被设置为 1。
// Check if integer is a power of two
if (my_int && !(my_int & (my_int - 1))) { /* ... */ }  检查整数是否是 2 的幂

```

### 小测试

```c
int main() {
    int x[5]; // x is at 0x7fffdfbf7f00
    printf("%p\n", x);       // -> 0x7fffdfbf7f00
    printf("%p\n", x + 1);   // -> 0x7fffdfbf7f04
    printf("%p\n", &x);      // -> 0x7fffdfbf7f00
    printf("%p\n", &x + 1);  // -> 0x7fffdfbf7f14
    return 0;
    1.x第一个元素的地址
    2.x第二个元素地址
    3.数组的地址
    4.&x指向整个数组的指针(int(*)[5] 类型 所以是20 换算为16进制为14
}
```
