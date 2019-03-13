#ifndef SIM_CMD_H
#define SIM_CMD_H

#define CMD_BUFLEN		64

class Sim_Cmd
{
public:
	Sim_Cmd(const char* data, int size);
	Sim_Cmd(unsigned int message_id, unsigned int message_length, unsigned int start_bit, unsigned int bit_size, char value); 
	~Sim_Cmd();

	int getId();
	int getLength();
	char* getValue();
	void setValue(unsigned int start_bit, unsigned int bit_size, char value);

	int getCmdLength() { return m_length; } 
	char* getCmd() { return m_cmd; }

private:
	int m_length;
	char m_cmd[CMD_BUFLEN]; 
};

#endif // SIM_CMD_H
