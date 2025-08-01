void* malloc(uint64_t size);

uint64_t fork();
uint64_t wait(uint64_t* status);
void exit(uint64_t status);

uint64_t get_pid();
uint64_t write(uint64_t fd, uint64_t* buffer, uint64_t size);
void sleep(uint64_t seconds);
void lock();
void unlock();

void sem_init(uint64_t* semaphore, uint64_t value);
void sem_wait(uint64_t* semaphore);
void sem_post(uint64_t* semaphore);

uint64_t n_producers = 4;
uint64_t m_consumers = 4;
uint64_t buffer_size = 3;

uint64_t* buffer_data;
uint64_t buffer_in;
uint64_t buffer_out;
uint64_t buffer_count;

uint64_t* empty;
uint64_t* full;
uint64_t* mutex;

uint64_t get_byte(uint64_t word, uint64_t byte_index) {
    uint64_t i;
    i = 0;

    while (i < byte_index) {
        word = word / 256;
        i = i + 1;
    }
    return word % 256;
}

void print_string(uint64_t *str) {
    uint64_t *ptr;
    uint64_t b;
    uint64_t c;
    uint64_t *buff;

    buff = malloc(8);
    ptr = str;

    while (1) {
        if (*ptr == 0)
            return;

        b = 0;
        while (b < 8) {
            c = get_byte(*ptr, b);

            if (c == 0)
                return;

            *buff = c;
            write(1, buff, 1);
            b = b + 1;
        }

        ptr = ptr + 1;
    }
}

void print_number(uint64_t number) {
    uint64_t digit;
    uint64_t divisor;
    uint64_t *d;

    d = malloc(8);

    if (number == 0) {
        *d = '0';
        write(1, d, 1);
        return;
    }

    divisor = 1;
    while (number / divisor >= 10) {
        divisor = divisor * 10;
    }

    while (divisor > 0) {
        digit = number / divisor;
        number = number % divisor;
        divisor = divisor / 10;
        *d = 48 + digit;
        write(1, d, 1);
    }
}

void delay(uint64_t iterations)
{
    uint64_t i;
    i = 0;
    while (i < iterations)
    {
        i = i + 1;
    }
}

void print_buffer() {
    uint64_t i;
    
    print_string("Buffer: | ");
    
    i = 0;
    while (i < buffer_size) {
        print_number(*(buffer_data + i));
        print_string(" | ");
        i = i + 1;
    }
    
    print_string("\n");
}

void print_producer(uint64_t item, uint64_t pos) {
    print_string("Producer ");
    print_number(get_pid());
    print_string(" repone el item: ");
    print_number(item);
    print_string(" en la posición: ");
    print_number(pos);
    print_string(" ");
}

void print_consumer(uint64_t item, uint64_t pos) {
    print_string("Consumer ");
    print_number(get_pid() - n_producers);
    print_string(" comeee el item: ");
    print_number(item);
    print_string(" de la posición: ");
    print_number(pos);
    print_string(" ");
}

void buffer_insert(uint64_t item) {
    *(buffer_data + buffer_in) = item;
    buffer_in = (buffer_in + 1) % buffer_size;
    buffer_count = buffer_count + 1;
}

uint64_t buffer_remove() {
    uint64_t item;
    item = *(buffer_data + buffer_out);
    *(buffer_data + buffer_out) = 0;
    buffer_out = (buffer_out + 1) % buffer_size;
    buffer_count = buffer_count - 1;
    return item;
}

void producer_process() {
    uint64_t count;
    uint64_t item;
    uint64_t pos;
    
    count = 0;
    
    while (count < 1) {
        item = get_pid();
        
        sem_wait(empty);
        sem_wait(mutex);

        pos = buffer_in;
        buffer_insert(item);
        print_producer(item, pos);
        print_buffer();
        delay(100000);

        sem_post(mutex);
        sem_post(full);

        count = count + 1;
    }
    
    exit(0);
}

void consumer_process() {
    uint64_t count;
    uint64_t item;
    uint64_t pos;
    
    count = 0;
    
    while (count < 1) {
        sem_wait(full);
        sem_wait(mutex);

        pos = buffer_out;
        item = buffer_remove();
        print_consumer(item, pos);
        print_buffer();
        delay(100000);

        sem_post(mutex);
        sem_post(empty);

        count = count + 1;
    }
    
    exit(0);
}

uint64_t main(uint64_t argc, uint64_t* argv) {
    uint64_t i;
    uint64_t total_processes;
    uint64_t* process_ids;
    uint64_t* status;
    uint64_t pid;
    
    // -- Buffer --
    buffer_data = malloc(buffer_size * sizeof(uint64_t));
    buffer_in = 0;
    buffer_out = 0;
    buffer_count = 0;

    i = 0;
    while (i < buffer_size) {
        *(buffer_data + i) = 0;
        i = i + 1;
    }
    
    // -- Sem --
    empty = malloc(sizeof(uint64_t));
    full = malloc(sizeof(uint64_t));
    mutex = malloc(sizeof(uint64_t));

    // -- Sem init --
    sem_init(empty, buffer_size);
    sem_init(full, 0);
    sem_init(mutex, 1);
    
    total_processes = n_producers + m_consumers;
    process_ids = malloc(total_processes * sizeof(uint64_t));
    status = malloc(sizeof(uint64_t));
    
    i = 0;
    while (i < n_producers) {
        pid = fork();
        
        if (pid == 0) {
            producer_process();
        } else {
            *(process_ids + i) = pid;
        }
        i = i + 1;
    }
    
    i = 0;
    while (i < m_consumers) {
        pid = fork();
        
        if (pid == 0) {
            consumer_process();
        } else {
            *(process_ids + i + n_producers) = pid;
        }
        i = i + 1;
    }
    
    i = 0;
    while (i < total_processes) {
        if (*(process_ids + i) != 0) {
            wait(status);
        }
        i = i + 1;
    }
    
    return 0;
}