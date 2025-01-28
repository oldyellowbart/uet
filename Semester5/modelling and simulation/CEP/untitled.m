

% Define the maximum current and temperature threshold
max_current = 10; % in Amperes
max_throttle = 100; % in percentage
temp_threshold = 75; % in degrees Celsius, threshold temperature
max_temp = 100; % in degrees Celsius, maximum temperature

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

% Generate data for plotting
throttle_values = linspace(0, 100, 500);
temperature_values = linspace(0, 100, 500);

% Calculate current for varying throttle at a fixed temperature (e.g., 50°C)
current_vs_throttle = arrayfun(@(t) esc_current(t, 50, max_current, max_throttle, temp_threshold, max_temp), throttle_values);

% Calculate current for varying temperature at a fixed throttle (e.g., 50%)
current_vs_temperature = arrayfun(@(temp) esc_current(50, temp, max_current, max_throttle, temp_threshold, max_temp), temperature_values);

% Plot Current vs Throttle
figure;
subplot(1, 2, 1);
plot(throttle_values, current_vs_throttle, 'b');
xlabel('Throttle (%)');
ylabel('Current (A)');
title('Current vs Throttle');
grid on;

% Plot Current vs Temperature
subplot(1, 2, 2);
plot(temperature_values, current_vs_temperature, 'r');
xlabel('Temperature (°C)');
ylabel('Current (A)');
title('Current vs Temperature');
grid on;

% Show plots
sgtitle('ESC Current Output Analysis');
