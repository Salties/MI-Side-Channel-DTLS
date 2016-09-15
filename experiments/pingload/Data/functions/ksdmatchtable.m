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
