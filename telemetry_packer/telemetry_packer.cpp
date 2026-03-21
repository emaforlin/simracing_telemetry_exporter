/**
 * @file telemetry_packer.cpp
 * @brief Implementation of data curation, preprocessing, and packaging.
 */

#include "telemetry_packer.h"
#include <cmath>
#include <cstring>

/* ── Helpers ─────────────────────────────────────────────────────────────── */

/** Clamp a value to [lo, hi]. */
template <typename T>
static inline T clamp(T value, T lo, T hi) {
    return (value < lo) ? lo : (value > hi) ? hi : value;
}

/** Convert a float in [0.0, 1.0] to a uint8_t percentage [0, 100]. */
static inline uint8_t float_to_pct(float v) {
    return static_cast<uint8_t>(clamp(static_cast<int>(std::round(v * 100.0f)), 0, 100));
}

/* ── Curation + Preprocessing ────────────────────────────────────────────── */

telemetry_dashboard_t curate_dashboard(const SPageFilePhysics* physics) {
    telemetry_dashboard_t dash;
    std::memset(&dash, 0, sizeof(dash));

    /* ── RPM ─────────────────────────────────────────────────────────────── */
    dash.rpm = static_cast<uint16_t>(clamp(physics->rpm, 0, 20000));

    /* max_rpm is not available in physics shared memory                     */
    dash.max_rpm = 0;

    /* ── Speed ───────────────────────────────────────────────────────────── */
    dash.speed_kph = static_cast<uint16_t>(
        clamp(static_cast<int>(std::round(physics->speedKmh)), 0, 500));

    /* ── Gear ────────────────────────────────────────────────────────────── *
     * ACC encoding:   0 = Reverse, 1 = Neutral, 2+ = Forward gears        *
     * Protocol encoding: -1 = R, 0 = N, 1..8 = Forward gears              */
    dash.gear = static_cast<int8_t>(physics->gear - 1);

    /* ── Pedals (float 0..1  →  uint8_t 0..100) ─────────────────────────── */
    dash.throttle_pct = float_to_pct(physics->gas);
    dash.brake_pct    = float_to_pct(physics->brake);
    dash.clutch_pct   = float_to_pct(physics->clutch);

    /* ── Temperatures ────────────────────────────────────────────────────── */
    dash.oil_temp_c   = 0; /* not available in physics shmem */
    dash.water_temp_c = static_cast<int16_t>(std::round(physics->waterTemp));

    /* ── Fuel ────────────────────────────────────────────────────────────── *
     * Protocol stores fuel as 0..1000 (× 0.1 %).                          *
     * Without max-fuel info we store raw kg × 10 as a practical proxy.     */
    dash.fuel_pct = static_cast<uint16_t>(
        clamp(static_cast<int>(std::round(physics->fuel * 10.0f)), 0, 65535));

    /* ── Flags ───────────────────────────────────────────────────────────── */
    uint8_t flags = 0;
    if (physics->pitLimiterOn) flags |= TELEM_FLAG_PIT_LIMITER;
    if (physics->abs > 0.0f)   flags |= TELEM_FLAG_ABS_ACTIVE;
    if (physics->tc  > 0.0f)   flags |= TELEM_FLAG_TC_ACTIVE;
    dash.flags = flags;

    return dash;
}

/* ── Packaging / Framing ─────────────────────────────────────────────────── */

bool pack_packet(telemetry_packet_type_t type,
                 const void* payload,
                 uint8_t payload_len,
                 uint8_t* out_buf,
                 size_t* out_len)
{
    if (payload_len > TELEMETRY_MAX_PAYLOAD) {
        return false;
    }

    size_t idx = 0;

    /* ── Start bytes ─────────────────────────────────────────────────────── */
    out_buf[idx++] = TELEMETRY_START_BYTE_1;   /* 0xAA */
    out_buf[idx++] = TELEMETRY_START_BYTE_2;   /* 0x55 */

    /* ── Type + Length ───────────────────────────────────────────────────── */
    out_buf[idx++] = static_cast<uint8_t>(type);
    out_buf[idx++] = payload_len;

    /* ── Payload ─────────────────────────────────────────────────────────── */
    if (payload_len > 0 && payload != nullptr) {
        std::memcpy(&out_buf[idx], payload, payload_len);
        idx += payload_len;
    }

    /* ── CRC-8/MAXIM over TYPE + LENGTH + PAYLOAD ────────────────────────── */
    /* CRC region starts at offset 2 (after start bytes), length = 2 + payload_len */
    uint8_t crc = telemetry_crc8(&out_buf[2], 2 + payload_len);
    out_buf[idx++] = crc;

    *out_len = idx;
    return true;
}

/* ── Convenience builders ────────────────────────────────────────────────── */

bool build_dashboard_packet(const SPageFilePhysics* physics,
                            uint8_t* out_buf,
                            size_t* out_len)
{
    telemetry_dashboard_t dash = curate_dashboard(physics);
    return pack_packet(TELEM_PKT_DASHBOARD,
                       &dash,
                       static_cast<uint8_t>(sizeof(dash)),
                       out_buf,
                       out_len);
}

bool build_heartbeat_packet(uint8_t* out_buf, size_t* out_len) {
    return pack_packet(TELEM_PKT_HEARTBEAT, nullptr, 0, out_buf, out_len);
}
