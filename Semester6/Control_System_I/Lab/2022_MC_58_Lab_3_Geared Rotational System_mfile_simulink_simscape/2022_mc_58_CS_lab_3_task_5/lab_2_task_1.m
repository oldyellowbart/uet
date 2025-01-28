% For constant tourque 5Nm 
clc 
clear 
TR = [0 5]; % time RANGE 
X0 = [0;0;0;0];%initial conditions 
[t,z] = ode45(@func1, TR, X0);%calling thr ide solver to solve by function 
%storing given array as vectors 
theta1 = z(:, 1); 
AngVel1 = z(:, 2); 
theta2 = z(:, 3);  
AngVel2 = z(:, 4);  
  
%plotting the angular displacements and velocities 
acc1 = diff(AngVel1); 
acc2= diff(AngVel2); 
plot(t,theta1,t,AngVel1,t,[0;acc1],t,theta2,t,AngVel2,t,[0;acc2]); 
xlabel('time') 
legend('Angular Displacement 1','Angular Velocity 1','Angulara acceleration 1','Angular Displacement 2','Angular Velocity 2','Angulara acceleration 2')  
ylabel('position & Velocity') 
title("m-file") 
%function containing the differentialequations 
function dx = func1(~, x) 
% Values of Coefficients 
J1=1; J2=10; D1=0.9; D2=0.02; k=3;T=5;  
  
% State Equations 
dx(1) = x(2); 
dx(3) = x(4); 
dx(2) = (T-D1*x(2)-k*x(1)+k*x(3))/J1; 
dx(4) = (-k*x(3)-D2*x(4)+k*x(1))/J2; 
dx = dx'; 
end