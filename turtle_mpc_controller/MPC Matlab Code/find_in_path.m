function [row] = find_in_path(x,y,path)
%FIND_IN_PATH Summary of this function goes here
%   Detailed explanation goes here
if y > 295
    left = 1;
    right = length(path);
    while left < right
        mid = idivide(int32(left+right),2,'floor');
        if path(mid,1) < x
            left = mid + 1;
        else
            right = mid;
        end
    end
    row = right;
else
left = 1;
right = length(path);
while left < right
    mid = idivide(int32(left+right),2,'floor');
    if path(mid,2) < y
        left = mid + 1;
    else
        right = mid;
    end
end
row = right;
end
% left = 1;
% right = length(path);
% while left < right
%     mid = idivide(int32(left+right),2,'floor');
%     if path(mid,1) < x
%         left = mid + 1;
%     else
%         right = mid;
%     end
% end
% rowx = right;
% 
% if path(rowy,1) >= x && path(rowy,2) >= y
%     row = rowy;
% else
%     row = rowx;
% end
end

