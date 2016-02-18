function saveinline(vmp,name,addNewLines)

[x,y,z]=size(vmp);
filename = strcat(name,'.txt');
fileID = fopen(filename,'w');

linear_vmp = vmp(:);
linear_vmp_size = length(linear_vmp);
linear_vmp_size = x*y*z;
for i =1:linear_vmp_size;            
    fprintf(fileID,'%f ',linear_vmp(i));
    if addNewLines
        fprintf(fileID, '\n');
    end
end
if ~addNewLines
    fprintf(fileID, '\n');
end
fprintf(fileID,'%d\n',x);
fprintf(fileID,'%d\n',y);
fprintf(fileID,'%d',z);
fclose(fileID);
end