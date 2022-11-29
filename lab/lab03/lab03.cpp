#include <iostream>

using namespace std;

int shuffle(int x, int k) {
    int y = (x << 1);
    y = y + ((y & (1 << k)) >> k);
    return (y & ((1 << k) - 1));
}

void connect(int src, int dest, int k) {
    int crt = src, conn;
    for (int i = k; i >= 1; i--) {
        cout << "------------\nEtaj: " << i - 1 << endl;
        crt = shuffle(crt, k);
        cout << "Shuffle: " << crt << endl;
        cout << "Bloc: " << crt / 2 << endl;
        if ((src & (1 << (i - 1))) != (dest & (1 << (i - 1)))) {
            cout << "Conexiunea: indirectă" << endl;
            if (crt % 2) {
                crt--;
            } else {
                crt++;
            }
        } else {
            cout << "Conexiunea: directă" << endl;
        }
    }
}

int main() {
    int k, m, src, dest;

    cout << "Insert k:\n";
    cin >> k;

    cout << "Insert m:\n";
    cin >> m;

    for (int i = 1; i <= m; i++) {
        cout << "Insert src:\n";
        cin >> src;
        cout << "Insert dest:\n";
        cin >> dest;

        connect(src, dest, k);
    }

    return 0;
}
