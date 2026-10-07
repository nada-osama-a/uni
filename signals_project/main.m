clear; close all; clc;

fs_input = input('Enter Sampling Frequency (Hz) [Default 1000]: ', 's');
if isempty(fs_input), fs = 1000; else fs = str2double(fs_input); end

t_start_input = input('Enter Start Time [Default 0]: ', 's');
if isempty(t_start_input), t_start = 0; else t_start = str2double(t_start_input); end

t_end_input = input('Enter End Time [Default 5]: ', 's');
if isempty(t_end_input), t_end = 5; else t_end = str2double(t_end_input); end

t = t_start : 1/fs : t_end;

% Number and positions of breakpoints
n_bps_input = input('Number of breakpoints [Default 0]: ', 's');
if isempty(n_bps_input), n_bps = 0; else n_bps = str2double(n_bps_input); end

bps = [];
for i = 1:n_bps
    bp_val = input(['  Position for Breakpoint ', num2str(i), ': ']);
    bps = [bps, bp_val];
end
bps = sort(bps);

% Define boundaries for regions
boundaries = [t_start, bps, t_end];
n_regions = length(boundaries) - 1;
y = zeros(size(t));

% 2. SIGNAL GENERATION
for i = 1:n_regions
    t1 = boundaries(i);
    t2 = boundaries(i+1);
    
    % interval masking to prevent overlaps
    if i == n_regions
        mask = (t >= t1) & (t <= t2);
    else
        mask = (t >= t1) & (t < t2);
    end
    
    t_reg = t(mask); 
    
    fprintf('\n--- Region %d: [%.2f to %.2f] ---\n', i, t1, t2);
    fprintf('1:DC, 2:Ramp, 3:Polynomial, 4:Exponential, 5:Sine, 6:Gaussian, 7:Sawtooth\n');
    choice = input('Select Signal Type: ');
    
    switch choice
        case 1 % DC
            A = input('  Amplitude: ');
            y_reg = A * ones(size(t_reg));
        case 2 % Ramp
            m = input('  Slope: ');
            c = input('  Intercept: ');
            y_reg = m * t_reg + c;
        case 3 % Polynomial
            deg = input('  Degree: ');
            A = input('  Amplitude: ');
            y_reg = A * (t_reg.^deg);
        case 4 % Exponential 
            A = input('  Amplitude: ');
            a = input('  Exponent: ');
            y_reg = A * exp(a * t_reg);
        case 5 % Sinusoidal 
            A = input('  Amplitude: ');
            f = input('  Freq (Hz): ');
            y_reg = A * sin(2*pi*f * t_reg);
        case 6 % Gaussian Pulse 
            A = input('  Amplitude: ');
            mean_v = input('  Mean: ');
            width = input('  Width (Std Dev): ');
            y_reg = A * exp(-(t_reg - mean_v).^2 / (2 * width^2));
        case 7 % Sawtooth 
            A = input('  Amplitude: ');
            f = input('  Frequency: ');
            y_reg = A * (2 * (f*t_reg - floor(f*t_reg + 0.5)));
        otherwise
            y_reg = zeros(size(t_reg));
    end
    y(mask) = y_reg; 
end

% Display final signal 
figure('Name', 'Manual Piecewise Signal');
plot(t, y, 'b-', 'LineWidth', 1.5);
grid on; 
xlabel('Time (s)'); 
ylabel('Amplitude'); 
title('Generated Piecewise Signal');

% 3. SIGNAL OPERATIONS MENU 
y_mod = y;
t_mod = t;
active = true;

