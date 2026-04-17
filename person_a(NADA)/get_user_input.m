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