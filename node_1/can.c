#include "can.h"

void init_CAN(void){
	MCP2515_init_loopback();
}



void can_send(message_ptr message) {
	// Alt her foregår med buffer 0

	 //Id. TXBnSIDH og TXBnSIDL
	// MCP2515_write_reg(MCP_TXB0SIDH, message->id / 8); // De åtte høyeste bitene i iden.
	// MCP2515_write_reg(MCP_TXB0SIDL, (message->id % 8) << 5); // De tre laveste bitene i iden.


	printf("message lowid 0x%x\n\r" ,  (message->id & 0b111)<<5);
	MCP2515_write_reg(MCP_TXB0SIDH, (message->id >> 3));
	MCP2515_write_reg(MCP_TXB0SIDL, (message->id & 0b111) <<5);

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

	uint8_t id_low = MCP2515_read_register(MCP_RXB0SIDL);
	uint8_t id_high = MCP2515_read_register(MCP_RXB0SIDH);
	message.id = (id_high << 3) | (id_low >>5);
	printf("high: 0x%x, low: 0x%x\n", id_high,id_low);
	// Lengde. RXBnDLC
	message.length = MCP2515_read_register(MCP_RXB0DLC);

	// Melding. RXBnDM
	for (int i = 0; i < message.length; i++) {
		message.data[i] = MCP2515_read_register(MCP_RXB0D0 + i);
	}

	return message;
}

void can_send_joystick_position(void){
	char c[8];
	c[0] = (char) joystick_position.x;
	c[1] = (char) joystick_position.y;
	
	message_t message;
	message.id = 1;
	message.length = 2;
	memcpy(message.data, c, 8);
	printf("Id %d, Datalength %d\n\r", message.id, message.length);
	for(int i = 0;i<message.length;i++){
		printf("%d ", message.data[i]);
	}
	can_send(&message);
}