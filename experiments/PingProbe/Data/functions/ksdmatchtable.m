%=====================================================================
%WARNING: Obseleted.
%Function: 
%	[P_TBL D_TBL] = ksdmatchtable(DATA)
%
%Description:
%	Computes the paired Kolmogorov-Smirnov Test P-value and
%	Distance among samples in DATA. The tests are performed on 
%	(X, X+Y).
%
%Return Values:
%	The first row and column in P_TBL and D_TBL represents the 
%	index of samples in DATA, as specified by DATA(1,:). The last
%	column presents the best match in each row.
%	
%	P_TBL: p-values of the tests. p-values for self tests are 
%	defined as 0. Best match defined as the maximum of each row.
%
%	D_TBL: KS Distance of the tests, multiplied by 1000 for
%	readability. The distance of self tests are defined as 1000.
%	Best match defined as the minimum of each row.
%
%Arguments:
%	DATA: Data set to be tested. In the format defined by 
%	loadall().
%=====================================================================
function [ptbl dtbl]= ksdmatchtable(data)
	samplenum = length(data);
	tablex = samplenum + 1;
	tabley = samplenum + 1;

	%Initialise KS p-value table.
	ptbl = 0 * ones(tabley, tablex);
	ptbl(1,:) = [0:samplenum];
	ptbl(:,1) = [0:samplenum];

	%Initialise KS distance table.
	dtbl = 1000 * ones(tabley, tablex);
	dtbl(1,:) = [0:samplenum];
	dtbl(:,1) = [0:samplenum];

	%Perform KS test on (x = data[i], y = data[i] + data[j]).
	for i = 1:samplenum
		for j = 1:samplenum
			if i == j %The same data.
				dtbl(i,j) = 1000; %Ignore distance.
				ptbl(i,j) = 0; %Ignore p-value.
				continue;
			end
			x = cell2mat(data(2,i));
			y = [cell2mat(data(2,j)); x]; %Concatenate y by x
			%Perform KS test.
			[p, ~, d] = kolmogorov_smirnov_test_2(x, y);
			%Record p-value.
			ptbl(i+1,j+1) = p;
			%Record KS distance.
			dtbl(i+1,j+1) = d * 1000;
		end
	end
end
