/* 
 *	RAMdisc driver
 */

#include <kernel.h>
#include <kdata.h>
#include <printf.h>
#include <devrd.h>

static uint8_t rd_present = 2;		/* Unknown */

/* Hooks to asm interface */
extern uint8_t rd_page;
extern uint8_t *rd_dptr;
extern uint16_t rd_block;
extern uint8_t rd_port;

extern void ramread(void);
extern void ramwrite(void);

static int rd_transfer(uint_fast8_t minor, bool is_read, uint_fast8_t rawflag)
{
    unsigned ct = 0, num;
    void (*op)(void);

    if(rawflag == 1) {
        if (d_blkoff(BLKSHIFT))
            return -1;
    }

    rd_port = minor ? 0x24 : 0x20;
    rd_page = rawflag;
    rd_dptr = udata.u_dptr;
    rd_block = udata.u_block;

    num = udata.u_nblock;

    if (is_read)
        op = ramread;
    else
        op = ramwrite;

    /* We must be careful here. Our stack on a swap is private but we are
       going to overwrite udata in some cases */
    while (ct < num) {
        if (rd_block > 511)
            break;
        op();
        /* rd_dptr is advanced by the asm helpers */
        rd_block++;
        ct++;
    }
    return ct << BLKSHIFT;
}

int rd_open(uint_fast8_t minor, uint16_t flag)
{
    flag;
    /* Cheaper to add than shift for the 0/1 to 1/2 case only */
    if(minor > 1 || !(rd_present & (minor + 1))) {
        udata.u_error = ENODEV;
        return -1;
    }
    return 0;
}

int rd_read(uint_fast8_t minor, uint_fast8_t rawflag, uint_fast8_t flag)
{
    flag;
    return rd_transfer(minor, true, rawflag);
}

int rd_write(uint_fast8_t minor, uint_fast8_t rawflag, uint_fast8_t flag)
{
    flag;
    return rd_transfer(minor, false, rawflag);
}

void rd_probe(void)
{
    rd_present = 3;	/* for now */
    if (rd_present)
        kputs("RAMdisc found at 0x20.\n");
    if (rd_present & 2)
        kputs("RAMdisc found at 0x24.\n");
}
