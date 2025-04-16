void* malloc(unsigned long);

uint64_t write(uint64_t fd, uint64_t* buffer, uint64_t bytes_to_write);

uint64_t pthread_create();
uint64_t pthread_join(uint64_t* status);
void     pthread_exit(uint64_t status);

uint64_t* c;

void print_integer(uint64_t i) {
  // single character number
  *c = 32 * 256 + 48 + i;
  write(1, c, 2);
}

int main(int argc, char** argv) {
  uint64_t  tid;
  uint64_t* status;

  status = malloc(sizeof(uint64_t));
  c = malloc(sizeof(uint64_t));

  tid = pthread_create();
  tid = pthread_create();
  tid = pthread_create();
  
  lock();
  print_integer(get_pid());
  unlock();

  if (tid) {
    sleep(200);
  }
  else {
    sleep(600);
  }

  lock();
  print_integer(get_pid());
  unlock();
 

  pthread_exit(0);
  
  return 0;
}