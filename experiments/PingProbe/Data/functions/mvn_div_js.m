function d = mvn_div_js(m1, m2, use_kl)
%MVN_DIV_JS Compute the Jensen-Shannon (JS) divergence between two multivariate normals.
%
%   [d] = MVN_DIV_JS(m1, m2) computes the JS divergence between two
%   multivariate normals. d is never negative.
%
%   The JS divergence is defined as:
%       d = 0.5*KL(m1, m1+m2) + 0.5*KL(m2, m1+m2)

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

    % Speedup:
    %
    %    m12 = mvn_bregmancentroid_kl_left([m1 m2]);
    %
    % using Cholesky & more Optimization

    m12.m = 0.5*m1.m + 0.5*m2.m;
    m12.cov = 0.5*(m1.cov + m1.m*m1.m') + 0.5*(m2.cov + m2.m*m2.m') ...
        - m12.m*m12.m';
    m12_chol = chol(m12.cov);
    m12.logdet = 2*sum(log(diag(m12_chol)));
    
    if ((nargin > 2) && (use_kl == 1))
        m12_ui = m12_chol\eye(length(m12.m));
        m12.icov = m12_ui*m12_ui';

        d = 0.5*mvn_div_kl(m1, m12) + 0.5*mvn_div_kl(m2, m12);
    else
        % Speedup original (entropy):
        %
        % d = mvn_entropy(m12) - 0.5*mvn_entropy(m1) - 0.5*mvn_entropy(m2);
        %
        % faster:
        
        d = 0.5*m12.logdet - 0.25*m1.logdet - 0.25*m2.logdet;
    end
    
    d = max(d, 0);
end
