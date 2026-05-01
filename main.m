
clear all; close all; clc;
fprintf('\n========================================\n');
fprintf('    INTERACTIVE SIGNAL GENERATOR        \n');
fprintf('========================================\n\n');

% Get user input
[fs, t_start, t_end, breakpoints] = get_user_input();

% Create time vector
t = t_start:1/fs:t_end;
boundries = [t_start, breakpoints, t_end];
num_regions = length(boundries) - 1;
y = zeros(size(t));

% Generate each region
for i = 1:num_regions
    region_start = boundries(i);
    region_end = boundries(i+1);

    if i == num_regions
        mask = (t >= region_start) & (t <= region_end);
    else
        mask = (t >= region_start) & (t < region_end);
    end
    t_region = t(mask);
    
    fprintf('\nRegion %d: [%.2f to %.2f]\n', i, region_start, region_end);
    
    % Display signal options
    fprintf('\nAvailable signal types:\n');
    fprintf('1. DC (amplitude)\n');
    fprintf('2. Ramp (slope, intercept)\n');
    fprintf('3. Polynomial (user-defined order - amplitude - intercept)\n');
    fprintf('4. Exponential (amplitude, exponent)\n');
    fprintf('5. Sinusoidal (amplitude, frequency)\n');
    fprintf('6. Gaussian pulse (amplitude, mean, width)\n');
    fprintf('7. Sawtooth wave (amplitude, frequency)\n');
    type = input('\nChoose signal type (1-7): ');
    
    switch type
        case 1 % DC
            A = input('Amplitude = ');
            y_region = A * ones(size(t_region));

        case 2 % Ramp
            m = input('Slope = ');
            b = input('Intercept = ');
            y_region = m .* t_region + b;

        case 3 % Polynomial
            order = input('Order = ');
            A = input('Amplitude = ');
            intercept = input('Intercept = ');
            y_region = A .* (t_region.^order) + intercept;

        case 4 % Exponential
            amp = input('Amplitude = ');
            exponent = input('Exponent = ');
            y_region = amp .* exp(exponent .* t_region);

        case 5 % Sinusoidal
            amp = input('Amplitude = ');
            freq = input('Frequency (Hz) = ');
            y_region = amp .* sin(2*pi*freq .* t_region);

        case 6 % Gaussian
            amp = input('Enter amplitude = ');
            mean_val = input('Enter mean (center time) = ');
            width = input('Enter width (standard deviation) = ');
            y_region = amp .* exp(-(t_region - mean_val).^2 / (2 * width.^2));

        case 7 % Sawtooth
            amp = input('Enter amplitude = ');
            freq = input('Enter frequency (Hz) = ');
            y_region = amp * (2 * (freq*t_region - floor(freq*t_region + 0.5)));

        otherwise
            fprintf('Invalid signal type\n');
            y_region = zeros(size(t_region));
    end
    
    % Store the generated signal
    y(mask) = y_region;
end

% Plot the final signal
figure('Name', 'Generated Signal', 'Position', [100, 100, 1000, 500]);
plot(t, y, 'b-', 'LineWidth', 1.5);
hold on;

% Mark breakpoints
for bp = breakpoints
    xline(bp, 'r--', 'LineWidth', 1);
end

xlabel('Time (seconds)');
ylabel('Amplitude');
title('Generated Piecewise Signal');
grid on;
if ~isempty(breakpoints)
    legend('Signal', 'Breakpoints', 'Location', 'best');
else
    legend('Signal', 'Location', 'best');
end

fprintf('\n=== Signal Generated Successfully ===\n');

%% ========================================================================
%% OPERATIONS MENU
%% ========================================================================

operation_running = true;
current_signal = y;
current_t = t;

