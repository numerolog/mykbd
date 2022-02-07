#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

struct lightnow {
	char l1;
	char l2;
	char l3;
};

int main(void) {
	printf("Hello World!\n");
	int fd = open("/dev/kbl", O_RDWR);
	if (fd < 0) {
		printf("open errored=%d!\n", fd);
		goto errored;
	}
	struct lightnow mylight;
	mylight.l1 = 0;
	mylight.l2 = 1;
	mylight.l3 = 0;
	int error;
	if ((error = ioctl(fd, 123, &mylight)) < 0) {
		printf("ioctl errored=%d!\n", error);
		goto errored;
	} else {
		printf("ioctl ok!\n");
	}
	
errored:
	if (fd >= 0)
		close(fd);
		
	return 0;
}
