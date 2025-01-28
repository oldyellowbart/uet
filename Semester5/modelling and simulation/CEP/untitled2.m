


% Define the simulation time
t = 0:0.001:10; % from 0 to 1 second with increments of 0.001

% Define the motor parameters
J = 0.01; % moment of inertia of the rotor
b = 0.1; % motor viscous friction constant
Ke = 0.01; % electromotive force constant
Kt = 0.01; % motor torque constant
R = 1; % electric resistance
L = 0.5; % electric inductance

% Define the ESC parameters
Vdc = 10; % DC supply voltage
duty_cycle = 0.5:0.01:1; % initial duty cycle

% Initialize variables
omega = zeros(size(t)); % motor speed

i = zeros(size(t)); % motor current
theta = 0; % initial rotor position

% Simulation loop
for k = 2:length(t)
    duty_cycle = 0.5 + ( t(k) / max(t)); % linear variation from 0.5 to 5
    % ESC control logic (simple proportional control for demonstration)
    error = 100 - omega(k-1); % desired speed is 100 rad/s
    duty_cycle = duty_cycle + 0.01 * error; % update duty cycle
    duty_cycle = min(max(duty_cycle, 0), 1); % limit duty cycle between 0 and 1
    
    % Calculate the applied voltage
    V = duty_cycle * Vdc;
    
    % Motor equations
    dtheta = omega(k-1) * (t(k) - t(k-1));
    theta = theta + dtheta;
    e = Ke * omega(k-1); % back EMF
    di = (V - e - R * i(k-1)) / L * (t(k) - t(k-1));
    i(k) = i(k-1) + di;
    torque = Kt * i(k);
    domega = (torque - b * omega(k-1)) / J * (t(k) - t(k-1));
    omega(k) = omega(k-1) + domega;
    omega_rpm = omega * (60 / (2 * pi));
end

% Plot the results
figure;

% Plot the motor speed
subplot(2, 1, 1);
plot(t, omega_rpm);
title('Motor Speed');
xlabel('Time (seconds)');
ylabel('Speed (rad/s)');

% Plot the motor current
subplot(2, 1, 2);
plot(t, i);
title('Motor Current');
xlabel('Time (seconds)');
ylabel('Current (A)');

