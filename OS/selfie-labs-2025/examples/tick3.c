int search(uint64_t* arr, uint64_t start, uint64_t end, uint64_t target) 
{
    uint64_t j;
    j = start;
    while (j <= end) {
        if (*(arr + j) == target) {
            return j;
        }
        j = j + 1;
    }
    return -1;
}

int main() 
{
    uint64_t i;
    uint64_t n;
    uint64_t target;
    uint64_t* arr;
    uint64_t t;
    n = 10;
    target = 17;
    arr = malloc(n * sizeof(uint64_t));

    i = 0;
    while (i < n) {
        *(arr + i) = 1 + i * 2;
        i = i + 1;
    }

    search(arr, 0, n, target);
    t = tick();
    exit(t);
}