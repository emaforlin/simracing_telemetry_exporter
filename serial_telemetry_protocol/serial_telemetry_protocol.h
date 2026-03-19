/**
 * @file serial_telemetry_protocol.h
 * @brief Binary packet protocol for serial telemetry communication.
 *
 * Packet format:
 * ┌──────────┬──────┬────────┬─────────────┬──────┐
 * │ START(2) │ TYPE │ LENGTH │ PAYLOAD(N)  │ CRC8 │
 * │ 0xAA 55  │ 1B   │ 1B     │ 0..240B     │ 1B   │
 * └──────────┴──────┴────────┴─────────────┴──────┘
 *
 * - START:   Two sync bytes (0xAA, 0x55) for frame detection.
 * - TYPE:    Identifies the payload schema (see telemetry_packet_type_t).
 * - LENGTH:  Number of bytes in PAYLOAD (0..TELEMETRY_MAX_PAYLOAD).
 * - PAYLOAD: Type-specific binary data, little-endian.
 * - CRC8:    CRC-8/MAXIM over TYPE + LENGTH + PAYLOAD.
 */

#ifndef SERIAL_TELEMETRY_PROTOCOL_H
#define SERIAL_TELEMETRY_PROTOCOL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── Framing constants ────────────────────────────────────────────────── */

#define TELEMETRY_START_BYTE_1 0xAA
#define TELEMETRY_START_BYTE_2 0x55
#define TELEMETRY_MAX_PAYLOAD 240
#define TELEMETRY_HEADER_SIZE 4 /* START(2) + TYPE(1) + LENGTH(1)  */
#define TELEMETRY_CRC_SIZE 1

/* ── Packet types ─────────────────────────────────────────────────────── */

typedef enum {
  TELEM_PKT_DASHBOARD = 0x01, /**< Core dashboard data             */
  TELEM_PKT_TIMING = 0x02,    /**< Lap / sector timing             */
  TELEM_PKT_HEARTBEAT = 0xFE, /**< Keep-alive, zero-length payload */
} telemetry_packet_type_t;

/* ── Payload: Dashboard (TELEM_PKT_DASHBOARD) ─────────────────────────
 * All multi-byte fields are **little-endian**.                          */

typedef struct __attribute__((packed)) {
  uint16_t rpm;         /**< Engine RPM (0 – 20 000)               */
  uint16_t max_rpm;     /**< Max RPM / red-line                    */
  uint16_t speed_kph;   /**< Vehicle speed in km/h                 */
  int8_t gear;          /**< Gear: -1 = R, 0 = N, 1..8 = forward  */
  uint8_t throttle_pct; /**< Throttle position   0..100            */
  uint8_t brake_pct;    /**< Brake pressure      0..100            */
  uint8_t clutch_pct;   /**< Clutch position     0..100            */
  int16_t oil_temp_c;   /**< Oil temperature °C                    */
  int16_t water_temp_c; /**< Water / coolant temperature °C        */
  uint16_t fuel_pct;    /**< Fuel remaining 0..1000 (× 0.1 %)     */
  uint8_t flags;        /**< Bit flags (see TELEM_FLAG_*)          */
} telemetry_dashboard_t;

/* Dashboard flags */
#define TELEM_FLAG_PIT_LIMITER (1u << 0)
#define TELEM_FLAG_ABS_ACTIVE (1u << 1)
#define TELEM_FLAG_TC_ACTIVE (1u << 2)
#define TELEM_FLAG_DRS_ENABLED (1u << 3)
#define TELEM_FLAG_DRS_ACTIVE (1u << 4)
#define TELEM_FLAG_HEADLIGHTS (1u << 5)

/* ── Payload: Timing (TELEM_PKT_TIMING) ──────────────────────────────── */

typedef struct __attribute__((packed)) {
  uint32_t lap_time_ms; /**< Current lap time in ms            */
  uint32_t best_lap_ms; /**< Best lap time in ms               */
  uint32_t last_lap_ms; /**< Last completed lap time in ms     */
  uint16_t current_lap; /**< Current lap number                */
  uint16_t total_laps;  /**< Total laps (0 = unlimited)        */
  int16_t position;     /**< Race position                     */
  int16_t total_cars;   /**< Total cars in session             */
} telemetry_timing_t;

/* ── Raw packet structure (used internally by parser) ─────────────────── */

typedef struct {
  uint8_t type;
  uint8_t length;
  uint8_t payload[TELEMETRY_MAX_PAYLOAD];
  uint8_t crc;
} telemetry_raw_packet_t;

/* ── CRC-8/MAXIM ─────────────────────────────────────────────────────── */

/**
 * @brief Compute CRC-8/MAXIM (poly 0x31, init 0x00, no reflect/xor).
 * @param data  Pointer to byte buffer.
 * @param len   Number of bytes.
 * @return      CRC-8 value.
 */
static inline uint8_t telemetry_crc8(const uint8_t *data, uint8_t len) {
  uint8_t crc = 0x00;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x80) {
        crc = (crc << 1) ^ 0x31;
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_TELEMETRY_PROTOCOL_H */