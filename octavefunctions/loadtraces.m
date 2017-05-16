function [traces keyguess] = loadtraces(tracefiles, guessfile)
	%echo on
	files = dir(tracefiles);
	for i = 1:length(files)
		traces(:, i) = load(files(i).name);
	end

	keyguess = load(guessfile);
	%echo off
end
