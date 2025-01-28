function pulse = generate_pulse(t)
    % Function to generate a pulse signal with 20ms high and 40ms low

    % Define the high and low durations
    high_duration = 0.02; % 20 ms
    low_duration = 0.04; % 40 ms
    period = high_duration + low_duration; % Total period

    % Initialize the pulse signal
    pulse = zeros(size(t));

    % Generate the pulse signal
    for k = 1:length(t)
        if mod(t(k), period) < high_duration
            pulse(k) = 1; % High for the high duration
        else
            pulse(k) = 0; % Low for the remaining period
        end
    end
end

