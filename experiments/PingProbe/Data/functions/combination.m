%Compute C^{m}_{n}
function ret = combination(m, n)
	Cn = factorial(n);
	Cd = factorial(m) * factorial(n-m);
	ret = Cn / Cd;
end
