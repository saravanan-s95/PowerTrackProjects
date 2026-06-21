/*
 * tftp.c - Serialization and parsing implementation
 */
#include <string.h>
#include <arpa/inet.h>
#include "tftp.h"

static packetbuffer_t packet_buf[PACKETSIZE];

int packet_parse(packetbuffer_t *buf, int len, packet_t *pkt)
{
    if (len < 2) return -1;
    
    opcode_t op;
    memcpy(&op, buf, 2);
    pkt->opcode = ntohs(op);

    if (pkt->opcode == OPCODE_RRQ || pkt->opcode == OPCODE_WRQ)
    {
        int p = 2;
        int f_len = 0;
        while (p < len && buf[p] != '\0')
        {
            pkt->filename[f_len++] = buf[p++];
        }
        pkt->filename[f_len] = '\0';
        p++; // skip null
        
        int m_len = 0;
        while (p < len && buf[p] != '\0')
        {
            pkt->mode[m_len++] = buf[p++];
        }
        pkt->mode[m_len] = '\0';
        return 0;
    }
    else if (pkt->opcode == OPCODE_DATA)
    {
        if (len < 4) return -1;
        bnum_t net_block;
        memcpy(&net_block, buf + 2, 2);
        pkt->blocknum = ntohs(net_block);
        pkt->data_length = len - 4;
        if (pkt->data_length > 0)
        {
            memcpy(pkt->data, buf + 4, pkt->data_length);
        }
        return 0;
    }
    else if (pkt->opcode == OPCODE_ACK)
    {
        if (len < 4) return -1;
        bnum_t net_block;
        memcpy(&net_block, buf + 2, 2);
        pkt->blocknum = ntohs(net_block);
        return 0;
    }
    else if (pkt->opcode == OPCODE_ERROR)
    {
        if (len < 4) return -1;
        ecode_t ec;
        memcpy(&ec, buf + 2, 2);
        pkt->ecode = ntohs(ec);
        int e_len = 0;
        int p = 4;
        while (p < len && buf[p] != '\0')
        {
            pkt->estring[e_len++] = buf[p++];
        }
        pkt->estring[e_len] = '\0';
        return 0;
    }
    return -1;
}

void packet_form_rrq(char *filename, char *mode, packetbuffer_t **buf, int *len)
{
    opcode_t op = htons(OPCODE_RRQ);
    packetbuffer_t *ptr = packet_buf;
    memcpy(ptr, &op, 2); ptr += 2;
    strcpy(ptr, filename); ptr += strlen(filename) + 1;
    strcpy(ptr, mode); ptr += strlen(mode) + 1;
    *len = ptr - packet_buf;
    *buf = packet_buf;
}

void packet_form_wrq(char *filename, char *mode, packetbuffer_t **buf, int *len)
{
    opcode_t op = htons(OPCODE_WRQ);
    packetbuffer_t *ptr = packet_buf;
    memcpy(ptr, &op, 2); ptr += 2;
    strcpy(ptr, filename); ptr += strlen(filename) + 1;
    strcpy(ptr, mode); ptr += strlen(mode) + 1;
    *len = ptr - packet_buf;
    *buf = packet_buf;
}

void packet_form_data(bnum_t blocknum, char *data, int data_len, packetbuffer_t **buf, int *len)
{
    opcode_t op = htons(OPCODE_DATA);
    bnum_t block = htons(blocknum);
    packetbuffer_t *ptr = packet_buf;
    memcpy(ptr, &op, 2); ptr += 2;
    memcpy(ptr, &block, 2); ptr += 2;
    if (data_len > 0)
    {
        memcpy(ptr, data, data_len); ptr += data_len;
    }
    *len = ptr - packet_buf;
    *buf = packet_buf;
}

void packet_form_ack(bnum_t blocknum, packetbuffer_t **buf, int *len)
{
    opcode_t op = htons(OPCODE_ACK);
    bnum_t block = htons(blocknum);
    packetbuffer_t *ptr = packet_buf;
    memcpy(ptr, &op, 2); ptr += 2;
    memcpy(ptr, &block, 2); ptr += 2;
    *len = ptr - packet_buf;
    *buf = packet_buf;
}

void packet_form_error(ecode_t ecode, const char *msg, packetbuffer_t **buf, int *len)
{
    opcode_t op = htons(OPCODE_ERROR);
    ecode_t ec = htons(ecode);
    packetbuffer_t *ptr = packet_buf;
    memcpy(ptr, &op, 2); ptr += 2;
    memcpy(ptr, &ec, 2); ptr += 2;
    strcpy(ptr, msg); ptr += strlen(msg) + 1;
    *len = ptr - packet_buf;
    *buf = packet_buf;
}