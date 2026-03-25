#include "serial.h"
#include <cstddef>
#include <cstdint>

Serial::Serial(const char *port, uint32_t baud_rate) : m_isOpen(false) {
  m_hSerial = CreateFile(port, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

  if (m_hSerial == INVALID_HANDLE_VALUE) {
    return;
  }

  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  if (!GetCommState(m_hSerial, &dcbSerialParams)) {
    CloseHandle(m_hSerial);
    m_hSerial = INVALID_HANDLE_VALUE;
    return;
  }

  dcbSerialParams.BaudRate = baud_rate;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;

  if (!SetCommState(m_hSerial, &dcbSerialParams)) {
    CloseHandle(m_hSerial);
    m_hSerial = INVALID_HANDLE_VALUE;
    return;
  }
  m_isOpen = true;
}

Serial::~Serial() {
  if (m_hSerial != INVALID_HANDLE_VALUE) {
    CloseHandle(m_hSerial);
    m_isOpen = false;
  }
}

bool Serial::isOpen() const { return m_isOpen; }

bool Serial::write(const uint8_t *data, size_t length) {
  if (!m_isOpen) {
    return false;
  }

  DWORD bytes_writen;
  return WriteFile(m_hSerial, data, length, &bytes_writen, NULL);
}