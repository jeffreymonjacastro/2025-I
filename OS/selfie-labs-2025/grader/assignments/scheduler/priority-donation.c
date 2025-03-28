void sched_class(uint64_t class);
void priority_set(uint64_t value);

void lock();
uint64_t pthread_create();
void pthread_exit(uint64_t status);

uint64_t* c;

void print_integer(uint64_t i) {
  // single character number
  *c = 32 * 256 + 48 + i;
  write(1, c, 2);
}

uint64_t SCHED_PRIORITY = 1;

int main() {
	uint64_t pid;

	c = malloc(sizeof(uint64_t));

	sched_class(SCHED_PRIORITY);
	priority_set(20);
	lock();
	pid = pthread_create();

	if (pid == 0) {
		lock();
		print_integer(get_pid());
		unlock();
	} else {
		print_integer(get_pid());
		unlock();
	}
}