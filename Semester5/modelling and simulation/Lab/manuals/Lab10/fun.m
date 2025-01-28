function dy= fun(~,y) 
   m1=1; 
   m2=4; 
   k1=4; 
   k2=5; 
   k3=0;
   f=5;
   fv1=3; 
   fv2=3; 
   fv3=2; 
   dy(1)=y(2); 
   dy(3)=y(4); 
   dy(2)=1/m1*(f-(k1+k2)*y(1)-(fv1+fv3)*y(2)+fv3*y(4)+k2*y(3)); 
   dy(4)=1/m2*(-(fv2+fv3)*y(4)-(k2+k3)*y(3)+k2*y(1)+fv3*y(2));   
   dy=dy'; 
end 