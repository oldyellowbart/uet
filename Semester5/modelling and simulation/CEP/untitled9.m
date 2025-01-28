% Define the time vector
t = 0:0.001:1; % from 0 to 1 second with increments of 0.001

% Generate the pulse signal
pulse = generate_pulse(t);

% Define the phase shift
phase_shift = 180; % Phase shift in degrees
shift_samples = round(phase_shift / 360 * length(t)); % Convert phase shift to samples

% Apply the phase shift
pulse_shifted = circshift(pulse, shift_samples);

% Plot the original and phase-shifted pulse signals
figure;


plot(t, pulse);
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal
hold on;

plot(t, pulse_shifted);
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal
hold off