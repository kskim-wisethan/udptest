#include "sim_cmd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>


Sim_Cmd::Sim_Cmd(const char* data, int size)
    : m_length(0)
{
    memset(m_cmd, 0, CMD_BUFLEN);
    memcpy(m_cmd, data, size);
}

Sim_Cmd::Sim_Cmd(unsigned int message_id, unsigned int message_length, unsigned int start_bit, unsigned int bit_size, char value)
    : m_length(0)
{
    memset(m_cmd, 0, CMD_BUFLEN);

    memcpy(m_cmd, &message_id, 4);
    memcpy(m_cmd + 4, &message_length, 4);

    char databuf[message_length + 1] = {0, }; 

    int division = (int)(start_bit / 8); 
    int remain = (int)(start_bit % 8);

    databuf[division] = value << remain;
    memcpy(m_cmd + 8, databuf, message_length);

    m_length = message_length + 8;
}

Sim_Cmd::~Sim_Cmd()
{
    memset(m_cmd, 0, CMD_BUFLEN);
}

int Sim_Cmd::getId()
{
    int ret = 0;
    memcpy(&ret, m_cmd, 4);

    return ret;
}

int Sim_Cmd::getLength()
{
    int ret = 0;
    memcpy(&ret, m_cmd + 4, 4);

    return ret;
}

char* Sim_Cmd::getValue()
{
    char* valuebuf = (char*)malloc((m_length + 1) * sizeof(char)); 
    memcpy(valuebuf, m_cmd + 8, m_length);

    return valuebuf;
}

void Sim_Cmd::setValue(unsigned int start_bit, unsigned int bit_size, char value)
{
    int division = (int)(start_bit / 8); 
    int remain = (int)(start_bit % 8);

    m_cmd[division + 8] |= value << remain;
}
