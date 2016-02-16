function saveinline(vmp,name)

[x,y,z]=size(vmp);
filename = strcat(name,'.txt');
fileID = fopen(filename,'w');
for i =1:1:x
    for j=1:1:y
        for k=1:1:z
            
            fprintf(fileID,'%f ',vmp(i,j,k));
            
        end
    end
end
fprintf(fileID,'\n%d\n',x);
fprintf(fileID,'%d\n',y);
fprintf(fileID,'%d\n',z);
fclose(fileID);
end