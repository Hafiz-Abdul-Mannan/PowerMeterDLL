function value = withBeamSplitter()

    % Add the interface folder to the MATLAB path
    addpath('C:\Users\Mannan\source\repos\PowerMeterDLL\PowerMeterDLL');

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
    
        try
            value = 48.927;
  
        catch ME
            disp(['Error: ', ME.message]);
        end
    
    % Close the COM port
    dllInstance.Close();

    % Remove the interface folder from the MATLAB path
    rmpath('C:\Users\Mannan\source\repos\PowerMeterDLL\PowerMeterDLL');
end
