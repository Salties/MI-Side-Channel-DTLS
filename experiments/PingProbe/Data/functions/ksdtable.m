%=====================================================================
%Function: 
%	[P_TBL D_TBL] = ksdtable(DATA)
%
%Description:
%	Computes the paired Kolmogorov-Smirnov Test P-value and
%	Distance among samples in DATA.
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
function [ptable dtable] = ksdtable(data)
	samplenum = length(data);
	%Plus one for title.
	width = samplenum + 2;
	height = samplenum + 1;
	%Initialise KS distance and p-valule table.
	dtable = 1000 * ones(height, width);
	ptable = 0 * ones(height, width);
	%Write titles.
	dtable(1,:) = [0:samplenum, 0];
	dtable(:,1) = [0:samplenum];
	ptable(1,:) = [0:samplenum, 0];
	ptable(:,1) = [0:samplenum];

	for i = 1:samplenum
		x = cell2mat(data(2,i));
		for j = i+1:samplenum
			y = cell2mat(data(2,j));
			[p,~,d] = kolmogorov_smirnov_test_2(x, y);
			dtable(i+1,j+1) = d * 1000;
			dtable(j+1,i+1) = d * 1000;
			ptable(i+1,j+1) = p;
			ptable(j+1,i+1) = p;

		end
		%Find the least KS distance match.
		ds = dtable(i + 1, :);
		[~, minindex] = min(ds(2:end));
		dtable(i+1, end) = minindex;
		%Find the most p-value match.
		ps = ptable(i + 1, :);
		[~, maxindex] = max(ps(2:end));
		ptable(i+1, end) = maxindex;

	end
end
