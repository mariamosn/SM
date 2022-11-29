n = [1, 2, 4, 8, 16, 32];
s = arrayfun(@(x) (x + 3) / 4, n);
e = arrayfun(@(x) (x + 3) / (4 * x), n);
r = arrayfun(@(x) (x + log2(x)) / x, n);
u = arrayfun(@(x) (x + 3) * (x + log2(x)) / (4 * (x .^ 2)), n);
q = arrayfun(@(x) (x + 3) .^ 2 / (16 * (x + log2(x))), n);

plot(n, s, 'k', n, r, 'b', n, q, 'r', n, e, 'y', n, u, 'm')
xlabel ("n");
legend("S(n)", "R(n)", "Q(n)", "E(n)", "U(n)");