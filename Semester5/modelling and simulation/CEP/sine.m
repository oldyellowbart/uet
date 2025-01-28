% Define the frequency
f = 50; % frequency in Hz
ph1 = 0;
ph2 = 120;
ph3 = 240;
degrees = t * 360;
t = 0:0.001:0.1;
t1 = 0:0.5:20;
for(t1){s=1;}

phase_shift1 = ph1 * (pi/180);
y1 = ((square(2*pi*f*t + phase_shift1)+1)/2);
phase_shift2 = ph2 * (pi/180);
y2 = (square(2*pi*f*t + phase_shift2)+1)/2;
phase_shift3 = ph3 * (pi/180);
y3 = (square(2*pi*f*t + phase_shift3)+1)/2;

plot(degrees, y1);
ylim([-2 2]);

hold on;
%plot(degrees, y2);

ylim([-2 2]); 

%hold on;
%plot(degrees, y3);
%ylim([-2 2]); 
end
% Add labels and title
xlabel('Time (seconds)');
ylabel('Amplitude');
title(['Sine Wave with Frequency ', num2str(f), ' Hz']);
hold off;