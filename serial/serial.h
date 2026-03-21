#pragma once

#include <cstdint>
#include <stdio.h>
#include <windows.h>

class Serial {
public:
  Serial(const char *port, uint32_t baud_rate);
  ~Serial();

  bool isOpen() const;
  bool write(const uint8_t *data, size_t length);

private:
  HANDLE m_hSerial;
  bool m_isOpen;
};
