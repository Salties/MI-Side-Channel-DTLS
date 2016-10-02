%=====================================================================
%Function: 
%	DATA = loadall(FOLDER, EXTENSION)
%
%Description:
%	Load all files in FOLDER with the extension EXTENSION.
%
%Return Values:
%	DATA: Loaded data.
%		DATA(1,:): Sample name (inherited from filename).
%		DATA(2,:): Sample data.
%		DATA(3,:): Sample size.
%
%Arguments:
%	FOLDER: Path to the raw data storage.
%	EXTENSION: Raw data file extension.
%=====================================================================
function data=loadall(fdir, fext)
	targetfiles = sprintf('%s/*.%s',fdir,fext);
	datfiles = dir(targetfiles);
	numfiles = length(datfiles);
	data=cell(3,numfiles);
	filenames = sort_nat({datfiles.name});

	for i = 1:numfiles
		filepath=sprintf('%s/%s', fdir, char(filenames(i)));
		[~, varname, ~] = fileparts(filepath);
		data(1,i) = varname;
		data(2,i) = load(filepath);
		data(3,i) = length(cell2mat(data(2,i)));
	end
end
