function [clustered_map, num_cluster] = find_clusters_3D(data)
%FIND_CLUSTERS_3D Finds cluster in a 3D binary image.
%
% INPUT A binary 3D image 
%
% OUPUT clustered_map is a the clustered image.
%		num_cluster is the number og cluster found.
%

[x,y,z] = size(data);

clustered_map = data;
current_label == 2;
q = CQueue();

for i=0:1:x
	for j=0:1:y
		for k=0:1:z
			if clustered_map(i,j,k) == 1
				q.push([i,j,k]);
				clustered_map(i,j,k) = current_label;
				while ~q.isempty()
					[x1,y1,z1] = q.pop();
					[beginx, beginy, beginz, endx, endy, endz] = find_borders(x,y,z,x1,y1,z1);
					for i1=beginx:1:endx
						for j1=beginy:1:endy
							for k1=beginz:1:endz
								if clustered_map(i1,j1,k1) == 1
									q.push([i1,j1,k1]);
									clustered_map(i,j,k) = current_label;
								end
							end
						end
1					end
				end
				current_label = current_label + 1;
			end
		end
	end
end

end

