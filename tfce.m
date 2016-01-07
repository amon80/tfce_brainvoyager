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

% define increment size forced by dh
data_range  = range(data(:));
precision = round(data_range / dh);
if precision > 200 % arbitrary decision to limit precision to 200th of the data range - needed as sometime under H0 one value can be very wrong
	increment = data_range / 200;
else
	increment = data_range / precision;
end

% check negative values if so do negate and add scores
if min(data(:)) > 0
	% select a height, obtain cluster map, obtain extent map (=cluster
	% map but with extent of cluster rather than number of the cluster)
	% then tfce score for that height
	index = 1; 
	tfce = NaN(x,y,z,length(min(data(:)):increment:max(data(:))));
	for h=min(data(:)):increment:max(data(:))
		[clustered_map, num] = find_clusters_3D((data > h));
		extent_map = zeros(x,y,z); % same as cluster map but contains extent value instead
		for i=1:num
			idx = clustered_map(:) == i;
			extent_map(idx) = extent_map(idx) + sum(idx); 
		end
		tfce(:,:,:,index) = (extent_map.^E).*h^H.*dh;
		index = index +1;
	end
	% compute final score
	tfce_score = nansum(tfce,4);   
else
	pos_data = (data > 0).*data;
	neg_data = abs((data < 0).*data);

	clear data;

	% select a height, obtain cluster map, obtain extent map
	% then tfce score for that height
	l = length(min(pos_data(:)):increment:max(pos_data(:)));
	pos_increment = (max(pos_data(:)) - min(pos_data(:))) / l;
	pos_tfce = NaN(x,y,z,l); 
    index = 1; 
	for h=min(pos_data(:)):pos_increment:max(pos_data(:))
		[clustered_map, num] = find_clusters_3D((pos_data > h));
		extent_map = zeros(x,y,z); % same as cluster map but contains extent value instead
		for i=1:num
			idx = clustered_map(:) == i;
			extent_map(idx) = extent_map(idx) + sum(idx); 
		end
		pos_tfce(:,:,:,index) = (extent_map.^E).*h^H.*dh;
		index = index +1;
	end

	l = length(min(neg_data(:)):increment:max(neg_data(:)))-1;
	neg_increment = (max(neg_data(:)) - min(neg_data(:))) / l;
	neg_tfce = NaN(x,y,z,l);
    index = 1; 
	for h=min(neg_data(:)):neg_increment:max(neg_data(:))
		[clustered_map, num] = find_clusters_3D((neg_data > h));
		extent_map = zeros(x,y,z); % same as cluster map but contains extent value instead
		for i=1:num
			idx = clustered_map(:) == i;
			extent_map(idx) = extent_map(idx) + sum(idx); 
		end
		neg_tfce(:,:,:,index) = (extent_map.^E).*h^H.*dh;
		index = index +1;
	end

	% compute final score
	tfce_score = nansum(pos_tfce,4)+nansum(neg_tfce,4);    
end