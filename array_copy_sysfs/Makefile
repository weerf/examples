
ifneq ($(KERNELRELEASE),)

obj-m := array_copy.o

else

KERNELDIR ?= /root/Downloads/linux-3.14.28

PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	@cp array_copy.ko /exports/

endif

clean:
	@rm -f *.o
	@rm -f *.ko
	@rm -f .*.cmd
	@rm -rf .tmp_versions/
	@rm -f Module.symvers
	@rm -f *.mod.c
	@rm -f modules.order