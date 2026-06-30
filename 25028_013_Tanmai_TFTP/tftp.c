#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tftp.h"

/* Parse packet */
int packet_parse(packet_t *pkt)
{
    return pkt->opcode;
}

void packet_extract_opcode(packet_t *pkt)
{
    printf("Opcode : %d\n", pkt->opcode);
}

void packet_parse_rq(packet_t *pkt)
{
    printf("Filename : %s\n", pkt->filename);
    printf("Mode     : %s\n", pkt->mode);
}

void packet_parse_data(packet_t *pkt)
{
    printf("Data : %s\n", pkt->data);
}

void packet_parse_ack(packet_t *pkt)
{
    printf("ACK Received\n");
}

void packet_parse_error(packet_t *pkt)
{
    printf("Error : %s\n", pkt->estring);
}

/* Packet Formation */
packetbuffer_t *append_to_packet(packetbuffer_t *packet, char *data)
{
    strcat(packet, data);
    return packet;
}

void packet_form_rrq(packet_t *pkt, char *filename, char *mode)
{
    memset(pkt, 0, sizeof(packet_t));

    pkt->opcode = OPCODE_RRQ;

    strcpy(pkt->filename, filename);
    strcpy(pkt->mode, mode);
}

void packet_form_wrq(packet_t *pkt, char *filename, char *mode)
{
    memset(pkt, 0, sizeof(packet_t));

    pkt->opcode = OPCODE_WRQ;

    strcpy(pkt->filename, filename);
    strcpy(pkt->mode, mode);
}

void packet_form_data(packet_t *pkt, char *data)
{
    memset(pkt, 0, sizeof(packet_t));

    pkt->opcode = OPCODE_DATA;

    strcpy(pkt->data, data);
    pkt->data_length = strlen(data);
}

void packet_form_ack(packet_t *pkt)
{
    memset(pkt, 0, sizeof(packet_t));

    pkt->opcode = OPCODE_ACK;
}

void packet_form_error(packet_t *pkt, ecode_t code, char *msg)
{
    memset(pkt, 0, sizeof(packet_t));

    pkt->opcode = OPCODE_ERROR;

    pkt->ecode = code;

    strcpy(pkt->estring, msg);
    pkt->estring_length = strlen(msg);
}

void packet_free(packet_t *pkt)
{
    memset(pkt, 0, sizeof(packet_t));
}

/* Packet Receipt */
int packet_receive_rrq(packet_t *pkt)
{
    return (pkt->opcode == OPCODE_RRQ);
}

int packet_receive_wrq(packet_t *pkt)
{
    return (pkt->opcode == OPCODE_WRQ);
}

int packet_receive_data(packet_t *pkt)
{
    return (pkt->opcode == OPCODE_DATA);
}

int packet_receive_ack(packet_t *pkt)
{
    return (pkt->opcode == OPCODE_ACK);
}

int packet_receive_error(packet_t *pkt)
{
    return (pkt->opcode == OPCODE_ERROR);
}

int packet_receive_invalid(packet_t *pkt)
{
    return (pkt->opcode < 1 || pkt->opcode > 5);
}