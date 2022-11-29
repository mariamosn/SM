# Adunarea a n elemente, pe p procesoare
# Cele n elemente sunt împărțite între cele p procesoare.
# Fiecare procesor face suma a câte n/p elemente. => p * (n / p) operații
# După care sumele parțiale de pe cele p procesoare sunt adunate,
# folosind o strategie de tip Divide et Impera. => log(p) operații
# O(n, p) = p * n / p + log(p) = n + log(p)
# O(n, 1) = n
# T(n, p) = n / p + log(p)
# T(n, 1) = n
# => O(n, 1) = T(n, 1)

# Pentru simplitate, consider că n = p în acest caz.
# O(n) = n + log(n); O(1) = n
# T(n) = 1 + log(n); T(1) = n

# S(n) = T(1) / T(n) = n / (1 + log(n)) = n / (1 + log(n))
# E(n) = S(n) / n = 1 / (1 + log(n))
# R(n) = O(n) / O(1) = (n + log(n)) / n = 1 + log(n) / n
# U(n) = R(n) * E(n) = (1 + log(n) / n) / (1 + log(n))
# Q(n) = S(n) * E(n) / R(n) = n ^ 2 / [(1 + log(n)) ^ 3]

n = [1, 2, 4, 8, 16, 32];
s = arrayfun(@(x) x / (1 + log2(x)) , n);
e = arrayfun(@(x) 1 / (1 + log2(x)), n);
r = arrayfun(@(x) 1 + log2(x) / x, n);
u = arrayfun(@(x) (1 + log2(x) / x) / (1 + log2(x)), n);
q = arrayfun(@(x) x .^ 2 / ((1 + log2(x)) .^ 3), n);

plot(n, s, 'k', n, r, 'b', n, q, 'r', n, e, 'y', n, u, 'm')
xlabel ("n");
legend("S(n)", "R(n)", "Q(n)", "E(n)", "U(n)");