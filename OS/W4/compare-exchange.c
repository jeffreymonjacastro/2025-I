int main() {
  int lock_val = 0;
  int c;
  
  c = __sync_val_compare_and_swap(&lock_val, 0, 1);
}

