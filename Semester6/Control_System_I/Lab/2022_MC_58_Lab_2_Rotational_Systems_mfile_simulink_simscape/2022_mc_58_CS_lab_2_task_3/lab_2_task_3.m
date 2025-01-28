clc; 
TR=0:0.01:10; 
x0=[0;0;0;0]; 
[t,x]=ode45(@Task3Fun,TR,x0); 
th1=x(:,1); 
om1=x(:,2); 
alpha1=gradient(om1)./gradient(t); 
th2=x(:,3); 
om2=x(:,4); 
alpha2=gradient(om2)./gradient(t); 
  
subplot(2,3,1); 
plot(t,th1);xlabel('time');ylabel('theta 1'); 
subplot(2,3,2); 
plot(t,om1);xlabel('time');ylabel('omega 1'); 
subplot(2,3,3); 
plot(t,alpha1);xlabel('time');ylabel('alpha 1') 
subplot(2,3,4); 
plot(t,th2);xlabel('time');ylabel('theta 2'); 
subplot(2,3,5); 
plot(t,om2);xlabel('time');ylabel('omega 2'); 
subplot(2,3,6); 
plot(t,alpha2);xlabel('time');ylabel('alpha 2') 
%%%%%%%%%%%%%%%%%%%% 
function dy=Task3Fun(~,y) 
    T=1; 
    dy(1)=y(2); 
    dy(2)=1/5*(y(4) + 9*y(3) - 9*y(2) - 9*y(1)); 
    dy(3)=y(4); 
    dy(4)=1/3*(T+y(2) + 9*y(1) - y(4) - 12*y(3)); 
    dy=dy'; 
end 