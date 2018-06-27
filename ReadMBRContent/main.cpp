#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>

int main(void){
	static struct hd_driveid hd;	
	
	const int device = open("/dev/sda", O_RDONLY | O_NONBLOCK);
	if(device < 0){
		std::cerr<<"Cannot open device!\n";
		std::exit(EXIT_FAILURE);
	}

	if(!ioctl(device, HDIO_GET_IDENTITY, &hd)){
		char username[1024];
		getlogin_r(username, 1024);

		std::wcout<<"Hello, "<<username<<"!\n\n"
			  <<"Bytes: "<<sizeof(hd)<<"\n"
			  <<"Multisect: "<<hd.multsect<<"\n"
			  <<"Config: "<<hd.config<<"\n"
			  <<"Cyls: "<<hd.cyls<<"\n"
			  <<"Bufer size: "<<hd.buf_size<<"\n"
			  <<"Heads: "<<hd.heads<<"\n"
			  <<"Sector Bytes: "<<hd.sector_bytes<<"\n"
			  <<"Sectors: "<<hd.sectors<<"\n";
	}else{
		perror("ERROR: HDIO_GET_IDENTITY");
		std::exit(EXIT_FAILURE);
	}
	close(device);
}
