function ret = ksdtable(data)
	samplenum = length(data);
	tablex = samplenum + 2;
	tabley = samplenum + 1;
	ret = 0 * ones(tabley, tablex);
	ret(1,:) = [0:samplenum, 0];
	ret(:,1) = [0:samplenum];
	
	for i = 1:samplenum
		x = cell2mat(data(2,i));
		for j = i+1:samplenum
			y = cell2mat(data(2,j));
			[~,~,d] = kolmogorov_smirnov_test_2(x, y);
			ret(i+1,j+1) = d * 1000;
			ret(j+1,i+1) = d * 1000;
		end
	end
end
