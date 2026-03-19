/**
 * @file telemetry_packer.h
 * @brief Curation, preprocessing and packaging of ACC telemetry data
 *        into serial telemetry protocol packets.
 *
 * Pipeline:  SPageFilePhysics ──▶ curate ──▶ preprocess ──▶ pack ──▶ byte[]
 */

#ifndef TELEMETRY_PACKER_H
#define TELEMETRY_PACKER_H

#include <cstddef>
#include "acc_shmem.h"
#include "serial_telemetry_protocol.h"

/* Maximum size of a fully framed packet:
   HEADER(4) + MAX_PAYLOAD(240) + CRC(1) = 245                              */
#define TELEMETRY_MAX_FRAME_SIZE \
    (TELEMETRY_HEADER_SIZE + TELEMETRY_MAX_PAYLOAD + TELEMETRY_CRC_SIZE)

/* ── Curation + Preprocessing ────────────────────────────────────────────── */

/**
 * @brief Curate and preprocess physics shared-memory data into a
 *        dashboard payload ready for serialization.
 *
 * Conversions applied:
 *   - float 0..1 inputs  →  uint8_t 0..100   (throttle, brake, clutch)
 *   - float speedKmh     →  uint16_t clamped  (0..500 km/h)
 *   - ACC gear encoding  →  protocol gear     (ACC 0=R → proto -1=R, etc.)
 *   - boolean flags      →  bitfield          (pit limiter, ABS, TC)
 *
 * @param physics  Pointer to the shared-memory physics page.
 * @return         Populated telemetry_dashboard_t struct.
 */
telemetry_dashboard_t curate_dashboard(const SPageFilePhysics* physics);

/* ── Packaging / Framing ─────────────────────────────────────────────────── */

/**
 * @brief Frame an arbitrary payload into a serial telemetry packet.
 *
 * Writes the full frame [START1][START2][TYPE][LEN][PAYLOAD...][CRC8]
 * into @p out_buf and sets @p out_len to the total number of bytes written.
 *
 * @param type         Packet type identifier.
 * @param payload      Pointer to payload bytes (may be NULL if payload_len == 0).
 * @param payload_len  Number of payload bytes (0..TELEMETRY_MAX_PAYLOAD).
 * @param out_buf      Destination buffer (must be >= TELEMETRY_MAX_FRAME_SIZE).
 * @param out_len      [out] Total frame length written.
 * @return             true on success, false if payload_len exceeds maximum.
 */
bool pack_packet(telemetry_packet_type_t type,
                 const void* payload,
                 uint8_t payload_len,
                 uint8_t* out_buf,
                 size_t* out_len);

/**
 * @brief Convenience: curate + pack a dashboard packet in one call.
 *
 * @param physics  Pointer to the shared-memory physics page.
 * @param out_buf  Destination buffer (>= TELEMETRY_MAX_FRAME_SIZE).
 * @param out_len  [out] Total frame length written.
 * @return         true on success.
 */
bool build_dashboard_packet(const SPageFilePhysics* physics,
                            uint8_t* out_buf,
                            size_t* out_len);

/**
 * @brief Build a zero-payload heartbeat packet.
 *
 * @param out_buf  Destination buffer (>= TELEMETRY_MAX_FRAME_SIZE).
 * @param out_len  [out] Total frame length written.
 * @return         true on success.
 */
bool build_heartbeat_packet(uint8_t* out_buf, size_t* out_len);

#endif /* TELEMETRY_PACKER_H */
