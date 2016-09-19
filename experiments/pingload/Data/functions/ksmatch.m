%=====================================================================
%WARNING: Obseleted.
%Function: 
%	KS_D = ksmatch(X, Y)
%
%Description:
%	Computes the Kolmogorov-Smirnov Distance between distribution 
%	a X and (X+Y).
%
%Return Values:
%	KS_D: Kolmogorov-Smirnov Distance between (X,X+Y). Multiplied 
%	by 1000 for readability.
%
%Arguments:
%	X,Y: Distributions to be evaluated. Both in a vector form.
%=====================================================================
function retval = ksmatch(d1, d2)
	format short g;

	if iscell(d1)
		d1 = cell2mat(d1);
	end
	if iscell(d2)
		d2 = cell2mat(d2);
	end
	
	dall = [d1; d2];

	[p ks d] = kolmogorov_smirnov_test_2(d1, dall);
	d = d * 1000;
	retval = [p ks d]';
end
