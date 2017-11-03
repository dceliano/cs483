#include <stdint.h>

typedef struct BYUADDRESSTRACE
{
	uint32_t addr;
	uint8_t reqtype;
    uint8_t size;
    uint8_t attr;
    uint8_t proc;
	uint32_t time;
} p2AddrTr;

/* reqtype values */
#define FETCH			0x00	// instruction fetch
#define MEMREAD			0x01	// memory read
#define MEMREADINV		0x02	// memory read and invalidate
#define MEMWRITE		0x03	// memory write	

#define IOREAD			0x10	// IO read
#define IOWRITE			0x11	// IO Write

#define DEFERREPLY		0x20	// deferred reply
#define INTA			0x21	// interrupt acknowledge
#define CNTRLAGNTRES	0x22	// central agent response
#define BRTRACEREC		0x23	// branch trace record

#define SHUTDOWN		0x31	// shutdown
#define	FLUSH			0x32	// flush
#define HALT			0x33	// halt
#define	SYNC			0x34	// sync
#define FLUSHACK		0x35	// acknowledge flush
#define STOPCLKACK		0x36	// acknowledge stop clock
#define SMIACK			0x37	// acknowledge SMI mode
