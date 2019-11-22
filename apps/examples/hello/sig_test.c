#include <stdio.h>
#include <signal.h>

void signal_handler(int sig, siginfo_t *info, void *ctx)
{
	printf("!! : %d, %d\n", sig, info->si_value.sival_int);
	// gotSigint = 1;
}

int sig_test_main()
{
	int i;
	struct sigaction sigact;

	printf("%s: PID is %ld\n", argv[0], (long)getpid());

	sigact.sa_handler = signal_handler;
	sigfillset(&sigact.sa_mask);
	// sigact.sa_flags = SA_RESETHAND;
	// sigact.sa_flags = SA_RESTART;
	sigact.sa_flags = SA_SIGINFO;

	sigaction(SIGUSR1, &sigact, NULL);
	// sigaction(SIGINT, &sigact, NULL);
	union sigval val;
	int cnt = 0;

	while (cnt < 10) {
		val.sival_int = cnt++;
		sigqueue(getpid(), SIGUSR1, val);
		// continue;
	}

	return 0;
}