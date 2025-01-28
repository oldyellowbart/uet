% Define the time vector
t = 0:0.001:1; % from 0 to 1 second with increments of 0.001

% Generate the pulse signal
pulse = generate_pulse(t);

% Plot the pulse signal
figure;
plot(t, pulse);
xlabel('Time (seconds)');
ylabel('Amplitude');
title('Pulse Signal with 20ms High and 40ms Low');
ylim([-0.1 1.1]); % Set y-axis limits to clearly show the signal