void sched_class(uint64_t class);
void priority_set(uint64_t value);

uint64_t pthread_create();
void pthread_exit(uint64_t status);
void* malloc(unsigned long);

uint64_t* c;

void print_integer(uint64_t i) {
  // single character number
  *c = 32 * 256 + 48 + i;
  write(1, c, 2);
}

uint64_t SCHED_PRIORITY = 1;

int main() {
	uint64_t pid;
	uint64_t priority;

	c = malloc(sizeof(uint64_t));

	sched_class(SCHED_PRIORITY);
	
	priority = 20;
	priority_set(priority);

	pid = pthread_create();

	if (pid == 0) {
		priority = 25;
		priority_set(priority);
	}

	pid = pthread_create();

	print_integer(get_pid());

	pthread_exit(0);
}