// memory.cpp
#include "memory.h"
#include <stdexcept>

Memory::Memory(const char* processName) {
    DWORD processId = GetProcessIdByName(processName);
    if (!processId) {
        throw std::runtime_error("Failed to find process");
    }

    processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processId);
    if (!processHandle) {
        throw std::runtime_error("Failed to open process");
    }

    baseAddress = GetModuleBaseAddress(processId, processName);
    if (!baseAddress) {
        CloseHandle(processHandle);
        throw std::runtime_error("Failed to get module base address");
    }
}

Memory::~Memory() {
    if (processHandle) {
        CloseHandle(processHandle);
    }
}

DWORD Memory::GetProcessIdByName(const char* processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);

        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                char currentProcessName[MAX_PATH];
                size_t numConverted;
                wcstombs_s(&numConverted, currentProcessName, processEntry.szExeFile, MAX_PATH);
                if (_stricmp(currentProcessName, processName) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processId;
}

uintptr_t Memory::GetModuleBaseAddress(DWORD processId, const char* moduleName) {
    uintptr_t moduleBase = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);

        if (Module32FirstW(snapshot, &moduleEntry)) {
            do {
                char currentModuleName[MAX_PATH];
                size_t numConverted;
                wcstombs_s(&numConverted, currentModuleName, moduleEntry.szModule, MAX_PATH);
                if (_stricmp(currentModuleName, moduleName) == 0) {
                    moduleBase = (uintptr_t)moduleEntry.modBaseAddr;
                    break;
                }
            } while (Module32NextW(snapshot, &moduleEntry));
        }
        CloseHandle(snapshot);
    }
    return moduleBase;
}