#include "can.h"

void init_CAN(void){
	MCP2515_init_loopback();
}



void can_send(message_ptr message) {
	// Alt her foregår med buffer 0

	 //Id. TXBnSIDH og TXBnSIDL
	// MCP2515_write_reg(MCP_TXB0SIDH, message->id / 8); // De åtte høyeste bitene i iden.
	// MCP2515_write_reg(MCP_TXB0SIDL, (message->id % 8) << 5); // De tre laveste bitene i iden.



	MCP2515_write_reg(MCP_TXB0SIDH, (message->id >> 3));
	MCP2515_write_reg(MCP_TXB0SIDL, (message->id & 0b111));

	 //Lengde. TXBnDLC
	MCP2515_write_reg(MCP_TXB0DLC, message->length);

	 //Melding. TXBnDm
	for (int i = 0; i < message->length; i++) {
		MCP2515_write_reg(MCP_TXB0D0 + i, message->data[i]);
	}
	MCP2515_request_send(0);
}

message_t can_receive() {
	// Alt her foregår med buffer 0
	message_t message = {};

	uint8_t id_low = MCP2515_read_register(MCP_RXB0SIDL) >> 5; // Shift right by 5 to get the lower 3 bits of the ID
	uint8_t id_high = MCP2515_read_register(MCP_RXB0SIDH);
	message.id = (id_high << 3) | id_low;
	printf("high: 0x%x, low: 0x%x\n", id_high,id_high);
	// Lengde. RXBnDLC
	message.length = MCP2515_read_register(MCP_RXB0DLC);

	// Melding. RXBnDM
	for (int i = 0; i < message.length; i++) {
		message.data[i] = MCP2515_read_register(MCP_RXB0D0 + i);
	}

	return message;
}
