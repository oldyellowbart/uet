syms s T 	 
A=[(s^2+2*s) , -2*s , 0 ; -2*s , (2*s+3) , -3 ; 0 , -3 , (3+s)]; B=[3*T ; 0 ; 0]; 
C=A\B; 	 theta4=C(3); 
thetaL=5*theta4; 	 
G=thetaL/T; 
G=collect(G,s); 
[num,den]=numden(G); 	 
num=sym2poly(num);
den=sym2poly(den);  
num=num/den(1);     %dividing by den(1) means dividing by the leading coefficient of denominator. Done to match the outputs 
den=den/den(1); 	 
fprintf('ThetaL(s)/T(s):') 
tf(num,den) 
