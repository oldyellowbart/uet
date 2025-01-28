function dy= randFun(t,y,m,f,k,fv1,fv2,r,J) 
   if f==0 
       f=5*sin(5*t); 
   end 
   dy(1)=y(2); 
   dy(3)=y(4); 
   dy(2)=1/m*(f - fv2*y(2) - fv1*y(2) - k*y(1) + r*fv1*y(4)); 
   dy(4)=1/J*(r*fv1*y(2) - r*r*fv1*y(4));   
   dy=dy'; 
end 