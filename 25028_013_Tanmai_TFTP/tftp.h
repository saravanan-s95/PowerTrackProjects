#ifndef TFTP
#define TFTP

#define BLIMIT 512
#define PACKETSIZE 516

/* Opcode definitions */
typedef unsigned short opcode_t;

#define OPCODE_RRQ 1
#define OPCODE_WRQ 2
#define OPCODE_DATA 3
#define OPCODE_ACK 4
#define OPCODE_ERROR 5

#define IS_RRQ(op) ((op) == OPCODE_RRQ)
#define IS_WRQ(op) ((op) == OPCODE_WRQ)
#define IS_DATA(op) ((op) == OPCODE_DATA)
#define IS_ACK(op) ((op) == OPCODE_ACK)
#define IS_ERROR(op) ((op) == OPCODE_ERROR)

/* Error codes */
typedef unsigned short ecode_t;

#define ECODE_NONE 8

#define ECODE_0 0
#define ESTRING_0 "Not defined, see error message(if any)."

#define ECODE_1 1
#define ESTRING_1 "File not found."

#define ECODE_2 2
#define ESTRING_2 "Access violation."

#define ECODE_3 3
#define ESTRING_3 "Disk full or allocation exceeded."

#define ECODE_4 4
#define ESTRING_4 "Illegal TFTP operation."

#define ECODE_5 5
#define ESTRING_5 "Unknown transfer ID."

#define ECODE_6 6

#define ECODE_7 7
#define ESTRING_7 "No such user."

#define IS_ECODE_0(ec) ((ec) == ECODE_0)
#define IS_ECODE_1(ec) ((ec) == ECODE_1)
#define IS_ECODE_2(ec) ((ec) == ECODE_2)
#define IS_ECODE_3(ec) ((ec) == ECODE_3)
#define IS_ECODE_4(ec) ((ec) == ECODE_4)
#define IS_ECODE_5(ec) ((ec) == ECODE_5)
#define IS_ECODE_6(ec) ((ec) == ECODE_6)
#define IS_ECODE_7(ec) ((ec) == ECODE_7)

/* Transfer modes */
#define MODE_NETASCII "netascii"
#define MODE_OCTET "octet"
#define MODE_MAIL "mail"

/* Timeout */
#define TIMEOUT 1
#define TIMEOUT_LIMIT 10

/* Block number */
typedef unsigned short bnum_t;
bnum_t blocknum;
/* Packet buffer */
typedef char packetbuffer_t;

/* TFTP packet structure */
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
    int estring_length;

} packet_t;

/* Packet Parsing */
int packet_parse(packet_t *pkt);

void packet_extract_opcode(packet_t *pkt);
void packet_parse_rq(packet_t *pkt);
void packet_parse_data(packet_t *pkt);
void packet_parse_ack(packet_t *pkt);
void packet_parse_error(packet_t *pkt);

packetbuffer_t *append_to_packet(packetbuffer_t *packet, char *data);

void packet_form_rrq(packet_t *pkt, char *filename, char *mode);

void packet_form_wrq(packet_t *pkt, char *filename, char *mode);

void packet_form_data(packet_t *pkt, char *data);

void packet_form_error(packet_t *pkt, ecode_t code, char *msg);

void packet_form_ack(packet_t *pkt);

void packet_free(packet_t *pkt);

/* Packet Receipt */
int packet_receive_rrq();
int packet_receive_wrq();
int packet_receive_data();
int packet_receive_ack();
int packet_receive_error();
int packet_receive_invalid();

#endif