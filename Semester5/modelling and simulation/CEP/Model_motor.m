% BLDC Motor Simulation in MATLAB with Fixed Voltage, Number of Poles and Non-Linear Efficiency  

% Motor parameters  
R = 0.090;          % Resistance in ohms  
L = 0.01;       % Inductance in henrys  
Kv = 930;       % Motor velocity constant (RPM/V)  
Ke = Kv / 60;   % Back EMF constant (V/rad/s)  
num_poles = 4;  % Number of poles in the motor  

% Fixed input voltage  
V_input = 12; % in volts  

% Vary current from 0 to 10 A  
current = 0:0.1:10; % Current from 0 to 10 A with 0.1 A steps  
num_points = length(current);  

% Initialize vectors  
rpm = zeros(1, num_points);  
voltage = zeros(1, num_points);  
input_power = zeros(1, num_points);  
output_power = zeros(1, num_points);  
efficiency = zeros(1, num_points);  

% Coefficients for non-linear efficiency model (example values, may need to be adjusted)  
a = 0.9;  % Peak efficiency  
b = 0.4;  % Affects the curve shape  
c = 0.05; % Minimum efficiency  

% Calculate RPM, Voltage, Input Power, Output Power, and Efficiency  
for k = 1:num_points  
    % Calculate back EMF based on current  
    E = Ke * (Kv * current(k)); % Back EMF in volts  
    
    % Calculate RPM using the relationship: RPM = Kv * I  
    rpm(k) = Kv * current(k);  
    
    % Calculate voltage using the formula: V = I*R + E  
    voltage(k) = R * current(k) + E; % Recalculate voltage  
    
    % Calculate input power (P_input = V * I)  
    input_power(k) = V_input * current(k);  
    
    % Calculate output power (P_output = Torque * Omega)  
    % Assuming a constant torque constant (Kt) for simplicity  
    Kt = 0.1; % Torque constant (Nm/A), adjust as needed  
    output_power(k) = Kt * current(k) * (rpm(k) * (2 * pi / 60)); % P_out = Torque * Omega  
    
    % Non-linear efficiency model  
    % Efficiency = a - b*(current)^2 - c  
    efficiency(k) = (a - b * (current(k)^2) - c);  
    
    % Ensure efficiency is never negative  
    efficiency(k) = max(efficiency(k), 0);  
    
    % Adjust efficiency based on output and input power  
    if input_power(k) > 0  
        efficiency(k) = min(efficiency(k), output_power(k) / input_power(k)); % Cap efficiency based on power  
    else  
        efficiency(k) = 0; % Avoid division by zero  
    end  
    
end  

% Plotting results  
figure;  

% Current vs RPM  
subplot(3,1,1);  
plot(current, rpm, 'LineWidth', 2);  
title('Current vs RPM');  
xlabel('Current (A)');  
ylabel('RPM');  
grid on;  

% Voltage vs RPM  
subplot(3,1,2);  
plot(voltage, rpm, 'LineWidth', 2);  
title('Voltage vs RPM');  
xlabel('Voltage (V)');  
ylabel('RPM');  
grid on;  

% Efficiency vs Current  
subplot(3,1,3);  
plot(current, efficiency * 100, 'LineWidth', 2); % Convert to percentage  
title('Efficiency vs Current');  
xlabel('Current (A)');  
ylabel('Efficiency (%)');  
grid on;  

% Enhance figure  
sgtitle('BLDC Motor Characteristics with Fixed Voltage and Non-Linear Efficiency');