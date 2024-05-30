#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t turno = 0;

void manejador(int signo) {
    // Cambiar turno
    turno = !turno;
}

int main() {
    pid_t pid1, pid2;
    struct sigaction sa;
    sa.sa_handler = manejador;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);

    // Registrar manejador de señales
    sigaction(SIGUSR1, &sa, NULL);

    if ((pid1 = fork()) == 0) {
        // Primer hijo - imprime números impares
        for (int i = 1; i <= 9; i += 2) {
            while (turno != 0) { /* espera su turno */ }
            printf("Impar: %d\n", i);
            fflush(stdout);  // Asegura que se imprime inmediatamente
            turno = 1;  // Cambia el turno
            kill(getppid(), SIGUSR1);  // Notifica al padre
            pause();  // Espera la señal del padre para continuar
        }
        exit(0);
    }

    if ((pid2 = fork()) == 0) {
        // Segundo hijo - imprime números pares
        for (int i = 2; i <= 10; i += 2) {
            while (turno != 1) { /* espera su turno */ }
            printf("Par: %d\n", i);
            fflush(stdout);  // Asegura que se imprime inmediatamente
            turno = 0;  // Cambia el turno
            kill(getppid(), SIGUSR1);  // Notifica al padre
            pause();  // Espera la señal del padre para continuar
        }
        exit(0);
    }

    // Padre alterna el turno y notifica a los hijos
    for (int i = 0; i < 5; ++i) {
        pause();  // Espera la señal del primer hijo
        kill(pid2, SIGUSR1);  // Notifica al segundo hijo
        pause();  // Espera la señal del segundo hijo
        kill(pid1, SIGUSR1);  // Notifica al primer hijo
    }

    // Padre espera a que los hijos terminen
    wait(NULL);
    wait(NULL);

    return 0;
}
