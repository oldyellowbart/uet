X0=[0;0];%initial condiitons are zero 
TR=[0 10];%time response RANGE 
%t=0:0.1:50; 
[t,x]=ode45(@func1,TR,X0); 
Displacement=x(:,1) 
Velocity=x(:,2) 
plot(t, Displacement) 
hold on 
plot(t,Velocity) 
acceleration = gradient(Velocity,t); 
hold on 
plot(t,acceleration) 
ylabel('x,v,a') 
xlabel('time') 
legend("displacement","velocity","acceleration"); 
  
%state variables give the following function 
function dx = func1(t,x) 
    M=10;B=5;K=5;F=5; 
    dx(1)=x(2)%for x dot 
    dx(2)=(F-B*x(2)-K*x(1))/M    %for x dot dot 
    dx = dx'; 
end 