% Define the maximum current and temperature threshold
max_current = 10; % in Amperes
max_throttle = 100; % in percentage
temp_threshold = 50; % in degrees Celsius, threshold temperature
max_temp = 100; % in degrees Celsius, maximum temperature

% Motor parameters
R  = 0.090;          % Resistance in ohms
L  = 0.0;       % Inductance in henrys
Kv = 930;       % Motor velocity constant (RPM/V)
Ke = Kv / 60;   % Back EMF constant (V/rad/s) FOR 1 DEGREE APROX
num_poles = 12;  % Number of poles in the motor

% Fixed input voltage
V_input = 12; % in volts

% Coefficients for non-linear efficiency model (example values, may need to be adjusted)
a = 0.9;  % Peak efficiency
b = 0.4;  % Affects the curve shape
c = 0.05; % Minimum efficiency

% Function to calculate current based on throttle and temperature
function current = esc_current(throttle, temperature, max_current, max_throttle, temp_threshold, max_temp)
    % Linear relationship between throttle and current
    current = (max_current / max_throttle) * throttle;
    
    % Adjust current based on temperature
    if temperature > temp_threshold
        % Decrease current linearly as temperature rises above threshold
        temp_factor = (max_temp - temperature) / (max_temp - temp_threshold);
        current = current * temp_factor;
    end
    
    % Ensure current is not negative
    current = max(current, 0);
end

% Function to calculate RPM based on current and voltage
function rpm = calculate_rpm(current, Kv)
            rpm = Kv * current;
end

% Function to calculate voltage based on current and back EMF
function voltage = calculate_voltage(current, R, E)
    voltage = (R  + E)* current;
end

% Function to calculate efficiency based on current
function efficiency = calculate_efficiency(current, a, b, c)
    efficiency = (a - b * (current^2) - c);
    efficiency = max(efficiency, 0); % Ensure efficiency is never negative
end

% Generate data for plotting
throttle_values     = linspace(0, 100, 500);
temperature_values  = linspace(0, 100, 500);
current_values      = linspace(0, 10, 500);
voltage_values      = linspace(5.6, 15, 500);

% Initialize vectors for results
rpm_vs_throttle       = zeros(1, length(throttle_values));
rpm_vs_temperature    = zeros(1, length(temperature_values));
rpm_vs_voltage        = zeros(1, length(voltage_values));
efficiency_vs_current = zeros(1, length(current_values));

% Calculate RPM and Voltage for varying throttle at a fixed temperature (e.g., 50°C)
for i = 1:length(throttle_values)
    throttle = throttle_values(i);
    temperature = 50;
    current = esc_current(throttle, temperature, max_current, max_throttle, temp_threshold, max_temp);
    E = Ke * (Kv * current); % Back EMF in volts
    rpm_vs_throttle(i) = calculate_rpm(current, Kv);
end

% Calculate RPM and Voltage for varying temperature at a fixed throttle (e.g., 50%)
for i = 1:length(temperature_values)
    throttle = 50;
    temperature = temperature_values(i);
    current = esc_current(throttle, temperature, max_current, max_throttle, temp_threshold, max_temp);
    rpm_vs_temperature(i) = calculate_rpm(current, Kv);
end

% Calculate RPM for varying voltage at a fixed throttle (e.g., 50%) and fixed temperature (e.g., 50°C)
for i = 1:length(voltage_values)
    voltage_input = voltage_values(i);
    throttle = 50;
    temperature = 50;
    current = esc_current(throttle, temperature, max_current, max_throttle, temp_threshold, max_temp);
    E = Ke * (Kv * current); % Back EMF in volts
    rpm_vs_voltage(i) = calculate_rpm(current * voltage_input / V_input , Kv);
end

% Calculate Efficiency for varying current
for i = 1:length(current_values)
    current = current_values(i);
    efficiency_vs_current(i) = calculate_efficiency(current, a, b, c);
end

% Plotting results

figure;

% Current vs RPM (Throttle effect)
subplot(3,2,1);
plot(throttle_values, rpm_vs_throttle, 'LineWidth', 2);
title('Throttle vs RPM');
xlabel('Throttle (%)');
ylabel('RPM');
grid on;

% Temperature vs RPM (Temperature effect)
subplot(3,2,2);
plot(temperature_values, rpm_vs_temperature, 'LineWidth', 2);
title('Temperature vs RPM');
xlabel('Temperature (°C)');
ylabel('RPM');
grid on;

% Voltage vs RPM (Voltage effect)
subplot(3,2,[3 4]);
plot(voltage_values, rpm_vs_voltage, 'LineWidth', 2);
title('Voltage vs RPM');
xlabel('Voltage (V)');
ylabel('RPM');
grid on;

% Current vs Efficiency
subplot(3,2,[5 6]);
plot(current_values, efficiency_vs_current * 100, 'LineWidth', 2); % Convert to percentage
title('Current vs Efficiency');
xlabel('Current (A)');
ylabel('Efficiency (%)');
grid on;

sgtitle('BLDC Motor Characteristics with Throttle and Temperature Effects');