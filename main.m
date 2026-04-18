%% ========================================================================
%%  PART II - INTERACTIVE SIGNAL GENERATOR
%% ========================================================================
clear;clc;
fprintf('\n========================================\n');
fprintf('    INTERACTIVE SIGNAL GENERATOR        \n');
fprintf('========================================\n\n');




 [fs, t_start, t_end, breakpoints] = get_user_input();

 t=t_start:1/fs:t_end;
boundries=[t_start,breakpoints,t_end];
num_regions= length(boundries)-1;
y=zeros(size(t));
for i=1:num_regions
    region_start=boundries(i);
    region_end=boundries(i+1);

    if i==num_regions
        mask=(t>=region_start)&(t<=region_end);
    else
        mask=(t >= region_start)&(t < region_end);
    end
t_region=t(mask);
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
        type=input('\nchoose signal type (1-7):');
        switch type
            case 1 %DC
                   A=input('Amplitude= ');
                   y_region=A*ones(size(t_region));

            case 2 %Ramp
                   m=input('Slope= ');
                   b=input('intercept= ');
                   y_region=m.*t_region+b;

            case 3 %Polynomial
                  order=input('Order= ');
                  A=input('Amplitude= ');
                  intercept=input('intercept= ');
                  y_region=A.*(t_region.^order)+intercept;

            case 4 %Exp
                  amp=input('Amplitude= ');
                  exponent=input('exponent= ');
                  y_region=amp.*exp(exponent.*t_region);

            case 5 %sinusoidal
                  amp=input('Amplitude= ');
                  freq=input('Frequancy(Hz)= ');
                  y_region=amp.*sin(2*pi*freq.*t_region);

            case 6 %Gaussian
                amp = input('Enter amplitude= ');
                mean_val = input('Enter mean (center time)= ');
                width = input('Enter width (standard deviation)= ');
                y_region = amp .* exp(-(t_region - mean_val).^2 / (2 * width.^2));

            case 7 %sawtooth
                amp = input('Enter amplitude: ');
                freq = input('Enter frequency (Hz): ');
                y_region= amp* (2 * (freq*t_region - floor(freq*t_region + 0.5)));

            otherwise
                fprintf('Invaild signal type\n');
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
legend('Signal', 'Breakpoints', 'Location', 'best');

fprintf('\n=== Signal Generated Successfully ===\n');

% ========================================================================
% Function definition (put at the end or in separate file)
% ========================================================================

function [fs, t_start, t_end, breakpoints] = get_user_input()
% GET_USER_INPUT Ask user for sampling frequency, time range, and breakpoints
% Returns: fs, t_start, t_end, breakpoints (array of breakpoint times)

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
