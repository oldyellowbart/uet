clc ;

% Transfer function (Theta_gear(s) / T(s))
num = 1;             % Numerator
den = [J1 0 k1];       % Denominator

% Create transfer function
sys_tf = tf(num, den);

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
TR = [0 5];
X0 = [0;0;0;0;0;0];

k1=8;k2=5;fv=25;d3=2;m=15;J1=8;J2=10;J3=12;r=5;

for T=0:1:5

    [t,y]= ode45(@(t,y)randFun(t,y,T,J1,J2,J3,m,r,fv,k1,k2,d3),TR,X0);
    
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
subplot(2,3,i);legend('1Nm Torque', '2Nm Torque', '3Nm Torque','4Nm Torque', '5Nm Torque')
end


