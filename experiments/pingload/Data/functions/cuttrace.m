%=====================================================================
%Function:
%	FILTEREDDATA = cuttrace(DATA, THRESHOLD)
%
%Description:
%	Remove samples in DATA that is less than THRESHOLD.
%
%Return Values:
%	FILTEREDDATA: Filtered data, as a vector.
%
%Arguments:
%	DATA: Raw data, in a vector.
%	THRESHOLD: Threshold for the filter, single variable.
%=====================================================================
function retval = cuttrace(data, threshold)
	format short g;
	retval = data(data > threshold);
end
