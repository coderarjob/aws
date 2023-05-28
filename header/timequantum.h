#ifndef TIME_QUANTUM_H
#define TIME_QUANTUM_H

	void StartTimeQuantum(int seconds);
	void timeQuantunExpirationHandler(int signal);
	unsigned int IsTimeQuantumExpired();
	
#endif //TIME_QUANTUM_H