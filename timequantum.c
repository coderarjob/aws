#include <unistd.h>
#include <signal.h>
#include "header/timequantum.h"

static unsigned int quantumExpired;

void StartTimeQuantum(int seconds)
{
	quantumExpired = 0;
	alarm(seconds);
	signal(SIGALRM,timeQuantunExpirationHandler);
}

void timeQuantunExpirationHandler(int signal)
{
	quantumExpired = 1;
}

unsigned int IsTimeQuantumExpired()
{
	return (quantumExpired == 1);
}