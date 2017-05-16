%Prototype:
%	computecorrmtx(trace, guess)
%
%Description:
%	Compute correlation coefficient matrix.
%
%Input:
%	trace(m, n): m points, n traces.
%	guess(n, k): trace n, key = k.
%
%Return:
%	corrmtx(i, j): Correlation at point i with key = j.

function corrmtx = computecorrmtx(trace, guess)
	for i = 1:size(trace, 1)
		for j = 1:size(guess,2)
			%Correlaton under point 'i' in traces with key guess 'j'.
			corrmtx(i, j) = corr(trace(i,:), guess(:,j));
		end
	end
end
