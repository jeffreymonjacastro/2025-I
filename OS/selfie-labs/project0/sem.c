uint64_t *sem;
uint64_t* c;  
uint64_t global_variable = 0;

void print_integer(uint64_t i) {
  // single character number
  *c = 32 * 256 + 48 + i;
  write(1, c, 2);
}

int main(int argc, char** argv) {
  uint64_t pid;

  c = malloc(sizeof(uint64_t));
  sem = malloc(sizeof(uint64_t));

  sem_init(sem, 1);

  pid = fork();

  if (pid) {
    sem_wait(sem);
    global_variable = 1;
    print_integer(global_variable);
    sem_post(sem);
  } else {
    sem_wait(sem);
    sleep(60);
    global_variable = 2;
    print_integer(global_variable);
    sem_post(sem);
  }


  return global_variable;
}