while operation_running
    fprintf('\n========================================\n');
    fprintf('        OPERATIONS MENU                  \n');
    fprintf('========================================\n');
    fprintf('1. Amplitude scaling\n');
    fprintf('2. Time reversal\n');
    fprintf('3. Time shift\n');
    fprintf('4. Expansion (stretch in time)\n');
    fprintf('5. Compression (squeeze in time)\n');
    fprintf('6. Addition of random noise (by SNR)\n');
    fprintf('7. Smoothing using moving average\n');
    fprintf('8. None (exit operations)\n');
    fprintf('========================================\n');
    
    op_choice = input('Choose operation (1-8): ');
    
    switch op_choice
        case 1 % Scaling
            k = input('Scale factor: ');
            current_signal = k * current_signal;
            fprintf('Amplitude scaled by %.2f\n', k);
            
        case 2 % Time reversal (manual)
            % Reverse signal manually
            temp_signal = current_signal;
            for i = 1:length(current_signal)
                current_signal(i) = temp_signal(length(current_signal) - i + 1);
            end
            
    % Reverse time manually
    temp_t = current_t;
    for i = 1:length(current_t)
        current_t(i) = -temp_t(length(current_t) - i + 1);
    end
    fprintf('Time reversal applied\n');
            
        case 3 % Time shift
            s = input('Shift value (s): ');
            current_t = current_t + s;
            fprintf('Time shifted by %.2f seconds\n', s);
            
        case 4 % Expansion
            a = input('Expansion factor: ');
            current_t = a * current_t;
            fprintf('Time expanded by factor %.2f\n', a);
            
        case 5 % Compression
            a = input('Compression factor: ');
            current_t = current_t / a;
            fprintf('Time compressed by factor %.2f\n', a);
            
        case 6 % Noise
            snr = input('SNR (dB): ');
            signal_power = mean(current_signal.^2);
            noise_power = signal_power / (10^(snr/10));
            noise = sqrt(noise_power) * randn(size(current_signal));
            current_signal = current_signal + noise;
            fprintf('Noise added with SNR = %.1f dB\n', snr);
            
        case 7 % Smoothing
            M = input('Smoothing window length: ');
            y_smooth = zeros(size(current_signal));
            for n = 1:length(current_signal)
                k1 = max(1, n - floor(M/2));
                k2 = min(length(current_signal), n + floor(M/2));
                y_smooth(n) = sum(current_signal(k1:k2)) / (k2 - k1 + 1);
            end
            current_signal = y_smooth;
            fprintf('Smoothed with window size = %.0f\n', M);
            
        case 8 % None
            fprintf('Exiting operations menu.\n');
            
        otherwise
            fprintf('Invalid choice\n');
    end
    
    % Plot updated signal after each operation (except exit)
    if op_choice ~= 8
        figure('Name', 'Signal After Operation', 'Position', [150, 150, 1000, 500]);
        plot(current_t, current_signal, 'b-', 'LineWidth', 1.5);
        xlabel('Time (seconds)');
        ylabel('Amplitude');
        title('Signal After Applied Operation');
        grid on;
        
        % Ask if user wants to continue
        cont = input('\nApply another operation? (1=Yes, 0=No): ');
        if cont ~= 1
            operation_running = false;
        end
    else
        operation_running = false;
    end
end

fprintf('\n=== Final Signal Ready ===\n');

fprintf('\n========================================\n');
fprintf('  GENERATING 10 RANDOM SIGNALS          \n');
fprintf('========================================\n\n');

% Create folder to save figures
if ~exist('Auto_Generated_Signals', 'dir')
    mkdir('Auto_Generated_Signals');
end

