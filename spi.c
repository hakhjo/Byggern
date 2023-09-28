#include "spi.h"
// Define the CS pin for MCP2515
#define MCP2515_CS_PORT PORTB
#define MCP2515_CS_PIN  PB4
#define SPI_READ_COMMAND 0x03
#define SPI_RESET_COMMAND 0xC0


#define MCP_RESET        0xC0
#define MCP_WRITE        0x02
#define MCP_READ         0x03

//-------
#define MCP_LOOPBACK     0x40  // Mode setting for loopback
#define MCP_CANCTRL      0x0F //adress for can control register
#define MCP_READ_STATUS_CMD 0xA0 //read_status

//request to send
#define MCP_RTS_TX0 0x81
#define MCP_RTS_TX1 0x82
#define MCP_RTS_TX2 0x84

//configs for init
#define MCP_CNF3         0x28
#define MCP_CNF2         0x29
#define MCP_CNF1         0x2A

// Bit modify
#define MCP_BM 0x05



void init_spi(void){
    /* Set MOSI and SCK and SS output, all others input */
    DDRB |= (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
    // Enable SPI, Master, set clock rate fck/16(SPRO) and setting SPIMODE = 0 since CPOL and CPHA are set to 0
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

}

// Send a reset command to MCP2515
void MCP2515_Reset() {
	MCP2515_Select();
	SPI_Transmit(MCP_RESET);
	MCP2515_Deselect();
	_delay_ms(10); // Allow some time for the reset to complete
}

void SPI_Transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}
// Chip Select (CS) for MCP2515
void MCP2515_Select(void) {
	MCP2515_CS_PORT &= ~(1 << MCP2515_CS_PIN);
}

// Chip Deselect (CS) for MCP2515
void MCP2515_Deselect(void) {
	MCP2515_CS_PORT |= (1 << MCP2515_CS_PIN);
}
uint8_t SPI_Receive(void) {
	// Send dummy byte to initiate reception
	SPI_Transmit(0xFF);
	
	// Wait for reception to complete
	//while (!(SPSR & (1 << SPIF)));
	
	return SPDR;
}

uint8_t readMCP2515Register(uint8_t address) {
	uint8_t receivedData;

    MCP2515_Select();

	SPI_Transmit(SPI_READ_COMMAND);  // Send the READ command
	SPI_Transmit(address);       // Send the register address
	receivedData = SPI_Receive();// Read the data
    MCP2515_Deselect();


	return receivedData;
}

uint8_t MCP2515_ReadStatus() {
	uint8_t status;
	
	MCP2515_Select();
	SPI_Transmit(MCP_READ_STATUS_CMD);
	status = SPI_Receive();
	MCP2515_Deselect();
	
	return status;
}
// Function to request to send from a specific buffer
void MCP2515_RequestToSend(uint8_t bufferNum) {
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

	MCP2515_Select();
	SPI_Transmit(command);
	MCP2515_Deselect();
}

void MCP2515_bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data) {
	MCP2515_Select();
	SPI_Transmit(MCP_BM);
	SPI_Transmit(address);
	SPI_Transmit(mask_byte);
	SPI_Transmit(data);
	MCP2515_Deselect();
}

// Write to MCP2515 register
void MCP2515_WriteRegister(uint8_t address, uint8_t data) {
	MCP2515_Select();
	SPI_Transmit(MCP_WRITE);
	SPI_Transmit(address);
	SPI_Transmit(data);
	MCP2515_Deselect();
}
 //Initialize MCP2515
void MCP2515_Init() {
	MCP2515_Reset();
	
	 //Set bit timing (assuming 16 MHz crystal and 500 kbps CAN speed)
	MCP2515_WriteRegister(MCP_CNF1, 0x00);
	MCP2515_WriteRegister(MCP_CNF2, 0x90);
	MCP2515_WriteRegister(MCP_CNF3, 0x82);
	
	 //Set normal mode
	MCP2515_WriteRegister(MCP_CANCTRL, 0x00);
}

// Initialize MCP2515 for loopback mode
void MCP2515_Init_Loopback() {
	MCP2515_Reset();
	
	 //Set bit timing (assuming 16 MHz crystal and desired CAN speed)
	 //If you're not connecting to a real CAN bus, these values are less critical, but they still need to be set.
	MCP2515_WriteRegister(MCP_CNF1, 0x00);
	MCP2515_WriteRegister(MCP_CNF2, 0x90);
	MCP2515_WriteRegister(MCP_CNF3, 0x82);
	
	 //Set loopback mode
	MCP2515_WriteRegister(MCP_CANCTRL, MCP_LOOPBACK);
}
