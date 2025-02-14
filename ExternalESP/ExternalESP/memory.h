// memory.h
#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <cstdint>
#include "SDK.hpp"

class Memory {
public:
    Memory(const char* processName);
    ~Memory();

    template<typename T>
    T Read(uintptr_t address) {
        T value;
        if (!ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(T), nullptr)) {
            throw std::runtime_error("Failed to read memory");
        }
        return value;
    }

    template<typename T>
    void Write(uintptr_t address, const T& value) {
        if (!WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), nullptr)) {
            throw std::runtime_error("Failed to write memory");
        }
    }

    bool IsProcessRunning() const { return processHandle != nullptr; }
    uintptr_t GetBaseAddress() const { return baseAddress; }

private:
    HANDLE processHandle;
    uintptr_t baseAddress;

    static DWORD GetProcessIdByName(const char* processName);
    static uintptr_t GetModuleBaseAddress(DWORD processId, const char* moduleName);
};