function [beginx, beginy, beginz, endx, endy, endz] = find_borders(x,y,z, x1, y1, z1)

if x1 - 1 < 1
	beginx = x1;
else
	beginx = x1 - 1;
end
if y1 - 1 < 1
	beginy = y1;
else
	beginy = y1 - 1;
end
if z1 - 1 < 1
	beginz = z1;
else
	beginz = z1 - 1;
end

if x1 + 1 > x
	endx = x1;
else
	endx = x1 + 1;
end
if y1 + 1 > y
	endy = y1;
else
	endy = y1 + 1;
end

if z1 + 1 > z
	endz = z1;
else
	endz = z1 + 1;
end
 

end