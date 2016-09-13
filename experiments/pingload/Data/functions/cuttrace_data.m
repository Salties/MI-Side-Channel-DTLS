function cutdata = cuttrace_data(data, threshold)
	cutdata = data;
	ndata = length(cutdata);
	disp(ndata);

	for i = 1:ndata
		trace = cell2mat(cutdata(2,i));
		trace = cuttrace(trace, threshold);
		cutdata(2,i) = trace;
	end
end
