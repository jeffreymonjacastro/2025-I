volatile int i;

int main() {
  __atomic_fetch_add(&i, 1, __ATOMIC_SEQ_CST);
  return 0;
}
