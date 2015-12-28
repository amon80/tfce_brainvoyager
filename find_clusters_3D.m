function [clustered_map, num_cluster] = find_clusters_3D(data)
%FIND_CLUSTERS_3D Finds cluster in a 3D binary image.
%
% INPUT A binary 3D image 
%
% OUPUT clustered_map is a the clustered image.
%		num_cluster is the number of cluster found.
%

[x,y,z] = size(data);

clustered_map = data;
current_label = 3;
num_cluster = 0;
q = CQueue();
f = fopen('test.txt', 'w');

for k=1:1:z
	for j=1:1:y
		for i=1:1:x
			if clustered_map(i,j,k) == 1
                num_cluster = num_cluster + 1;
                clustered_map(i,j,k) = current_label;
				q.push([i,j,k]);
                fprintf(f, 'Adding %d %d %d\n',i,j,k);                
				while ~q.isempty()
					p = q.pop();
					[beginx, beginy, beginz, endx, endy, endz] = find_borders(x,y,z,p(1),p(2),p(3));
					for k1=beginz:1:endz
						for j1=beginy:1:endy
							for i1=beginx:1:endx
								fprintf(f, 'checking %d %d %d, value: %d\n',i1,j1,k1, clustered_map(i1,j1,k1));
								if clustered_map(i1,j1,k1) == 1
                                    clustered_map(i1,j1,k1) = current_label;
                                    fprintf(f, 'checking %d %d %d, now value should be: %d\n',i1,j1,k1, clustered_map(i1,j1,k1));
									q.push([i1,j1,k1]);
                                    fprintf(f, 'Adding %d %d %d\n',i1,j1,k1);
								end
							end
						end
					end
				end
				current_label = current_label + 1;
			end
		end
	end
end

for k=1:1:z
	for j=1:1:y
		for i=1:1:x
			if clustered_map(i,j,k) ~= 0
				clustered_map(i,j,k) = clustered_map(i,j,k) - 1;
			end
		end
	end
end

fclose(f);

end

