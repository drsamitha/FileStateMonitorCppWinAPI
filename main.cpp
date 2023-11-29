#include <iostream>
#include <windows.h>

void MonitorCurrentDirectory() {
    wchar_t currentDirectory[MAX_PATH];
    GetCurrentDirectoryW(MAX_PATH, currentDirectory);

    HANDLE hChange = FindFirstChangeNotificationW(
        currentDirectory,
        FALSE,
        FILE_NOTIFY_CHANGE_LAST_WRITE
    );

    if (hChange == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Error creating change notification handle: " << GetLastError() << std::endl;
        return;
    }

    DWORD waitStatus;

    while (true) {
        waitStatus = WaitForSingleObject(hChange, INFINITE);

        if (waitStatus == WAIT_OBJECT_0) {
            std::wcout << L"File modified in directory: " << currentDirectory << std::endl;

            // Reset the change notification
            if (!FindNextChangeNotification(hChange)) {
                std::wcerr << L"Error resetting change notification handle: " << GetLastError() << std::endl;
                break;
            }
        } else {
            std::wcerr << L"Error waiting for change notification: " << GetLastError() << std::endl;
            break;
        }
    }

    FindCloseChangeNotification(hChange);
}

int main() {
    MonitorCurrentDirectory();

    return 0;
}
