%=====================================================================
%Function:
%	FILTEREDDATA = cuttrace_data(DATA, THRESHOLD)
%
%Description:
%	Remove samples in DATA that is less than THRESHOLD.
%
%Return Values:
%	FILTEREDDATA: Filtered data, in format of loadall().
%
%Arguments:
%	DATA: Raw data, in format of loadall().
%	THRESHOLD: Threshold for the filter, single variable.
%=====================================================================

function cutdata = cuttrace_data(data, threshold)
	cutdata = data;
	ndata = length(cutdata);

	for i = 1:ndata
		trace = cell2mat(cutdata(2,i));
		trace = cuttrace(trace, threshold);
		cutdata(2,i) = trace;
		cutdata(3,i) = length(trace);
	end
end
