clc;clear; 
TR = [0 100]; 
X0 = [0;0;0;0]; 
m=15;f=10.5;k=24.5;fv2=50;fv1=30.5;r=7.5;J=20; 
K=[]; 
x=[];
v=[];
th=[];
om=[];
acc1=[];
acc2=[];
for k=10:1:20 
    K(end+1)=k; 
    [t,y]=ode45(@(t,y) func(t,y,m,f,k,fv1,fv2,r,J),TR,X0); 
    x1=y(:,1); 
    v1=y(:,2); 
    x2=y(:,3); 
    v2=y(:,4); 
    a1 = gradient(t,v1);
    a2 = gradient(t,v2);
    x(end+1)=max(x1);
    v(end+1)=max(v1); 
    th(end+1)=max(x2);
    om(end+1)=max(v2);
    acc1(end+1)=max(a1);
    acc2(end+1)=max(a2);
end
subplot(1,6,1) 
plot(K,x) 
xlabel('stiffness') 
ylabel('Displacement-1') 
subplot(1,6,2) 
plot(K,v) 
xlabel('stiffness') 
ylabel('Velocity-1') 
subplot(1,6,3) 
plot(K,th) 
xlabel('stiffness') 
ylabel('theta-2') 
subplot(1,6,4) 
plot(K,om) 
xlabel('stiffness') 
ylabel('omega-2') 
subplot(1,6,5) 
plot(K,acc1) 
xlabel('stiffness') 
ylabel('Acc-1') 
subplot(1,6,6) 
plot(K,acc2) 
xlabel('stiffness') 
ylabel('acc-2') 