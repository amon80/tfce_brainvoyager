clear;
clc;
vmp = xff('*.vmp');
map = vmp.Map(1).VMPData;
tfce = zeros(size(map));

H = 2; 
E = 0.5;

for x=1:1:size(map,1)

	for y=1:1:size(map,2)

		for z=1:1:size(map,3)

			line = squeeze(map(:,y,z));
			%plot(linexy);
			hp = line(x);
            if (hp<=0) 
                continue;
            end;
			dh = 0.1;
            %h = 0:dh:hp;
            h = min(line):dh:hp;
            val = 0; oldval = 0; value = 0;
			for ih=1:1:size(h,2)
				fh1 = line>h(ih);
				i=0; eh = 0;
				while x-i>0 && fh1(x-i),
				    i=i+1;
				end
				eh = eh + i -1;
				i=0;
				while x+i<numel(fh1) && fh1(x+i),
				    i=i+1;
				end
				eh = eh + i -1;
                
                oldval = val;
                val = (eh^E)*(h(ih)^H);
                
                value = value + ((oldval + val)/2)*dh;
			end;
            tfce(x,y,z) = tfce(x,y,z) + val/3;
		end;
	end;
end;


for x=1:1:size(map,1)

	for y=1:1:size(map,2)

		for z=1:1:size(map,3)

			line = squeeze(map(x,:,z));
			%plot(linexy);
			hp = line(y);
            if (hp<=0) 
                continue;
            end;
			dh = 0.1;
            %h = 0:dh:hp;
            h = 0:dh:hp;
            val = 0; oldval = 0; value = 0;
			for ih=1:1:size(h,2)
				fh1 = line>h(ih);
				i=0; eh = 0;
				while y-i>0 && fh1(y-i),
				    i=i+1;
				end
				eh = eh + i -1;
				i=0;
				while y+i<numel(fh1) && fh1(y+i),
				    i=i+1;
				end
				eh = eh + i -1;
                
                oldval = val;
                val = (eh^E)*(h(ih)^H);
                
                value = value + ((oldval + val)/2)*dh;
			end;
            tfce(x,y,z) = tfce(x,y,z) + val/3;
		end;
	end;
end;


for x=1:1:size(map,1)

	for y=1:1:size(map,2)
        
        line = squeeze(map(x,y,:));
		
        for z=1:1:size(map,3)

			%plot(linexy);
			hp = line(z);
            if (hp<=0) 
                continue;
            end;
			dh = 0.1;
            %h = 0:dh:hp;
            h = 0:dh:hp;
            val = 0; oldval = 0; value = 0;
			for ih=1:1:size(h,2)
				fh1 = line>h(ih);
				i=0; eh = 0;
				while z-i>0 && fh1(z-i),
				    i=i+1;
				end
				eh = eh + i -1;
				i=0;
				while z+i<numel(fh1) && fh1(z+i),
				    i=i+1;
				end
				eh = eh + i -1;
                
                oldval = val;
                val = (eh^E)*(h(ih)^H);
                
                value = value + ((oldval + val)/2)*dh;
			end;
            tfce(x,y,z) = tfce(x,y,z) + val/3;
        end;
	end;
end;