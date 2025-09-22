# SmartTV Protocol v1

This document defines the **application-layer protocol** used between Smart TV servers and Smart Remote clients.

---

## States
- **OFF** (default)  
  - Only `ON` command is accepted.  
- **ON**  
  - Channel 1 is active by default.  
  - TV remembers previous channel when turned off (unless the server restarts).  

---

## Client → Server Commands
- `ON`
  Turn the TV on (only valid if currently OFF).  

- `OFF`
  Turn the TV off.

- `GET_CHANNEL`
  Query the current active channel.

- `GET_CHANNELS`
  Query the number of available channels.

- `SET_CHANNEL <n>`
  Set the TV to channel `<n>` (valid range: `1..C`).
  
---

## Server → Client Responses
- `OK`
  Generic success response.

- `ERROR <reason>`
  Error response with explanation. Examples:
  - `ERROR invalid_channel`
  - `ERROR tv_off`

- `CHANNEL <n>`
  Reports the current active channel.

- `CHANNELS <c>`
  Reports the total number of available channels.

---

## Examples

### Turn on the TV
```bash
Client: ON
Server: OK
```

### Query current channel
```bash
Client: GET_CHANNEL
Server: CHANNEL 1
```

### Change to channel 5
```bash
Client SET_CHANNEL 5
Server: OK
```

### Invalid channel
```bash
Client: SET_CHANNEL 99
Server: ERROR invalid_channel
```

---

## Notes
- All messages are sent as **text lines** terminated by `\n`
- Commands and responses are **case-insensitive** (`ON` = `on`)
- Protocol can be easily extended with new commands (e.g. `MUTE`, `VOLUME <n>`)
