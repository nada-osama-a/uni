% Question 1
clear all; close all; clc;

% Part (b) - Compute Fourier Transform
w = linspace(-50, 50, 3000);

% First part: e^(-2t) from 0 to 2
X1 = (1 - exp(-(2 + 1j*w)*2)) ./ (2 + 1j*w);

% Second part: damped cosine from 2 to infinity
X2 = 0.5 * (exp(-(0.5 + 1j*(w - 4*pi))*2) ./ (0.5 + 1j*(w - 4*pi)) +...
            exp(-(0.5 + 1j*(w + 4*pi))*2) ./ (0.5 + 1j*(w + 4*pi)));
X = X1 + X2;

figure('Name', 'Question 1', 'Position', [100, 100, 1200, 900]);
subplot(2,2,1);
plot(w, abs(X), 'b-', 'LineWidth', 1.5);
title('(1) Magnitude Spectrum |X(\omega)|');
xlabel('\omega (rad/s)');
ylabel('|X(\omega)|');
grid on;
xlim([-50, 50]);

% Plot 2: Phase Spectrum
subplot(2,2,2);
plot(w, angle(X), 'r-', 'LineWidth', 1.5);
title('(2) Phase Spectrum \angle X(\omega)');
xlabel('\omega (rad/s)');
ylabel('Phase (radians)');
grid on;
xlim([-50, 50]);

% Part (c) - 95% Energy Bandwidth
E_total = trapz(w, abs(X).^2);
% cumulative energy
E_cum = cumtrapz(w, abs(X).^2);
% Find frequency where cumulative energy reaches 95%
B = 0;
for i = 1:length(w)
    if E_cum(i) >= 0.95 * E_total
        B = w(i);
        break;
    end
end

% Plot 3: Cumulative Energy
subplot(2,2,3);
plot(w, E_cum/E_total*100, 'b-', 'LineWidth', 1.5);
hold on;
plot([B B], [0 100], 'r--', 'LineWidth', 1.5);
plot([-50 50], [95 95], 'g--', 'LineWidth', 1.5);
xlabel('\omega (rad/s)');
ylabel('Cumulative Energy (%)');
title(sprintf('(3) Cumulative Energy (95%% at ω = %.2f rad/s)', B));
legend('Energy', '95% point', '95% line', 'Location', 'best');
grid on;
xlim([0, 50]);

% Plot 4: Magnitude Spectrum with Bandwidth Marked (Zoomed)
subplot(2,2,4);
plot(w, abs(X), 'b-', 'LineWidth', 1.5);
hold on;
plot([B B], [0 max(abs(X))], 'r--', 'LineWidth', 1.5);
plot([-B -B], [0 max(abs(X))], 'r--', 'LineWidth', 1.5);
xlabel('\omega (rad/s)');
ylabel('|X(\omega)|');
title(sprintf('(4) 95%% Bandwidth: ω_B = %.2f rad/s (%.2f Hz)', B, B/(2*pi)));
legend('|X(\omega)|', 'Bandwidth limits', 'Location', 'best');
grid on;
xlim([-30, 30]);
