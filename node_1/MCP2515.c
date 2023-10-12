#include "MCP2515.h"


// Send a reset command to MCP2515
void MCP2515_reset(void) {
	MCP2515_select();
	SPI_transmit(MCP_RESET);
	MCP2515_deselect();
	_delay_ms(10); // Allow some time for the reset to complete
}

// Chip Select (CS) for MCP2515
void MCP2515_select(void) {
	MCP2515_CS_PORT &= ~(1 << MCP2515_CS_PIN);
}

// Chip Deselect (CS) for MCP2515
void MCP2515_deselect(void) {
	MCP2515_CS_PORT |= (1 << MCP2515_CS_PIN);
}

uint8_t MCP2515_read_register(uint8_t address) {
	uint8_t received_data;
	MCP2515_select();

	SPI_transmit(SPI_READ_COMMAND);  // Send the READ command
	SPI_transmit(address);       // Send the register address
	received_data = SPI_receive();// Read the data
	MCP2515_deselect();
	return received_data;
}

uint8_t MCP2515_read_status(void) {
	uint8_t status;
	
	MCP2515_select();
	SPI_transmit(MCP_READ_STATUS_CMD);
	status = SPI_receive();
	MCP2515_deselect();
	
	return status;
}

// Function to request to send from a specific buffer
void MCP2515_request_send(uint8_t bufferNum) {
	uint8_t command;

	switch (bufferNum) {
		case 0:
		command = MCP_RTS_TX0;
		break;
		case 1:
		command = MCP_RTS_TX1;
		break;
		case 2:
		command = MCP_RTS_TX2;
		break;
		default:
		return; // Invalid buffer number
	}

	MCP2515_select();
	SPI_transmit(command);
	MCP2515_deselect();
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data) {
	MCP2515_select();
	SPI_transmit(MCP_BM);
	SPI_transmit(address);
	SPI_transmit(mask_byte);
	SPI_transmit(data);
	MCP2515_deselect();
}

// Write to MCP2515 register
void MCP2515_write_reg(uint8_t address, uint8_t data) {
	MCP2515_select();
	SPI_transmit(MCP_WRITE);
	SPI_transmit(address);
	SPI_transmit(data);
	MCP2515_deselect();
}
//Initialize MCP2515
void MCP2515_init() {
	MCP2515_reset();
	
	const uint8_t sjw = 0;	   // sync jump width length 1 x T_q
	const uint8_t btlmode = 1; // PS2 bit time length
	const uint32_t Fq = 500;		   // baud rate prescaler, 500kHz
	const uint8_t prseg = 2;   // propagation segment length 
	const uint8_t phseg1 = 6;  // PS1 length
	const uint8_t phseg2 = 7;  // PS2 length
	const uint8_t brp = 1;
	printf("brp %d \n\r", brp);
	mcp2515_write(MCP_CNF3, phseg2 -1); //not shiftet becaus phseg2 is the rightmost
	mcp2515_write(MCP_CNF2, (btlmode << 7) |  ((phseg1-1) <<3)| (prseg-1)); //set to ps2 length given in cnf3, set length of ps1 and there are 2 propagaton segment bits
	mcp2515_write(MCP_CNF1, (brp-1)); //since sjw = 0 i dont bother, and 
	
	//Set normal mode
	MCP2515_write_reg(MCP_CANCTRL, 0x00);
}

// Initialize MCP2515 for loopback mode
void MCP2515_init_loopback() {
	MCP2515_reset();
	
	//Set bit timing (assuming 16 MHz crystal and desired CAN speed)
	//If you're not connecting to a real CAN bus, these values are less critical, but they still need to be set.
	MCP2515_write_reg(MCP_CNF1, 0x00);
	MCP2515_write_reg(MCP_CNF2, 0x90);
	MCP2515_write_reg(MCP_CNF3, 0x82);
	
	//Set loopback mode
	MCP2515_write_reg(MCP_CANCTRL, MCP_LOOPBACK);
}
