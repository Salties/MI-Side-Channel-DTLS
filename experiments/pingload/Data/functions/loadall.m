function data=loadall(fdir, fext)
	targetfiles = sprintf('%s/*.%s',fdir,fext);
	datfiles = dir(targetfiles);
	numfiles = length(datfiles);
	data=cell(2,numfiles);
	filenames = sort_nat({datfiles.name});

	for i = 1:numfiles
		filepath=sprintf('%s/%s', fdir, char(filenames(i)));
		[~, varname, ~] = fileparts(filepath);
		data(1,i) = varname;
		data(2,i) = load(filepath);
	end
end
