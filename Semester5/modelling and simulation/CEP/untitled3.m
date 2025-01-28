% Define the dataset
x = linspace(0, 180,3)
% Calculate the combined function
y_combined = sin(x)  

% Plot the combined function
figure;
plot(x, y_combined, 'k'); % Plot in black

% Add labels and title
xlabel('X-axis');
ylabel('Y-axis');
title('Combined Graph of Sine and Cosine Waves');
legend('sin(x) + cos(x)');