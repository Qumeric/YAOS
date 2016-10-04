#ifndef __PIC_H__
#define __PIC_H__

// FIXME strange names?
#define IDT_MASTER_DESCRIPTOR1  32
#define IDT_MASTER_DESCRIPTOR2  39

#define IDT_SLAVE_DESCRIPTOR    34

#define IDT_SLAVE_DESCRIPTOR1   40
#define IDT_SLAVE_DESCRIPTOR2   47

void init_pic();

void send_master_command(uint8_t);
void send_master_data(uint8_t);
void send_slave_command(uint8_t);
void send_slave_data(uint8_t);

void eoi_master();
void eoi_slave();

#endif /*__PIC_H__*/