% Generate 10 automatic signals
for auto_num = 1:10
    fprintf('Auto generate signal %d/10...\n', auto_num)
    
    % Time parameter
    t_auto = t_start:1/fs:t_end;
    
    % Random breakpoints
    num_randbp = randi([0,4]);
    if num_randbp > 0
        breakpoints_auto = t_start + (t_end - t_start) * rand(1, num_randbp);
        breakpoints_auto = sort(breakpoints_auto);
    else
        breakpoints_auto = [];
    end
    
    % Define regions
    boundries_auto = [t_start, breakpoints_auto, t_end];
    num_randregions = length(boundries_auto) - 1;
    
    % Initialize signal
    y_auto = zeros(size(t_auto));
    
    % Generate each region automatically
    for region = 1:num_randregions
        region_start = boundries_auto(region);
        region_end = boundries_auto(region+1);
        
        % Mask for this region
        if region == num_randregions
            mask = (t_auto >= region_start) & (t_auto <= region_end);
        else
            mask = (t_auto >= region_start) & (t_auto < region_end);
        end
        
        t_region_auto = t_auto(mask);
        
        % Signal type random from 1-7
        type_auto = randi([1,7]);
        
        switch type_auto
            case 1 % DC
                A = 10 * (rand() - 0.5);
                y_region_auto = A * ones(size(t_region_auto));
                
            case 2 % Ramp
                m = 10 * (rand() - 0.5);
                b = 10 * (rand() - 0.5);
                y_region_auto = m * t_region_auto + b;
                
            case 3 % Polynomial
                order = randi([1,2]);
                A = 5 * (rand() - 0.5);
                intercept = 5 * (rand() - 0.5);
                y_region_auto = A * (t_region_auto.^order) + intercept;
                
            case 4 % Exponential
                amp = 5 * rand();
                exponent = 2 * (rand() - 0.5);
                y_region_auto = amp * exp(exponent * t_region_auto);
                
            case 5 % Sinusoidal
                amp = 5 * rand();
                freq = randi([1,5]);
                y_region_auto = amp * sin(2*pi*freq * t_region_auto);
                
            case 6 % Gaussian
                amp = 5 * rand();
                mean_val = region_start + (region_end - region_start) * rand();
                width = (region_end - region_start) / (3 + 5 * rand());
                y_region_auto = amp * exp(-(t_region_auto - mean_val).^2 / (2 * width^2));
                
            case 7 % Sawtooth
                amp = 5 * rand();
                freq = randi([1,5]);
                y_region_auto = amp * (2 * (freq * t_region_auto - floor(freq * t_region_auto + 0.5)));
        end
        
        y_auto(mask) = y_region_auto;
    end
    
    % Plot and save
    figure('Position', [100, 100, 1000, 500]);
    plot(t_auto, y_auto, 'b-', 'LineWidth', 1.5);
    hold on;
    
    % Mark breakpoints
    if ~isempty(breakpoints_auto)
        for bp = breakpoints_auto
            xline(bp, 'r--', 'LineWidth', 1);
        end
        legend('Signal', 'Breakpoints', 'Location', 'best');
    else
        legend('Signal', 'Location', 'best');
    end
    
    xlabel('Time (seconds)');
    ylabel('Amplitude');
    title(sprintf('Auto-Generated Signal %d/%d (fs=%.1fHz, %d regions)', auto_num, 10, fs, num_randregions));
    grid on;
    
    filename = sprintf('Auto_Generated_Signals/Signal_%02d.png', auto_num);
    saveas(gcf, filename);
    close(gcf);
end

fprintf('\n================================================\n');
fprintf('  COMPLETE! 10 signals saved in folder:        \n');
fprintf('  "Auto_Generated_Signals"                     \n');
fprintf('     End of program    \n ')
fprintf('================================================\n');

%% ========================================================================
%% FUNCTION: GET USER INPUT
%% ========================================================================

function [fs, t_start, t_end, breakpoints] = get_user_input()

    % Ask for sampling frequency
    fs_input = input('Enter sampling frequency (Hz) [default 1000]: ', 's');
    if isempty(fs_input)
        fs = 1000;
    else
        fs = str2double(fs_input);
    end
    
    % Ask for start time
    start_input = input('Enter start time (seconds) [default 0]: ', 's');
    if isempty(start_input)
        t_start = 0;
    else
        t_start = str2double(start_input);
    end
    
    % Ask for end time
    end_input = input('Enter end time (seconds) [default 5]: ', 's');
    if isempty(end_input)
        t_end = 5;
    else
        t_end = str2double(end_input);
    end
    
    % Ask for number of breakpoints
    num_input = input('Enter number of breakpoints [default 0]: ', 's');
    if isempty(num_input)
        num_breakpoints = 0;
    else
        num_breakpoints = str2double(num_input);
    end
    
    % Get breakpoint positions
    breakpoints = [];
    for i = 1:num_breakpoints
        bp = input(sprintf('Enter breakpoint %d position (between %.1f and %.1f): ', i, t_start, t_end));
        breakpoints = [breakpoints, bp];
    end
    breakpoints = sort(breakpoints);
    
    fprintf('\n--- Settings Confirmed ---\n');
    fprintf('Sampling frequency: %.1f Hz\n', fs);
    fprintf('Time range: %.1f to %.1f seconds\n', t_start, t_end);
    fprintf('Breakpoints: ');
    disp(breakpoints);
end