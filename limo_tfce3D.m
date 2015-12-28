function tfce_score = limo_tfce3D(varargin)

% implementation of the Threshold-free cluster enhancement method
% developped for fMRI by Smith & Nichols, NeuroImage 44(2009), 83-98
%
% INPUT tfce_score = limo_tfce(data)
%       tfce_score = limo_tfce(data,channeighbstructmat)
%       tfce_score = limo_tfce(data,channeighbstructmat,E,H,dh)
%
%       data can be either 3D: a map of values 
%       or data can be 4D: a set of value maps computed under H0 
%       type specify if case is observed or H0 data
%       E, H and dh are the parameters of the tfce algorithm defaults are 0.5, 2, 0.1
%       tfce = sum(extent(h)^E*height^H*dh)      
%
% OUPUT tfce_score is a map of scores
%
% Ref 
% Pernet, C., Nichols, T.E., Latinus, M. & Rousselet, G.A.
% Cluster-based computational methods for mass univariate analysis of 
% event-related brain potentials/fields. - in prep
%
% Cyril Pernet 18-10-2011
% -----------------------------
% Copyright (C) LIMO Team 2010


% precision max = 200; % define how many thresholds between min t/F map and
% max t/F map --> needed as sometime under H0 some values can be
% arbritrarily high due to low variance during resampling

%% check input

if nargin == 1 || nargin == 2
	E = 0.5;  
	H = 2;
	dh = 0.1;
elseif nargin == 5
	E = varargin{3};
	H = varargin{4};
	dh = varargin{5};
elseif nargin > 7
	error('too many arguments')
end

data = varargin{1};
try
	channeighbstructmat = varargin{2};
catch
	channeighbstructmat = [];
end

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
		try
			[clustered_map,num] = bwlabel((data > h));
		catch
			[clustered_map, num] = limo_ft_findcluster((data > h), channeighbstructmat,2);
		end
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
		try
			[clustered_map,num] = bwlabel((pos_data > h));
		catch
			[clustered_map, num] = limo_ft_findcluster((pos_data > h), channeighbstructmat,2);
		end
		extent_map = zeros(x,y,z); % same as cluster map but contains extent value instead
		for i=1:num
			idx = clustered_map(:) == i;
			extent_map(idx) = extent_map(idx) + sum(idx); 
		end
		pos_tfce(:,:,z,index) = (extent_map.^E).*h^H.*dh;
		index = index +1;
	end

	hindex = index-1;
	l = length(min(neg_data(:)):increment:max(neg_data(:)))-1;
	neg_increment = (max(neg_data(:)) - min(neg_data(:))) / l;
	neg_tfce = NaN(x,y,z,l); index = 1; 
	for h=min(neg_data(:)):neg_increment:max(neg_data(:))
		try
			[clustered_map,num] = bwlabel((neg_data > h));
		catch
			[clustered_map, num] = limo_ft_findcluster((neg_data > h), channeighbstructmat,2);
		end
		extent_map = zeros(x,y,z); % same as cluster map but contains extent value instead
		for i=1:num
			idx = clustered_map(:) == i;
			extent_map(idx) = extent_map(idx) + sum(idx); 
		end
		neg_tfce(:,:,z,index) = (extent_map.^E).*h^H.*dh;
		index = index +1;
	end

	% compute final score
	tfce_score = nansum(pos_tfce,4)+nansum(neg_tfce,4);    
end