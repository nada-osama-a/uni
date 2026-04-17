% config.m - Shared settings for the project

% Default values for user input
DEFAULT_FS = 1000;           % Sampling frequency (Hz)
DEFAULT_T_START = 0;         % Start time (seconds)
DEFAULT_T_END = 5;           % End time (seconds)

% Random ranges for auto-generation (will use later)
RANDOM_AMPLITUDE = [0.1, 10];
RANDOM_FREQ = [1, 50];
RANDOM_SLOPE = [-5, 5];