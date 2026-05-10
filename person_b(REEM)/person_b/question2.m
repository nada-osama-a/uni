clear all; close all; clc;

%Time Domain
fs = 10000;              
t = -10:1/fs:10;        

% x1(t) 
x1 = (sin(2*pi*t)./(2*pi*t)) .* cos(3*pi*t);
x1(t==0) = 1; 

% x2(t) calculation
rect = (abs(t) <= 2);
x2 = rect .* cos(3*pi*t);

figure;
subplot(2,2,1);
plot(t, x1, 'b','LineWidth',1.5); title('x_1(t) = sinc(2t) cos(3\pi t)'); grid on;
subplot(2,2,2);
plot(t, x2, 'r','LineWidth',1.5); title('x_2(t) = rect(t/4) cos(3\pi t)'); grid on;

%Frequency Domain (FFT)
L = length(t);
X1 = fftshift(fft(x1))/L; % Normalized FFT
X2 = fftshift(fft(x2))/L;
f = (-L/2:L/2-1)*(fs/L); 

subplot(2,2,3);
plot(f, abs(X1), 'b', 'LineWidth', 1.5);
title('Magnitude Spectrum of x_1(t)');
xlim([-5 5]); grid on;

subplot(2,2,4);
plot(f, abs(X2), 'r', 'LineWidth', 1.5);
title('Magnitude Spectrum of x_2(t)');
xlim([-5 5]); grid on;
xlabel('Frequency (Hz)');