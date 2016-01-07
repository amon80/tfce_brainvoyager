function  perm_matrix = perm(matrix)
% implementation of 3D matrix permutation
%
% INPUT matrix = a 3D matrix
%
% OUPUT perm_matrix = a permutation of the 3D matrix in input.
% -----------------------------

[x,y,z] = size(matrix);
perm_matrix = matrix;

%for tests
%rng(1);

%for real use 
rng('shuffle');

for i=1:x
    for j=1:y
        for k=1:z
            rx = 1 + (x-1)*rand(); 
            ry = 1 + (y-1)*rand();
            rz = 1 + (z-1)*rand();
            
            tmp = perm_matrix(rx,ry,rz);
            perm_matrix(rx,ry,rz) = perm_matrix(i,j,k);
            perm_matrix(i,j,k) = tmp;
        end
    end
end

end
