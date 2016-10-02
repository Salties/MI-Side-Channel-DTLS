function [d] = mvn_div_kl(p, q)
%MVN_DIV_KL  Compute the Kullback-Leibler (KL) divergence.
%
%   [d] = MVN_DIV_KL(m1, m2) computes the KL divergence between two
%   multivariate normals. d is never negative.

%   (c) 2010-2011, Dominik Schnitzer, <dominik.schnitzer@ofai.at>
%   http://www.ofai.at/~dominik.schnitzer/mvn
%
%   This file is part of the MVN Octave/Matlab Toolbox
%   MVN is free software: you can redistribute it and/or modify
%   it under the terms of the GNU General Public License as published by
%   the Free Software Foundation, either version 3 of the License, or
%   (at your option) any later version.
%
%   MVN is distributed in the hope that it will be useful,
%   but WITHOUT ANY WARRANTY; without even the implied warranty of
%   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
%   GNU General Public License for more details.
%
%   You should have received a copy of the GNU General Public License
%   along with MVN.  If not, see <http://www.gnu.org/licenses/>.

    n = length(p.m);
    m = q.m - p.m;

    d = 0.5 * ((q.logdet - p.logdet) + mvn_traceprod(q.icov, p.cov) + ...
        m' * q.icov * m - n);
    d = max(d, 0);
end
