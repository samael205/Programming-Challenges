#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/hdreg.h>

int main(void){
	using std::wcout;
	static struct hd_driveid hd;	
	
	const int device = open("/dev/sda", O_RDONLY | O_NONBLOCK);
	if(device < 0){
		std::cerr<<"Cannot open device!\n";
		std::exit(EXIT_FAILURE);
	}

	if(!ioctl(device, HDIO_GET_IDENTITY, &hd)){
		char username[1024];
		getlogin_r(username, 1024);

		wcout<<"Hello, "<<username<<"!\n\n";
		wcout<<"Bytes: "<<sizeof(hd)<<"\n";
		wcout<<"Multisect: "<<hd.multsect<<"\n";
		wcout<<"Config: "<<hd.config<<"\n";
		wcout<<"Cyls: "<<hd.cyls<<"\n";
		wcout<<"Bufer size: "<<hd.buf_size<<"\n";
		wcout<<"Heads: "<<hd.heads<<"\n";
		wcout<<"Sector Bytes: "<<hd.sector_bytes<<"\n";
		wcout<<"Sectors: "<<hd.sectors<<"\n";
	}else{
		perror("ERROR: HDIO_GET_IDENTITY");
		std::exit(EXIT_FAILURE);
	}
}