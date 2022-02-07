obj-m += my_kbd.o
KDIR ?= /lib/modules/$(shell uname -r)/build
all:
	$(MAKE) -C $(KDIR) M=$(PWD) V=1 modules
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) V=1 clean
