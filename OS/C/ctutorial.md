# C tutorial

[Youtube tutorial](https://www.youtube.com/watch?v=KJgsSFOSQv0&t=31s)

- C is a compiled language
- C is a low-level language
- C is a procedural language
- C is a general-purpose language
- C is a structured language

## Basic program

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
  printf("Hello, World!\n");
  return 0;
}
```

- #include <stdio.h> is a preprocessor directive that includes the standard input/output library
- #include <stdlib.h> is a preprocessor directive that includes the standard library
- main() is the entry point of the program
- printf() is a function that prints to the console
- return 0; indicates that the program has completed successfully
- All instructions end with a semicolon (;)

To run the program in console:

```bash
gcc hello.c -o hello
./hello
```

- The program is build (compiled) and then executed

## Variables

### Characters

```c
char c = 'a'; // single character
char str[] = "Hello, World!"; // string
```

### Numbers

```c
int i = 10; // integer
float f = 10.5; // float
double d = 10.5; // double
```

### Booleans

```c
#include <stdbool.h>
bool b = true; // boolean
```

## Constant

- Constants are fixed values that do not change during the execution of a program
- Constants can be of any data type
- Constants are declared using the `const` keyword
- Constants can be declared using the `#define` preprocessor directive

### Example

```c
const int MAX = 100; // constant integer
const float PI = 3.14; // constant float
const char NAME[] = "John"; // constant string
#define MAX 100 // constant integer
#define PI 3.14 // constant float
#define NAME "John" // constant string
```

## Printf

- printf() is used to print output to the console
- printf() can take multiple arguments
- printf() can take format specifiers to format the output

### Format specifiers

- %d - integer
- %f - float
- %c - character
- %s - string
- %p - pointer
- %x - hexadecimal
- %o - octal
- %u - unsigned integer
- %e - scientific notation
- %g - general format
- %ld - long integer
- %lld - long long integer
- %lf - double
- %lu - unsigned long integer
- %llu - unsigned long long integer

### Example:

```c
int favNum = 10;
char myChar = 'a';
printf("My favourite %s is %d\n", "number", favNum);
printf("My favourite %s is %c\n", "character", myChar);
```

## Scanf

- scanf() is used to read input from the user
- scanf() can take multiple arguments
- scanf() can take format specifiers to format the input
- scanf() can take pointers as arguments

### Example

```c
int age;
printf("Enter your age: ");
scanf("%d", &age);
printf("You are %d years old.\n", age);
```

## Arrays

- Arrays are used to store multiple values of the same type in a single variable
- Arrays are declared using the `[]` brackets
- Arrays can be of any data type
- Arrays can be multidimensional
- Arrays are zero-indexed

### Example

```c
int arr[5]; // array of integers
int arr[5] = {1, 2, 3, 4, 5}; // array of integers with initial values
int arr[2][3] = {{1, 2, 3}, {4, 5, 6}}; // 2D array of integers
int arr[2][3][4]; // 3D array of integers
```

## Functions

- Functions are used to group code into reusable blocks
- Functions can take arguments and return values
- Functions can be of any data type
- Functions can be defined using the `return_type function_name(arguments) { code }` syntax
- Functions can be called using the `function_name(arguments)` syntax

### Example

```c
int add(int a, int b) {
  return a + b;
}

int main() {
  int sum = add(10, 20);
  printf("The sum is %d\n", sum);
  return 0;
}
```

## Struct

- Structs are used to group related data into a single unit
- Structs can contain multiple data types
- Structs can be declared using the `struct struct_name { data_type member1; data_type member2; }` syntax
- Structs can be used to create complex data types

### Example

```c
struct Person {
  char name[50];
  int age;
};

int main() {
  struct Person p1;
  strcpy(p1.name, "John"); // copy string to struct member
  p1.age = 30;
  printf("Name: %s\n", p1.name);
  printf("Age: %d\n", p1.age);
  return 0;
}
```

## Memory Adress and Pointers

- Pointers are variables that store the memory address of another variable
- Pointers are declared using the `*` operator
- Pointers can be used to access and modify the value of the variable they point to
- Pointers can be used to dynamically allocate memory using the `malloc()` and `free()` functions
- Pointers can be used to create linked lists, trees, and other data structures
- Pointers can be used to pass arguments to functions by reference
- Pointers can be used to create arrays of pointers

### Example

```c
int a = 10;
int *p = &a; // pointer to integer
printf("Address of a: %p\n", &a);
printf("Value of p: %p\n", p);
printf("Value of a: %d\n", *p); // dereference pointer
*p = 20; // change value of a using pointer
printf("Value of a: %d\n", a);
```
