#include <stdio.h>
#include <unistd.h>

void miTurno() {
	while (1) {
		sleep(1);
		printf("Es mi turno!\n");
	}
}

void tuTurno() {
	while(1) {
		sleep(2);
		printf("Es tu turno!\n");
	}
}

int main () {
	miTurno();
	tuTurno();

	return 0;
}