while active
    fprintf('\n--- Signal Operations ---\n');
    fprintf('1:Scale, 2:Reverse, 3:Shift, 4:Expand, 5:Compress, 6:Noise, 7:Smooth, 8:Exit\n');
    op = input('Choose Operation: ');
    
    if op == 1 % Amplitude scaling 
        k = input('  Scale Factor: ');
        y_mod = k * y_mod;
        
    elseif op == 2 % Time reversal 
        y_mod = y_mod(end:-1:1);
        t_mod = -t_mod(end:-1:1);
        
    elseif op == 3 % Time shift 
        sh = input('  Shift Value: ');
        t_mod = t_mod + sh;
        
    elseif op == 4 % Expansion 
        ex = input('  Expansion Factor: ');
        t_mod = t_mod * ex;
        
    elseif op == 5 % Compression 
        co = input('  Compression Factor: ');
        t_mod = t_mod / co;
        
    elseif op == 6 % Addition of random noise 
        snr_db = input('  SNR (dB): ');
        sig_pwr = sum(y_mod.^2) / length(y_mod);
        noise_pwr = sig_pwr / (10^(snr_db/10));
        y_mod = y_mod + sqrt(noise_pwr) * randn(size(y_mod));
        
    elseif op == 7 % Smoothing (Moving Average) 
        win = input('  Window Size: ');
        temp_y = y_mod;
        for n = 1:length(y_mod) 
            idx1 = max(1, n - floor(win/2));
            idx2 = min(length(y_mod), n + floor(win/2));
            temp_y(n) = sum(y_mod(idx1:idx2)) / (idx2 - idx1 + 1);
        end
        y_mod = temp_y;
        
    else
        active = false;
    end
    
    if active && op ~= 8
        figure;
        plot(t_mod, y_mod, 'r-', 'LineWidth', 1.5);
        grid on; 
        title('Updated Signal After Operation');
        xlabel('Time (s)'); 
        ylabel('Amplitude');
    end
end

% 4. AUTOMATIC GENERATION (10 SIGNALS) 
fprintf('\n--- Generating 10 Random Signals ---\n');

if ~exist('Auto_Generated_Signals', 'dir')
    mkdir('Auto_Generated_Signals');
end

for n = 1:10
    fprintf('  Generating signal %d/10...\n', n);
    
    t_auto = t_start:1/fs:t_end;
    y_auto = zeros(size(t_auto));
    
    % Random Breakpoints
    n_bp_rand = randi([0, 4]);
    bp_rand = sort(t_start + (t_end - t_start) * rand(1, n_bp_rand));
    b_rand = [t_start, bp_rand, t_end];
    
    for r = 1:(length(b_rand)-1)
        r1 = b_rand(r);
        r2 = b_rand(r+1);
        m_auto = (t_auto >= r1) & (t_auto <= r2);
        tr = t_auto(m_auto);
        
        type = randi([1, 7]);
        
        switch type
            case 1 % DC
                yr = (10 * rand() - 5) * ones(size(tr));
            case 2 % Ramp
                yr = (10 * rand() - 5) * tr + (10 * rand() - 5);
            case 3 % Polynomial
                yr = (5 * rand() - 2.5) * (tr.^2);
            case 4 % Exponential
                yr = rand() * exp((2*rand() - 1) * tr);
            case 5 % Sinusoidal
                yr = 5 * rand() * sin(2*pi*randi([1,5]) * tr);
            case 6 % Gaussian
                width = (r2 - r1) / (3 + 5*rand());
                yr = exp(-(tr - (r1+r2)/2).^2 / (2 * width^2));
            case 7 % Sawtooth
                freq = randi([1,5]);
                yr = 2 * (freq*tr - floor(freq*tr + 0.5));
        end
        y_auto(m_auto) = yr;
    end
    
    % Plot and save 
    figure('Position', [100, 100, 800, 400]);
    plot(t_auto, y_auto, 'b-', 'LineWidth', 1.5);
    grid on;
    title(['Auto-Generated Signal #', num2str(n)]);
    xlabel('Time (s)');
    ylabel('Amplitude');
    saveas(gcf, ['Auto_Generated_Signals/Signal_', num2str(n), '.png']);
    pause(1);
    close(gcf);
end

fprintf('  SUCCESS! 10 random signals saved in folder: \n');
fprintf('  "Auto_Generated_Signals"\n');