clc;clear;
TR = [0 10];
X0 = [0;0];
 K=2;
 G = 1.5;
 B = 2;
 m = 2;
 A = 10;
 P = 23;
for P=1:1:5
[t,x] = ode45(@(t,x) var(t,x,P,m,A,B,K),TR,X0)
disp1 = x(:,1)
velocity1 = x(:,2)
accel = (P*A - B*velocity1 - K*disp1)/m

subplot(1,3,1);
hold on;
plot(t,disp1)
subplot(1,3,2);
hold on;

plot(t,velocity1)
subplot(1,3,3);
hold on;

plot(t,accel)


end
function dx= var(t,x,P,m,A,B,K)

dx(1)=x(2);
dx(2) = (P*A - B*x(2) - K*x(1))/m
 dx=dx';
end

