%% About definePowerMeterDLL.mlx
% This file defines the MATLAB interface to the library |PowerMeterDLL|.
%
% Commented sections represent C++ functionality that MATLAB cannot automatically define. To include
% functionality, uncomment a section and provide values for &lt;SHAPE&gt;, &lt;DIRECTION&gt;, etc. For more
% information, see <matlab:helpview(fullfile(docroot,'matlab','helptargets.map'),'cpp_define_interface') Define MATLAB Interface for C++ Library>.



%% Setup
% Do not edit this setup section.
function libDef = definePowerMeterDLL()
libDef = clibgen.LibraryDefinition("PowerMeterDLLData.xml");
%% OutputFolder and Libraries 
libDef.OutputFolder = "C:\Users\Mannan\source\repos\PowerMeterDLL\PowerMeterDLL";
libDef.Libraries = "C:\Users\Mannan\source\repos\PowerMeterDLL\PowerMeterDLL\PowerMeterDLL.lib";

%% C++ class |PowerMeter| with MATLAB name |clib.PowerMeterDLL.PowerMeter| 
PowerMeterDefinition = addClass(libDef, "PowerMeter", "MATLABName", "clib.PowerMeterDLL.PowerMeter", ...
    "Description", "clib.PowerMeterDLL.PowerMeter    Representation of C++ class PowerMeter."); % Modify help description values as needed.

%% C++ class method |Open| for C++ class |PowerMeter| 
% C++ Signature: bool PowerMeter::Open(std::wstring const & comPortName,DWORD baudRate)
OpenDefinition = addMethod(PowerMeterDefinition, ...
    "bool PowerMeter::Open(std::wstring const & comPortName,DWORD baudRate)", ...
    "MATLABName", "Open", ...
    "Description", "Open Method of C++ class PowerMeter."); % Modify help description values as needed.
defineArgument(OpenDefinition, "comPortName", "string", "input");
defineArgument(OpenDefinition, "baudRate", "uint32");
defineOutput(OpenDefinition, "RetVal", "logical");
validate(OpenDefinition);

%% C++ class method |IsOpen| for C++ class |PowerMeter| 
% C++ Signature: bool PowerMeter::IsOpen()
IsOpenDefinition = addMethod(PowerMeterDefinition, ...
    "bool PowerMeter::IsOpen()", ...
    "MATLABName", "IsOpen", ...
    "Description", "IsOpen Method of C++ class PowerMeter."); % Modify help description values as needed.
defineOutput(IsOpenDefinition, "RetVal", "logical");
validate(IsOpenDefinition);

%% C++ class method |Close| for C++ class |PowerMeter| 
% C++ Signature: bool PowerMeter::Close()
CloseDefinition = addMethod(PowerMeterDefinition, ...
    "bool PowerMeter::Close()", ...
    "MATLABName", "Close", ...
    "Description", "Close Method of C++ class PowerMeter."); % Modify help description values as needed.
defineOutput(CloseDefinition, "RetVal", "logical");
validate(CloseDefinition);

%% C++ class method |GetValue| for C++ class |PowerMeter| 
% C++ Signature: double PowerMeter::GetValue()
GetValueDefinition = addMethod(PowerMeterDefinition, ...
    "double PowerMeter::GetValue()", ...
    "MATLABName", "GetValue", ...
    "Description", "GetValue Method of C++ class PowerMeter."); % Modify help description values as needed.
defineOutput(GetValueDefinition, "RetVal", "double");
validate(GetValueDefinition);

%% C++ class constructor for C++ class |PowerMeter| 
% C++ Signature: PowerMeter::PowerMeter()
PowerMeterConstructor1Definition = addConstructor(PowerMeterDefinition, ...
    "PowerMeter::PowerMeter()", ...
    "Description", "clib.PowerMeterDLL.PowerMeter Constructor of C++ class PowerMeter."); % Modify help description values as needed.
validate(PowerMeterConstructor1Definition);

%% C++ class constructor for C++ class |PowerMeter| 
% C++ Signature: PowerMeter::PowerMeter(PowerMeter const & input1)
PowerMeterConstructor2Definition = addConstructor(PowerMeterDefinition, ...
    "PowerMeter::PowerMeter(PowerMeter const & input1)", ...
    "Description", "clib.PowerMeterDLL.PowerMeter Constructor of C++ class PowerMeter."); % Modify help description values as needed.
defineArgument(PowerMeterConstructor2Definition, "input1", "clib.PowerMeterDLL.PowerMeter", "input");
validate(PowerMeterConstructor2Definition);

%% C++ class public data member |m_comPortName| for C++ class |PowerMeter| 
% C++ Signature: std::wstring PowerMeter::m_comPortName
addProperty(PowerMeterDefinition, "m_comPortName", "string", ...
    "Description", "string16    Data member of C++ class PowerMeter."); % Modify help description values as needed.

%% C++ class public data member |baudRate| for C++ class |PowerMeter| 
% C++ Signature: DWORD PowerMeter::baudRate
addProperty(PowerMeterDefinition, "baudRate", "uint32", ...
    "Description", "uint32    Data member of C++ class PowerMeter."); % Modify help description values as needed.

%% Validate the library definition
validate(libDef);

end
