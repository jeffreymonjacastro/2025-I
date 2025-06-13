// global variable for pointing to the "Hello World!    " string
uint64_t* foo;
uint64_t id;

void print_number(uint64_t number) {
    uint64_t digit;
    uint64_t divisor;
    uint64_t *d;

    d = malloc(sizeof(uint64_t)); 
    divisor = 1;

    while (number / divisor >= 10) {
        divisor = divisor * 10;
    }

    while (divisor > 0) {
        digit = number / divisor; // most significant digit
        number = number % divisor; // less significant digits
        divisor = divisor / 10;

        *d = 48 + digit; // 48 = ASCII('0')
        write(1, d, 1);   // write the digit in console
  }
}

// main procedure for printing "Hello World!    " on the console
int main(int argc, char** argv) {
  // point to the "Hello World!    " string
  foo = "Hello World!    ";

  // strings are actually stored in chunks of 8 characters in memory,
  // that is, here as "Hello Wo", and "rld!    " which allows us to
  // print them conveniently in chunks of 8 characters at a time
  lock();
  // as long as there are characters print them
  while (*foo != 0) {
    // 1 means that we print to the console
    // foo points to a chunk of 8 characters
    // 8 means that we print 8 characters
    write(1, foo, 8);

    // go to the next chunk of 8 characters
    foo = foo + 1;

  }
  
  id = get_pid();
  // print_number(id);

  unlock();
}