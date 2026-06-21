/*
 * tftp.h - Protocol constants and packet structures
 */
#ifndef TFTP_H
#define TFTP_H

#define BLIMIT      512
#define PACKETSIZE  516

typedef unsigned short opcode_t;
#define OPCODE_RRQ   1
#define OPCODE_WRQ   2
#define OPCODE_DATA  3
#define OPCODE_ACK   4
#define OPCODE_ERROR 5

#define IS_RRQ(op)   ((op) == OPCODE_RRQ)
#define IS_WRQ(op)   ((op) == OPCODE_WRQ)
#define IS_DATA(op)  ((op) == OPCODE_DATA)
#define IS_ACK(op)   ((op) == OPCODE_ACK)
#define IS_ERROR(op) ((op) == OPCODE_ERROR)

typedef unsigned short ecode_t;
#define ECODE_0     0
#define ECODE_1     1
#define ECODE_2     2
#define ECODE_3     3
#define ECODE_4     4
#define ECODE_5     5
#define ECODE_6     6
#define ECODE_7     7

#define ESTRING_0   "Not defined, see error message"
#define ESTRING_1   "File not found."
#define ESTRING_2   "Access violation."
#define ESTRING_3   "Disk full or allocation exceeded."
#define ESTRING_4   "Illegal TFTP operation."
#define ESTRING_5   "Unknown transfer ID."
#define ESTRING_6   "File already exists."
#define ESTRING_7   "File already exists / No such user."

#define MODE_NETASCII "netascii"
#define MODE_OCTET    "octet"

#define TIMEOUT         2
#define TIMEOUT_LIMIT   5

typedef unsigned short bnum_t;
typedef char packetbuffer_t;

typedef struct tftp_packet
{
    char filename[PACKETSIZE];
    opcode_t opcode;
    char mode[PACKETSIZE];
    char data[BLIMIT];
    int data_length;
    bnum_t blocknum;
    ecode_t ecode;
    char estring[PACKETSIZE];
} packet_t;

int packet_parse(packetbuffer_t *buf, int len, packet_t *pkt);
void packet_form_rrq(char *filename, char *mode, packetbuffer_t **buf, int *len);
void packet_form_wrq(char *filename, char *mode, packetbuffer_t **buf, int *len);
void packet_form_data(bnum_t blocknum, char *data, int data_len, packetbuffer_t **buf, int *len);
void packet_form_ack(bnum_t blocknum, packetbuffer_t **buf, int *len);
void packet_form_error(ecode_t ecode, const char *msg, packetbuffer_t **buf, int *len);

#endif