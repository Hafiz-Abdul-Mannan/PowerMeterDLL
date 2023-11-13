% Add the interface folder to the MATLAB path
addpath('C:\Users\Mannan\source\repos\combineConsole');

% Create an instance of the PowerMeterDLL class
dllInstance = clib.PowerMeterDLL.PowerMeterDLL();

% Specify the COM port name and baud rate
comPortName = '\\.\COM11'; % Replace with the desired COM port name
baudRate = 9600; % Desired baud rate

% Open the COM port
if ~dllInstance.Open(comPortName, baudRate)
    disp('Failed to open the COM port!');
    return;
end

% Define the number of iterations
num_iterations = 10;

% Initialize an empty matrix to store angle and corresponding values
dataMatrix = zeros(num_iterations, 2);

% Initialize the angle variable
angle = 0;

for i = 1:num_iterations
    % Call the GetValue function to retrieve the value from the power meter
    try
        value = dllInstance.GetValue();
        disp(['Received value: ', num2str(value)]);
        
        % Store angle and corresponding value in the matrix
        dataMatrix(i, 1) = angle;
        dataMatrix(i, 2) = value;
        
        % Increment the angle by 22.5 degrees
        angle = angle + 22.5;
    catch ME
        disp(['Error: ', ME.message]);
    end
    
    % Pause for one second
    pause(1);
end

% Close the COM port
dllInstance.Close();

% Remove the interface folder from the MATLAB path
rmpath('C:\Users\Mannan\source\repos\combineConsole');

% Display the stored dataMatrix
disp('Stored dataMatrix:');
disp(dataMatrix);
