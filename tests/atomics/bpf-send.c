#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/bpf.h>
#include <stdio.h>
#include <string.h>

/*
		http://bastian.rieck.ru/howtos/bpf/
		https://gist.github.com/msantos/939154
		https://www.vankuik.nl/2012-02-09_Writing_ethernet_packets_on_OS_X_and_BSD
*/

int main() {

		char buf[ 11 ] = { 0 };
		int bpf = 0;

		for( int i = 0; i < 99; i++ )
		{
			sprintf( buf, "/dev/bpf%i", i );
			bpf = open(buf, O_RDWR);

			if( bpf != -1 )
				break;
		}

		const char* interface = "lo0";
		struct ifreq bound_if;

		strcpy(bound_if.ifr_name, interface);
		if(ioctl(bpf, BIOCSETIF, &bound_if) > 0)
		    return(-1);

		int buf_len = 1;

		if( ioctl(bpf, BIOCIMMEDIATE, &buf_len) == -1 )
			return( -1 );

		if( ioctl(bpf, BIOCGBLEN, &buf_len) == -1 )
			  return( -1 );

		int read_bytes = 0;

/*
		ethernet_frame* frame;
		struct bpf_hdr* bpf_buf = new bpf_hdr[buf_len];
		struct bpf_hdr* bpf_packet;

		while(run_loop)
		{
			memset(bpf_buf, 0, buf_len);

			if((read_bytes = read(bpf, bpf_buf, buf_len)) > 0)
			{
				int i = 0;

				// read all packets that are included in bpf_buf. BPF_WORDALIGN is used
				// to proceed to the next BPF packet that is available in the buffer.

				char* ptr = reinterpret_cast<char*>(bpf_buf);
				while(ptr < (reinterpret_cast<char*>(bpf_buf) + read_bytes))
				{
					bpf_packet = reinterpret_cast<bpf_hdr*>(ptr);
					frame = (ethernet_frame*)((char*) bpf_packet + bpf_packet->bh_hdrlen);

					ptr += BPF_WORDALIGN(bpf_packet->bh_hdrlen + bpf_packet->bh_caplen);
				}
			}
		}
*/

		return 0;
}
