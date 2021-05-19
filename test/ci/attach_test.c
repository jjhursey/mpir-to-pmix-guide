#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MPIR_SHIM_TESTCASE
#include "mpirshim.h"
#include "mpirshim_test.h"

typedef struct MPIR_PROCDESC {
  char *host_name;
  char *executable_name;
  int pid;
} MPIR_PROCDESC;

extern int MPIR_proctable_size;
extern MPIR_PROCDESC *MPIR_proctable;
extern char *MPIR_debug_abort_string;

char *target_cmd[] = {"prterun", "-n", "4", "--host", "localhost:4", "hello",
                      "10", NULL};

void timeout(int signum) {
    /* Send 3 kills to prterun so it gracefully terminates */
    system("killall prterun");
    system("killall prterun");
    system("killall prterun");
    fprintf(stderr, "ERROR: Shim launch test timed out\n");
    exit(1);
}

void MPIR_Breakpoint_hook(void)
{
    int i;
    int rc;

    if (4 != MPIR_proctable_size) {
        fprintf(stderr, "ERROR: Expected MPIR_proctable_size=4, is %d\n",
                MPIR_proctable_size);
        exit(1);
    }
    for (i = 0; i < MPIR_proctable_size; i++) {
        if ('\0' == MPIR_proctable[i].host_name[0]) {
            fprintf(stderr, "ERROR: Expected hostname[%d], is empty\n", i);
            exit(1);
        }
        if (0 != strcmp(basename(MPIR_proctable[i].executable_name), "hello")) {
            fprintf(stderr, "ERROR: Expected executableName[%d]=hello, is %s\n",
                    i, MPIR_proctable[i].executable_name);
            exit(1);
        }
        if (0 == MPIR_proctable[i].pid) {
            fprintf(stderr, "ERROR: pid[%d] is zero\n", i);
            exit(1);
        }
    }
    rc = MPIR_Shim_release_application();
    if (0 != rc) {
        fprintf(stderr, "ERROR: MPIR_Shim_release_application failed, rc %d\n",
                rc);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    int rc;
    pid_t pid;

    /* Set a 30 second time limit for this test */
    signal(SIGALRM, timeout);
    alarm(30);
    pid = fork();
    if (0 == pid) {
        execvp("prterun", target_cmd);
        fprintf(stderr, "ERROR: prterun exec failed\n");
        exit(1);
    }
    else if (-1 == pid) {
        fprintf(stderr, "ERROR: fork for prterun failed\n");
        exit(1);
    }

    /* This is the parent process Sleep for a few seconds so prterun can launch */
    sleep(5);
    rc = MPIR_Shim_common(MPIR_SHIM_ATTACH_MODE, pid, 0, 0, NULL, NULL);
    if (0 != rc) {
        fprintf(stderr, "ERROR: Invocation of shim module failed, rc %d\n", rc);
        wait(NULL);
        exit(1);
    }
    if (NULL != MPIR_debug_abort_string) {
        fprintf(stderr, "ERROR: MPIR_debug_abort_string not NULL, is %s\n",
                MPIR_debug_abort_string);
        wait(NULL);
        exit(1);
    }
    wait(&rc);
    if (0 != WEXITSTATUS(rc)) {
        fprintf(stderr, "ERROR: prterun exited with rc %d\n", rc);
        exit(1);
    }
    printf("MPIR shim attach test completed successfully\n");
    exit(0);
}
      

    
