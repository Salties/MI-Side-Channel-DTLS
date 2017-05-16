%Prototype:
%	[idxvec corrvec pointvec] = maxcorr(corrmtx)
%
%Description:
%	Find maximum correlation coefficients.
%
%Input:
%	corrmtx(i, j): Correlation at point i with key = j.
%
%Return:
%	keyvec		: Sub key guess.
%	corrvec		: Maximum correlation.
%	pointvec	: Point of maximun correlation.

function [keyvec corrvec pointvec] = maxcorr(corrmtx)
	for i = 1:size(corrmtx, 2)
		keyvec(i) = i - 1;
		[corrvec(i) pointvec(i)] = max(corrmtx(:,i));
	end
	keyvec = keyvec';
	corrvec = corrvec';
	pointvec = pointvec';
end
