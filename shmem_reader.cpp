#include <iostream>
#include "acc_shmem.h"

template <typename T>
class SharedMemReader {
private:
    HANDLE m_hMapFile;
    T* m_mapAddr;
    std::wstring mapName;

public:
    SharedMemReader(std::wstring name) : mapName(name), m_hMapFile(nullptr), m_mapAddr(nullptr) {}

    bool init() {
        m_hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, mapName.c_str());
        if (m_hMapFile == nullptr) {
            return false;
        }

        m_mapAddr = (T*)MapViewOfFile(m_hMapFile, FILE_MAP_READ, 0, 0, sizeof(T));
      
        return m_mapAddr != nullptr;
    }

    const T* data() const { return m_mapAddr; }

    ~SharedMemReader() {
        if (m_mapAddr) {
            UnmapViewOfFile(m_mapAddr);
        }
        if (m_hMapFile) {
            CloseHandle(m_hMapFile);
        }
    }
};