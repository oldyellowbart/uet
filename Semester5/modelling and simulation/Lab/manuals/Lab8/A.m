clc;clear; 
TR = [0 50]; 
X0 = [0;0;0;0]; 
m=15;f=10.5;k=24.5;fv2=50;fv1=30.5;r=7.5;J=20; 
for f=1:1:5 
    [t,y]=ode45(@(t,y) func(t,y,m,f,k,fv1,fv2,r,J),TR,X0); 
    x1=y(:,1); 
    v1=y(:,2); 
    x2=y(:,3); 
    v2=y(:,4); 
    a1 = gradient(t,v1);
    a2 = gradient(t,v2);

    subplot(1,6,1) 
    plot(t,x1) 
    hold on 
    xlabel('time') 
    ylabel('Displacement-1')

    subplot(1,6,2) 
    plot(t,v1) 
    hold on 
    xlabel('time') 
    ylabel('Velocity-1')

    subplot(1,6,3) 
    plot(t,x2) 
    hold on 
    xlabel('time') 
    ylabel('theta-2') 

    subplot(1,6,4) 
    plot(t,v2) 
    hold on 
    xlabel('time') 
    ylabel('omega-2')

    subplot(1,6,5) 
    plot(t,a1) 
    hold on 
    xlabel('time') 
    ylabel('Accelaration1')

    subplot(1,6,6) 
    plot(t,a2) 
    hold on 
    xlabel('time') 
    ylabel('Accleration2')
end 

for i=1:4 
subplot(1,6,i);legend('1N force', '2N force', '3N force', '4N force', '5N force') 
end 