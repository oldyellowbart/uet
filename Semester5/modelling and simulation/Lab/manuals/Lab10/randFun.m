function dy= randFun(~,y,T,J1,J2,J3,m,r,fv,k1,k2,d3)
dy(1)=y(2);
dy(3)=y(4);
dy(5)=y(6);
dy(2)=1/J1*(T - k1*y(1) + k1*y(3));
dy(4)=1/(J2+m*r*r)*(k1*y(1) - (d3+fv*r^2)*y(4) -(k1+k2*r^2)*y(3) + d3*y(6));
dy(6)=1/J3*(d3*(y(4)-y(6)));
dy=dy';
end

