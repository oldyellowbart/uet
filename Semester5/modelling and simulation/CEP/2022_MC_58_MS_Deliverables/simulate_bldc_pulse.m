% Define the time vector
t = 0:0.001:0.1; % from 0 to 1 second with increments of 0.001

% Define the square wave input with 33.33% duty cycle
duty_cycle = 33.33; % Duty cycle in percentage
period = 0.06; % Total period (e.g., 60 ms for this example)
high_duration = 0.02; % High duration
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

% Apply phase shifts
phase_shift1 = 0; % Phase shift in degrees
phase_shift2 = 60; % Phase shift in degrees
phase_shift3 = 120; % Phase shift in degrees

shift_samples1 = round(phase_shift1 / 360 * length(t)); % Convert phase shift to samples
shift_samples2 = round(phase_shift2 / 360 * length(t)); % Convert phase shift to samples
shift_samples3 = round(phase_shift3 / 360 * length(t)); % Convert phase shift to samples

u1 = circshift(u, shift_samples1);
u2 = circshift(u, shift_samples2);
u3 = circshift(u, shift_samples3);

% Plot the square waves



plot(t, u1);
ylim([-0.1 1.1]);
hold on;

plot(t, u2);
ylim([-0.1 1.1]);
hold on

plot(t, u3);
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal
hold off;