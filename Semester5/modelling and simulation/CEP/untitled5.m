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

% Apply a 120-degree phase shift
phase_shift = 120; % Phase shift in degrees
shift_samples = round(phase_shift / 360 * length(t)); % Convert phase shift to samples
u_shifted = circshift(u, shift_samples);

% Plot the original and phase-shifted square waves
figure;

subplot(2, 1, 1);
plot(t, u);
xlabel('Time (seconds)');
ylabel('Amplitude');
title('Original Square Wave with 33.33% Duty Cycle');
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal

subplot(2, 1, 2);
plot(t, u_shifted);
xlabel('Time (seconds)');
ylabel('Amplitude');
title('Phase-Shifted Square Wave (120°)');
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal