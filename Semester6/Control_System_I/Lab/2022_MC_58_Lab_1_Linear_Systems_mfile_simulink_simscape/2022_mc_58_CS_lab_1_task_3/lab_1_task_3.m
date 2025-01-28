syms s zrdot 
A=[(mu*s^2+(bs+bt)*s+ks+kt) , -(ks+bs*s) ; -(ks+bs*s) , (ms*s^2+ks+bs*s)]; 
B=[(kt/s+bt)*zrdot;0]; 
C=A\B; 
zs=C(1); 
G=zs/zrdot; 
G=collect(G,s); 
[num,den]=numden(G); 
num=sym2poly(num);den=sym2poly(den); 
num=num/den(1); %dividing by den(1) means dividing by the leading coefficient of denominator i.e., 20 in this case. Done to match the outputs.   
den=den/den(1); 
G=tf(num,den)