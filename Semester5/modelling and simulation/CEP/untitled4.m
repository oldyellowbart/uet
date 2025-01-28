% Define the time vector
t = 0:0.001:1; % from 0 to 1 second with increments of 0.001

% Define the square wave input with 33.33% duty cycle
duty_cycle = 33.33; % Duty cycle in percentage
period = 0.06; % Total period (e.g., 60 ms for this example)
high_duration = period * (duty_cycle / 100); % High duration
low_duration = period - high_duration; % Low duration

% Create the square wave signal
u = zeros(size(t));
for k = 1:length(t)
    if mod(t(k), period) < high_duration
        u(k) = 1; % High for the high duration
    else
        u(k) = 0; % Low for the remaining period
    end
end

% Plot the square wave
figure;
plot(t, u);
xlabel('Time (seconds)');
ylabel('Amplitude');
title('Square Wave with 33.33% Duty Cycle');
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal