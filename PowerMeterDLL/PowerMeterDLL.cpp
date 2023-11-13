#include "pch.h"
#include "PowerMeterDLL.h"
#include <iostream>


extern "C" {


bool PowerMeterDLL::Open(const std::wstring& comPortName, DWORD baudRate) {
    m_comPortName = comPortName;
    this->baudRate = baudRate;
    m_hSerial = CreateFile(m_comPortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (m_hSerial == INVALID_HANDLE_VALUE)
        return false;
    return true;
}

bool PowerMeterDLL::IsOpen() {
    return (m_hSerial != NULL && m_hSerial != INVALID_HANDLE_VALUE);
}

    bool PowerMeterDLL::Close() {
        if (IsOpen()) {
            CloseHandle(m_hSerial);
            m_hSerial = NULL;
            return true;
        }
        return false;
    }

double PowerMeterDLL::dBmToMilliwatts(double dBmValue) {
    return pow(10, dBmValue / 10);
}

double PowerMeterDLL::GetValue() {
    if (!IsOpen()) {
        if (!Open(m_comPortName, this->baudRate)) {
            std::cerr << "Failed to open the COM port!" << std::endl;
            return 0.0;
        }
    }

    std::string command = ":BBPOWFULL:POW:DBM:WLEN1?\r\n";

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = MAXDWORD;
    timeouts.ReadTotalTimeoutConstant = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    if (!SetCommTimeouts(m_hSerial, &timeouts)) {
        std::cerr << "Failed to set communication timeouts!" << std::endl;
        Close();
        return 0.0;
    }

    if (!WriteFile(m_hSerial, command.c_str(), static_cast<DWORD>(command.length()), NULL, NULL)) {
        std::cerr << "Failed to write to the COM port!" << std::endl;
        Close();
        return 0.0;
    }

    const int bufferSize = 256;
    char response[bufferSize];
    DWORD bytesRead;

    if (ReadFile(m_hSerial, response, sizeof(response) - 1, &bytesRead, NULL)) {
        response[bytesRead] = '\0';

        double dBmValue = atof(response);
        double milliwattsValue = dBmToMilliwatts(dBmValue);

        return milliwattsValue;
    }

    Close();
    return 0.0;
}

}
