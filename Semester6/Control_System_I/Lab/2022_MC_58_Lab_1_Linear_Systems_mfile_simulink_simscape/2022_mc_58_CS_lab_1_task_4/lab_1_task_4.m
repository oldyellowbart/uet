clc; 
TR=0:0.01:10; 
x0=[0;0;0;0;0]; 
[t,x]=ode45(@Task2Fun,TR,x0); 
x1=x(:,1); 
x1_dot=x(:,2); 
x1_ddot=gradient(x1_dot)./gradient(t); 
x2=x(:,3); 
x2_dot=x(:,4); 
x2_ddot=gradient(x2_dot)./gradient(t); 
  
subplot(2,3,1); 
plot(t,x1);xlabel('time');ylabel('x1'); 
subplot(2,3,2); 
plot(t,x1_dot);xlabel('time');ylabel('x1 dot'); 
subplot(2,3,3); 
plot(t,x1_ddot);xlabel('time');ylabel('x1 double dot') 
subplot(2,3,4); 
plot(t,x2);xlabel('time');ylabel('x2'); 
subplot(2,3,5); 
plot(t,x2_dot);xlabel('time');ylabel('x2 dot'); 
subplot(2,3,6); 
plot(t,x2_ddot);xlabel('time');ylabel('x2 double dot') 
%%%%%%%%%%%%%%%%%%%% 
function dy=Task2Fun(t,y) 
    f=1; 
    dy(1)=y(2); 
    dy(2)=1/4*(f - 8*y(2) - 5*y(1) + 8*y(4) + 5*y(5)); 
    dy(3)=y(4); 
    dy(5)=1/4*(5*y(1) + 4*y(4) - 5*y(5)); 
    dy(4)=1/4*(8*y(2) + 4*dy(5) - 16*y(4)); 
    dy=dy'; 
end 