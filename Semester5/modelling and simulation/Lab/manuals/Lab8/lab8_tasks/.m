clc ;

% Transfer function (Theta_gear(s) / T(s))
num = [1];             % Numerator
den = [J1 0 k1];       % Denominator

% Create transfer function
sys_tf = tf(num, den)

% State-space matrices
A = [0 1 0 0 0 0;
    -k1/J1 0 k1/J1 0 0 0;
    0 0 0 1 0 0;
    k1/(J2+m*r^2) 0 -(k1+k2*r^2)/(J2+m*r^2) -(fv*r^2+d3)/(J2+m*r^2) 0 d3/(J2+m*r^2);
    0 0 0 0 0 1;
    0 0 0 d3/J3 0 -d3/J3];

B = [0; 1/J1; 0; 0; 0; 0];
C = [1 0 0 0 0 0];
D = 0;

% Create the state-space system
sys_ss = ss(A, B, C, D);

%% Part A Variant Torque

clc;clear;
TR = [0 50];
X0 = [0;0;0;0;0;0];

k1=50;k2=50;fv=50;d3=30.5;T=40;m=15;J1=20;J2=10;J3=16;r=7.5;

for T=10:5:30

    [t,y]= ode45(@(t,y)randFun2(t,y,T,J1,J2,J3,m,r,fv,k1,k2,d3),TR,X0);
    
    thGear=y(:,3);
    omGear=y(:,4);
   
    x=r*thGear;
    v=r*omGear;
    
    % Compute acceleration
    a1 = gradient(v, t);
    a2 = gradient(omGear, t);

    subplot(2,3,4)
    plot(t,thGear)
    hold on
    xlabel('time')
    ylabel('theta-Gear')
   
    subplot(2,3,5)
    plot(t,omGear)
    hold on
    xlabel('time')
    ylabel('Angular velocity-Gear')
    
    subplot(2,3,1)
    plot(t,x)
    hold on
    xlabel('time')
    ylabel('displacement')
    
    subplot(2,3,2)
    plot(t,v)
    hold on
    xlabel('time')
    ylabel('velocity')

    subplot(2,3,3)
    plot(t, a1)
    hold on
    xlabel('time')
    ylabel('Acceleration-1')

    subplot(2,3,6)
    plot(t, a2)
    hold on
    xlabel('time')
    ylabel('Angular acceleration-Gear')

end

for i=1:6
subplot(2,3,i);legend('10N Torque', '15N Torque', '20N Torque','25N Torque', '30N Torque')
end


%%  Part B Variant Radius   

clc;
clear;
TR = [0 50];
X0 = [0;0;0;0;0;0];
k1=50;k2=50;fv=50;d3=30.5;T=40;m=15;J1=20;J2=10;J3=16;r=7.5;
for r=1:1:8
    [t,y]=ode45(@(t,y)randFun2(t,y,T,J1,J2,J3,m,r,fv,k1,k2,d3),TR,X0);
    
    thGear=y(:,3);
    omGear=y(:,4);
    x=r*thGear;
    v=r*omGear;
    
    % Compute acceleration
    a1 = gradient(v, t);
    a2 = gradient(omGear, t);

    subplot(2,3,4)
    plot(t,thGear)
    hold on
    xlabel('time')
    ylabel('theta-Gear')
   
    subplot(2,3,5)
    plot(t,omGear)
    hold on
    xlabel('time')
    ylabel('Angular velocity-Gear')
    
    subplot(2,3,1)
    plot(t,x)
    hold on
    xlabel('time')
    ylabel('displacement')
    
    subplot(2,3,2)
    plot(t,v)
    hold on
    xlabel('time')
    ylabel('velocity')

    subplot(2,3,3)
    plot(t, a1)
    hold on
    xlabel('time')
    ylabel('Acceleration-1')

    subplot(2,3,6)
    plot(t, a2)
    hold on
    xlabel('time')
    ylabel('Angular acceleration-Gear')

end
for i=1:6
subplot(2,3,i);legend('1m radius', '2m radius', '3m radius','4m radius', '5m radius', '6m radius', '7m radius', '8mradius')
end


function dy= randFun2(t,y,T,J1,J2,J3,m,r,fv,k1,k2,d3)
dy(1)=y(2);
dy(3)=y(4);
dy(5)=y(6);
dy(2)=1/J1*(T - k1*y(1) + k1*y(3));
dy(4)=1/(J2+m*r*r)*(k1*y(1) - (d3+fv*r^2)*y(4) -(k1+k2*r^2)*y(3) + d3*y(6));
dy(6)=1/J3*(d3*(y(4)-y(6)));
dy=dy';
end