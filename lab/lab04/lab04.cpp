#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int main() {
    int n, k;
    vector<vector<int>> perm;
    vector<int> p1, p2;
    vector<int> left, right;

    cout << "Insert N.\n";
    cin >> n;

    k = log2(n);

    cout << "Insert permutation.\n";
    for (int i = 0; i < n; i++) {
        int x;
        p1.push_back(i);
        cin >> x;
        p2.push_back(x);

        left.push_back(i);
        right.push_back(i);
    }
    perm.push_back(p1);
    perm.push_back(p2);

    for (int i = 1; i < k; i++) {
        // i = stage
        // ordonarea perechilor
        vector<vector<int>> pairs;
        vector<int> pair;
        vector<int> visited(n, 0);
        int crt = 1;

        visited[0] = 1;
        pair.push_back(perm[0][0]);
        pair.push_back(perm[1][0]);
        pairs.push_back(pair);

        while (pairs.size() < n) {
            int crt_pos, neigh_pos, neigh = -1, ok = 0;

            if (crt == 1) {
                crt_pos = right[pairs[pairs.size() - 1][crt]];
            } else {
                crt_pos = left[pairs[pairs.size() - 1][crt]];
            }

            if (crt_pos % 2)
                neigh_pos = crt_pos - 1;
            else
                neigh_pos = crt_pos + 1;
            
            if (crt == 1) {
                for (int j = 0; j < n && neigh == -1; j++) {
                    if (right[j] == neigh_pos)
                        neigh = j;
                }
            } else {
                for (int j = 0; j < n && neigh == -1; j++) {
                    if (left[j] == neigh_pos)
                        neigh = j;
                }
            }

            for (int j = 0; j < n && !ok; j++) {
                if (perm[crt][j] == neigh) {
                    if (visited[perm[0][j]] == 0) {
                        vector<int> p;
                        p.push_back(perm[0][j]);
                        p.push_back(perm[1][j]);
                        pairs.push_back(p);
                        visited[perm[0][j]] = 1;
                        ok = 1;
                    }
                    break;
                }
            }

            if (!ok) {
                for (int j = 0; j < n; j++) {
                    if (!visited[perm[0][j]]) {
                        vector<int> p;
                        p.push_back(perm[0][j]);
                        p.push_back(perm[1][j]);
                        pairs.push_back(p);
                        visited[perm[0][j]] = 1;
                        ok = 1;
                        crt = 0;
                        break;
                    }
                }
            }

            crt = 1 - crt;
        }

        // perechile de pe pozițiile pare (numerotarea începe de la 0)
        // se vor conecta la subrețeaua Benes superioară
        // iar cele de pe poziții imapre, la cea inferioară

        vector<int> left_conn(n, 0), right_conn(n, 0);
        for (int j = 0; j < pairs.size(); j++) {
            int a = pairs[j][0], b = pairs[j][1];
            if (j % 2) {
                left_conn[left[a]] = -1;
                right_conn[right[b]] = -1;
            } else {
                left_conn[left[a]] = 1;
                right_conn[right[b]] = 1;
            }
        }

        cout << endl << "Stage " << i << ":\n";
        for (int j = 0; j < n / 2; j++) {
            if (left_conn[2 * j] == 1)
                cout << "direct interconn\n";
            else
                cout << "inverse interconn\n";
        }

        cout << endl << "Stage " << 2 * k - 1 - (i - 1) << ":\n";
        for (int j = 0; j < n / 2; j++) {
            if (right_conn[2 * j] == 1)
                cout << "direct interconn\n";
            else
                cout << "inverse interconn\n";
        }

        // transfera porturile in din stanga la out
        for (int j = 0; j < n; j++) {
            if (left[j] % 2 == 1 && left_conn[left[j]] == 1) {
                left[j] = left[j] - 1;;
            } else if (left[j] % 2 == 0 && left_conn[left[j]] == -1) {
                left[j] = left[j] + 1;
            }
        }

        // transfera porturile out din dreapta la in
        for (int j = 0; j < n; j++) {
            if (right[j] % 2 == 1 && right_conn[right[j]] == 1) {
                right[j] = right[j] - 1;;
            } else if (right[j] % 2 == 0 && right_conn[right[j]] == -1) {
                right[j] = right[j] + 1;
            }
        }

        // transfera porturile de la out la in pentru stage-ul urmator (in stanga)
        // si porturile in la out pentru stage-ul anterior (in dreapta)
        int groups = (1 << (i - 1));
        int cnt = n / groups;
        for (int j = 0; j < n; j++) {
            // stanga
            int group = left[j] / cnt;
            int gr_off = group * cnt;
            if (left[j] % 2 == 0) {
                left[j] = (left[j] % cnt) / 2 + gr_off;
            } else {
                left[j] = cnt / 2 + (left[j] % cnt) / 2 + gr_off;
            }

            // dreapta
            group = right[j] / cnt;
            gr_off = group * cnt;
            if (right[j] % 2 == 0) {
                right[j] = (right[j] % cnt) / 2 + gr_off;
            } else {
                right[j] = cnt / 2 + (right[j] % cnt) / 2 + gr_off;
            }
        }

        // reordonarea permutarii
        vector<int> p1_new, p2_new;
        for (int poz = 0; poz < n; poz++) {
            int val = -1;
            for (int k = 0; k < n && val == -1; k++) {
                if (left[k] == poz)
                    val = k;
            }

            p1_new.push_back(val);

            int aux = -1;
            for (int k = 0; k < n && aux == -1; k++) {
                if (perm[0][k] == val)
                    aux = perm[1][k];
            }
            p2_new.push_back(aux);
        }
        perm.clear();
        perm.push_back(p1_new);
        perm.push_back(p2_new);
    }

    cout << "\nStage " << k << ":\n";
    for (int i = 0; i < n / 2; i++) {
        if (left[perm[0][i * 2]] == right[perm[1][i * 2]]) {
            cout << "direct interconn\n";
        } else {
            cout << "inverse interconn\n";
        }
    }

    return 0;
}
