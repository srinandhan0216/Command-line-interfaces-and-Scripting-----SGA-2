#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define NUM_CHILDREN 4       // Number of child processes to create
#define TIMEOUT_SECONDS 5    // Max allowed runtime before a child is killed
// Struct to track each child's info
typedef struct {
    pid_t pid;
    time_t start_time;
    int finished;
} ChildInfo;

int main() {
    ChildInfo children[NUM_CHILDREN];

    printf("Parent process started. PID = %d\n", getpid());

    // Step 1: Create child processes using fork()
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // fork() failed
            perror("fork failed");
            exit(1);
        }
        else if (pid == 0) {
            // ---- CHILD PROCESS CODE ----
            // Child index 3 deliberately sleeps too long to test timeout/kill logic
            int sleep_time = (i == 3) ? 8 : (i + 1) * 2;
            printf("Child %d (PID=%d) started, will run for %d seconds.\n",
                   i, getpid(), sleep_time);
            sleep(sleep_time);
            printf("Child %d (PID=%d) finished work normally.\n", i, getpid());
            exit(0);
        }
        else {
            // ---- PARENT PROCESS CODE ----
            children[i].pid = pid;
            children[i].start_time = time(NULL);
            children[i].finished = 0;
        }
    }
    // Step 2: Monitor children in a loop without blocking
    int remaining = NUM_CHILDREN;
    while (remaining > 0) {
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (children[i].finished) continue;

            int status;
            // WNOHANG makes waitpid() return immediately if the child hasn't
            // exited yet, instead of blocking the parent -- this lets us
            // check on ALL children in a loop.
            pid_t result = waitpid(children[i].pid, &status, WNOHANG);

            if (result == 0) {
                // Child still running -- check if it has timed out
                time_t now = time(NULL);
                if (now - children[i].start_time > TIMEOUT_SECONDS) {
                    printf("Parent: Child %d (PID=%d) exceeded timeout. Sending SIGTERM.\n",
                           i, children[i].pid);
                    kill(children[i].pid, SIGTERM);
                    sleep(1);
                    waitpid(children[i].pid, &status, 0);
                    children[i].finished = 1;
                    remaining--;
                }
            }
            else if (result == children[i].pid) {
                if (WIFEXITED(status)) {
                    printf("Parent: Child %d (PID=%d) exited normally with status %d.\n",
                           i, children[i].pid, WEXITSTATUS(status));
                } else if (WIFSIGNALED(status)) {
                    printf("Parent: Child %d (PID=%d) was terminated by signal %d.\n",
                           i, children[i].pid, WTERMSIG(status));
                }
                children[i].finished = 1;
                remaining--;
            }
        }
        sleep(1); // Poll every second instead of busy-looping
    }
    printf("Parent: All children have been handled. No zombies remain.\n");
    return 0;
}
