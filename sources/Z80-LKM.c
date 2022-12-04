#include <Z80.h>
#include <linux/module.h>


static int __init z80_init(void)
	{
	printk(KERN_INFO "Zilog Z80 CPU emulator v"Z80_LIBRARY_VERSION_STRING"\n");
	return 0;
	}


static void __exit z80_exit(void)
	{}


module_init(z80_init);
module_exit(z80_exit);

EXPORT_SYMBOL(z80_power);
EXPORT_SYMBOL(z80_instant_reset);
EXPORT_SYMBOL(z80_int);
EXPORT_SYMBOL(z80_nmi);
EXPORT_SYMBOL(z80_execute);
EXPORT_SYMBOL(z80_run);

MODULE_LICENSE("GPL v3");
MODULE_DESCRIPTION("Zilog Z80 CPU emulator");
MODULE_AUTHOR("Manuel Sainz de Baranda y Goñi <manuel@zxe.io>");
