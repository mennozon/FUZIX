int fd_open(uint8_t minor, uint16_t flag);
int fd_read(uint8_t minor, uint8_t rawflag, uint8_t flag);
int fd_write(uint8_t minor, uint8_t rawflag, uint8_t flag);
int fd_ioctl(uint8_t minor, uarg_t request, char *buffer);

#define fdbios_drive	(*((uint8_t *)0x6049))
#define fdbios_track 	(*((uint16_t *)0x604A))
#define fdbios_sector	(*((uint8_t *)0x604C))
#define fdbios_addr	(*((uint8_t *)0x604E))
#define fdbios_floppy	(*((uint8_t *)0x6080))

