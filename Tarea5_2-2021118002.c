#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

void manejador_terminacion(int signo) {
    printf("bye %d\n", getpid());
    exit(0);
}

int main() {
    pid_t pids[10];
    struct sigaction sa;
    sa.sa_handler = manejador_terminacion;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGTERM, &sa, NULL);

    for (int i = 0; i < 10; ++i) {
        if ((pids[i] = fork()) == 0) {
            while (1) {
                printf("PID %d está corriendo\n", getpid());
                sleep(1);
            }
        }
    }

    srand(time(NULL));
    for (int i = 0; i < 10; ++i) {
        int idx = rand() % 10;
        if (pids[idx] != 0) {
            kill(pids[idx], SIGTERM);
            pids[idx] = 0;
            sleep(1);  // Añadir un pequeño retraso para observar mejor la terminación
        }
    }

    for (int i = 0; i < 10; ++i) {
        wait(NULL);
    }

    return 0;
}
