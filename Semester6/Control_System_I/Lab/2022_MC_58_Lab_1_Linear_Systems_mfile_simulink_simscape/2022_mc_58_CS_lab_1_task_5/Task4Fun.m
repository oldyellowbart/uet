function dy=Task4Fun(t,y,f) 
   m1=1; 
   m2=4; 
   k1=3; 
   k2=2; 
   k3=1; 
   c1=0.03; 
   c2=0.02; 
   c3=0.01; 
   dy(1)=y(2); 
   dy(3)=y(4); 
   dy(2)=1/m1*(f-(k1+k2)*y(1)-(c1+c2)*y(2)+c2*y(4)+k2*y(3)); 
   dy(4)=1/m2*(-(c2+c3)*y(4)-(k2+k3)*y(3)+k2*y(1)+c2*y(2));   
   dy=dy'; 
end 