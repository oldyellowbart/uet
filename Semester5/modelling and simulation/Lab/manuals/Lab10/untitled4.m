% Define the system parameters
m = 1; % Mass
B = 1; % Damping coefficient
k = 1; % Spring constant
GA = 1; % Gain

% Define the system of differential equations
function dz = stateSpaceODE(t, z)
    % State variables
    m = 1; % Mass
B = 1; % Damping coefficient
k = 1; % Spring constant
GA = 1; % Gain
    x1 = z(1);
    x2 = z(2);
    
    % Differential equations
    dx1 = x2;
    dx2 = (GA - B*x2 - k*x1) / m;
    
    % Return the derivatives as a column vector
    dz = [dx1; dx2];
end

% Initial conditions
z0 = [0; 0]; % Initial state [x1(0); x2(0)]

% Time span for the simulation
tspan = [0 10];

% Solve the system using ode45
[t, z] = ode45(@stateSpaceODE, tspan, z0);

% Extract the state variables
x1 = z(:, 1);
x2 = z(:, 2);

% Plot the results
figure;
subplot(2, 1, 1);
plot(t, x1);
title('State Variable x1 (Displacement)');
xlabel('Time (s)');
ylabel('x1');

subplot(2, 1, 2);
plot(t, x2);
title('State Variable x2 (Velocity)');
xlabel('Time (s)');
ylabel('x2');