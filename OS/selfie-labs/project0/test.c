void print_integer(uint64_t i) {
  uint64_t* c;  
  c = malloc(sizeof(uint64_t));
  *c = 32 * 256 + 48 + i;
  write(1, c, 2);
}

int main(){
  uint64_t pid1;
  uint64_t pid2;

  pid1 = fork();
  pid2 = fork();

  if (pid1) {
    if (pid2) {
      // P0
      claim(0, 3);
      claim(1, 2);
      claim(2, 2);

      allocate(0, 1);
      allocate(1, 0);
      allocate(2, 0);

      request(0, 0);
      // request(2, 1);
    } else {
      // P3
      claim(0, 4);
      claim(1, 2);
      claim(2, 2);

      allocate(0, 0);
      allocate(1, 0);
      allocate(2, 2);

      // sleep(150);
    }
  } else {
    if (pid2) {
      // P1
      claim(0, 6);
      claim(1, 1);
      claim(2, 3);

      allocate(0, 6);
      allocate(1, 1);
      allocate(2, 2);

      // sleep(50);
    } else {
      // P2
      claim(0, 3);
      claim(1, 1);
      claim(2, 4);

      allocate(0, 2);
      allocate(1, 1);
      allocate(2, 1);

      // sleep(100);
    }
  }



  return 0;
}