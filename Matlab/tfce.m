function tfce_score = tfce(varargin)

% implementation of the Threshold-free cluster enhancement method
% developped for fMRI by Smith & Nichols, NeuroImage 44(2009), 83-98
%
% INPUT tfce_score = tfce(data)
%       tfce_score = tfce(data,E,H,dh)
%
%       data must be 3D: a map of values
%       tfce = sum(extent(h)^E*height^H*dh)      
%
% OUPUT tfce_score is a map of scores(unthresholded)
% -----------------------------
%% check input

if nargin == 1
	E = 0.5;  
	H = 2;
	dh = 0.1;
elseif nargin == 4
	E = varargin{2};
	H = varargin{3};
	dh = varargin{4};
elseif nargin > 4
	error('too many arguments')
end

data = varargin{1};

[x,y,z]=size(data);

clear varargin;

data = (data > 0).*data;

min_value = min(data(:));
max_value = max(data(:));

% define increment size forced by dh
data_range  = range(data(:));
precision = data_range / dh;
if precision > 100 % arbitrary decision to limit precision to 200th of the data range - needed as sometime under H0 one value can be very wrong
	increment = data_range / 100;
else
	increment = data_range / precision;
end

% select a height, obtain cluster map, obtain extent map (=cluster
% map but with extent of cluster rather than number of the cluster)
% then tfce score for that height
num_steps = floor(data_range / increment);
tfce_score = zeros(x,y,z);
for k=0:num_steps-1
    h = min_value + (k*increment);
    binary_map = (data > h);
    CC = bwconncomp(binary_map, 6);
    num_clusters = CC.NumObjects;
    extent_map = zeros(x,y,z); % same as cluster map but contains extent value instead
    for i=1:num_clusters
        indices = CC.PixelIdxList{1,i};
        extent_map(indices) = length(indices);
    end
    tfce_score = tfce_score +((extent_map.^E).*h^H.*increment);
end

