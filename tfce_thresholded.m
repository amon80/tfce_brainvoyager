function [ tfce_image ] = tfce_thresholded(varargin)
% implementation of the Threshold-free cluster enhancement method
% developped for fMRI by Smith & Nichols, NeuroImage 44(2009), 83-98
%
% INPUT tfce_thresholded_image = tfce(data)
%       tfce_thresholded_image = tfce(data,E,H,dh)
%       tfce_thresholded_image = tfce(data,E,H,dh, nruns)
%
%       data must be 3D: a map of values      
%
% OUPUT tfce_thresholded_image is a map of p-values, each for voxel
% -----------------------------
%% check input

if nargin == 1
    nruns = 1000;
	E = 0.5;  
	H = 2;
	dh = 0.1;
elseif nargin == 2
    nruns = varargin{2};
    E = 0.5;  
	H = 2;
	dh = 0.1;
elseif nargin == 5
    nruns = varargin{2};
	E = varargin{3};
	H = varargin{4};
	dh = varargin{5};
elseif nargin > 5
	error('too many arguments')
end

data = varargin{1};
[x,y,z] = size(data);
clear varargin;

%Must be thresholded
tfce_score = tfce(data, E, H, dh);
tfce_score_max_perms = zeros(1,nruns);

for i=1:nruns
    %doesn't work
    data = perm(data);
    score_perm_i = tfce(data, E, H, dh);
    tfce_score_max_perms(i) = max(score_perm_i(:));
end

end

