// Maria Moșneag 343C1

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define M 0
#define E 1
#define S 2
#define I 3

#define PRRD 0
#define PRWR 1
#define BUSRD 2
#define BUSRDX 3
#define NONE 4
#define FLUSH 5
#define FLUSHSTAR 6

#define NOT_USED 0
#define USED 1

using namespace std;

int next_state(int crt_state, int &bus_act, int op_type, int s) {
	if (crt_state == I) {
		if (op_type == PRRD && s == USED) {
			bus_act = BUSRD;
			return S;
		}

		if (op_type == PRRD && s == NOT_USED) {
			bus_act = BUSRD;
			return E;
		}

		if (op_type == PRWR) {
			bus_act = BUSRDX;
			return M;
		}

		bus_act = NONE;
		return I;
	}

	if (crt_state == S) {
		if (op_type == PRRD) {
			bus_act = NONE;
			return S;
		}

		if (op_type == BUSRD) {
			bus_act = FLUSHSTAR;
			return S;
		}

		if (op_type == BUSRDX) {
			bus_act = FLUSHSTAR;
			return I;
		}

		if (op_type == PRWR) {
			bus_act = BUSRDX;
			return M;
		}

		bus_act = NONE;
		return S;
	}

	if (crt_state == E) {
		if (op_type == PRRD) {
			bus_act = NONE;
			return E;
		}

		if (op_type == BUSRD) {
			bus_act = FLUSH;
			return S;
		}

		if (op_type == BUSRDX) {
			bus_act = FLUSH;
			return I;
		}

		if (op_type == PRWR) {
			bus_act = NONE;
			return M;
		}

		bus_act = NONE;
		return E;
	}

	if (crt_state == M) {
		if (op_type == PRRD || op_type == PRWR) {
			bus_act = NONE;
			return M;
		}

		if (op_type == BUSRD) {
			bus_act = FLUSH;
			return S;
		}

		if (op_type == BUSRDX) {
			bus_act = FLUSH;
			return I;
		}

		bus_act = NONE;
		return M;
	}

	bus_act = NONE;
	return crt_state;
}

int main() {
    int n;
	string input;
	int crt_proc;
	int op_type;
	int bus_act;
	int s;
	vector<char> states{'M', 'E', 'S', 'I'};
	vector<string> bus_actions{"PrRd", "PrWr", "BusRd", "BusRdX", "-", "Flush", "Flush*"};

	cout << "Enter the number of processors (< 10):\n";
	cin >> n;

	vector<int> crt_state(n, I);

	// print table head
	cout << "Processor action|\t";
	for (int i = 0; i < n; i++) {
		cout << "P" << i << "\t|\t";
	}
	cout << "Bus action\n";

	// print initial state
	cout << "\tInitial\t|\t";
	for (int i = 0; i < n; i++) {
		cout << "I" << "\t|\t";
	}
	cout << "-\n";

	while (true) {
		cin >> input;

		// exit
		if (input == "exit") {
			break;
		}
		
		// wrong input
		if (input.length() != 4 ||
				input[0] != 'P' ||
				input[1] < '0' ||
				input[1] >= '0' + n ||
				(input.substr(2, 2) != "Rd" &&
				input.substr(2, 2) != "Wr")) {
			cout << "Wrong input, try again.\n";
			continue;
		}

		// parse input
		crt_proc = input[1] - '0';
		if (input.substr(2, 2) == "Rd") {
			op_type = PRRD;
		} else {
			op_type = PRWR;
		}

		// check if S or S#
		s = NOT_USED;
		for (int i = 0; i < n; i++) {
			if (crt_state[i] != I) {
				s = USED;
				break;
			}
		}

		cout << "\t" << input << "\t|\t";

		crt_state[crt_proc] = next_state(crt_state[crt_proc],
								bus_act, op_type, s);

		int aux_bus_act;
		for (int i = 0; i < n; i++) {
			if (i != crt_proc) {
				crt_state[i] = next_state(crt_state[i],
								aux_bus_act, bus_act, s);
			}

			cout << states[crt_state[i]] << "\t|\t";
		}

		cout << bus_actions[bus_act] << endl;
	}

    return 0;
}
