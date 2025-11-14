#include <cstddef>
#include <stddef.h>
#include <stdint.h>
typedef struct {
    unsigned char *items;
    size_t count;
    size_t capacity;
} Cws_Payload_Buffer;

typedef enum {
    CWS_OPCODE_CONT = 0x0,
    CWS_OPCODE_TEXT = 0x1,
    CWS_OPCODE_BIN = 0x2,
    CWS_OPCODE_CLOSE = 0x8,
    CWS_OPCODE_PING = 0x9,
    CWS_OPCODE_PONG = 0xA
} Cws_Opcode;

typedef struct {
    bool fin, rsv1, rsv2, rsv3;
    Cws_Opcode opcode;
    bool masked;
    size_t payload_len;
    uint8_t mask[4];
} Cws_Frame_Header;

#define CHUNK_SIZE 1024

#define CWS_FIN(header) (((header)[0] >> 7)&0x1);
#define CWS_RSV1(header) (((header)[0] >> 6)&0x1);
#define CWS_RSV2(header) (((header)[0] >> 5)&0x1);
#define CWS_RSV3(header) (((header)[0] >> 4)&0x1);
#define CWS_OPCODE(header) ((header)[0] & 0xF);
#define CWS_MASK(header) ((header)[1] >> 7);
#define CWS_PAYLOAD_LEN(header) ((header)[1] & 0x7F);

typedef enum {
    CWS_SHUTDOWN_READ,
    CWS_SHUTDOWN_WRITE,
    CWS_SHUTDOWN_BOTH,
} Cws_Shutdown_How;

typedef enum {
    CWS_OK = 0,
    CWS_ERROR_ERRNO = -1,
} Cws_Error;

typedef struct {
    void *data;
    int (*read)(void *data, void *buffer, size_t len);
    int (*peek)(void *data, const void *buffer, size_t len);
    int (*shutdown)(void *data, Cws_Shutdown_How how);
    int (*close)(void *data, Cws_Shutdown_How how);
} Cws_Socket;

typedef struct {
    Cws_Socket socket;
    bool debug;
    bool client;
} Cws;
