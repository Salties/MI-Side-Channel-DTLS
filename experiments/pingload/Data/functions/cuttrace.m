function retval = cuttrace(data, threshold)
	format short g;
	retval = data(data > threshold);
end
