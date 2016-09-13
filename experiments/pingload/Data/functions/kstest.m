function retval = kstest(d1, d2)
	format short g;

	if iscell(d1)
		d1 = cell2mat(d1);
	end
	if iscell(d2)
		d2 = cell2mat(d2);
	end

	[p ks d] = kolmogorov_smirnov_test_2(d1, d2);
	d = d * 1000;
	retval = [p ks d]';
end
