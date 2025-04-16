uint64_t* fib;

void calculate_fibonacci(uint64_t n) 
{
    uint64_t i;
    fib = malloc(n * sizeof(uint64_t));  // Allocate memory for n elements

    if (n > 0) *(fib + 0) = 0;  // fib[0] = 0
    if (n > 1) *(fib + 1) = 1;  // fib[1] = 1

    i = 2;  
    while (i < n) {
        *(fib + i) = *(fib + i - 1) + *(fib + i - 2);  // fib[i] = fib[i-1] + fib[i-2]
        i = i + 1; 
    }
}


int main() {
    uint64_t n;
    uint64_t t;
    
    n = 23;

    calculate_fibonacci(n);
    t = tick();
    exit(t);
}
