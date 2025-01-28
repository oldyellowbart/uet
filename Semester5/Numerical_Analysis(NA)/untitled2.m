
% Example (3.1) Page 71 
 4x – 2y + 6z = 8 
 2x + + 8y 2z = 4 
 6x + + 10y 3z = 0 
by solving AX = B Let C = inv (A) 
so X = A\B or X = B / A or X = C B 
 A = [4 -2 6; 2 8 2; 6 10 3]; 
 B = [8; 4; 0]; 
>> X = inv(A) * B % execute 
