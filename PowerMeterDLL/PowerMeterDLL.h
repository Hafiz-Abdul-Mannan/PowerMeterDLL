#pragma once

#include <string>
#include <Windows.h>

#ifdef POWERMETERDLL_EXPORTS
#define POWERMETERDLL_API __declspec(dllexport)
#else
#define POWERMETERDLL_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    class POWERMETERDLL_API PowerMeterDLL {
    public:
        
        bool Open(const std::wstring& comPortName, DWORD baudRate);
        bool IsOpen();
        bool Close();
        double GetValue();
        std::wstring m_comPortName = L"\\\\.\\COM11";
        void* m_hSerial = NULL;
        DWORD baudRate = 9600;

    private:

        double dBmToMilliwatts(double dBmValue);
    };

#ifdef __cplusplus
}
#endif
