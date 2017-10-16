#include "benchmark/benchmark.h"
#ifdef __OpenBSD__
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <net/bpf.h>

/*
 * https://github.com/iovisor/bcc/tree/master/tests/cc
 * https://gist.github.com/msantos/939154
 * http://bastian.rieck.ru/howtos/bpf/
 * https://www.vankuik.nl/2012-02-09_Writing_ethernet_packets_on_OS_X_and_BSD
 */

unsigned char dest_mac[ETHER_ADDR_LEN]  = {0x00, 0x1b, 0x21, 0x53, 0x83, 0x4f};
unsigned char src_mac[ETHER_ADDR_LEN] = {0xc8, 0xbc, 0xc8, 0x91, 0x79, 0x2d};

struct frame_t {
    struct ether_header header;
    unsigned char payload[ETHER_MAX_LEN - ETHER_HDR_LEN];
    ssize_t len;
    ssize_t payload_len;
};

const size_t ETHER_PAYLOAD_START = (2*ETHER_ADDR_LEN) + ETHER_TYPE_LEN;
const size_t ETHER_PAYLOAD_LEN = ETHER_MAX_LEN - ETHER_HDR_LEN - ETHER_CRC_LEN;

int open_dev(void)
{
    char buf[ 11 ] = { 0 };
    int bpf = 0;
    int i = 0;
    for(i = 0; i < 99; i++ )
    {
        sprintf( buf, "/dev/bpf%i", i );
        bpf = open( buf, O_RDWR );
        if( bpf != -1 ) {
            printf("Opened device /dev/bpf%i\n", i);
            break;
        }
    }
    if(bpf == -1) {
        printf("Cannot open any /dev/bpf* device, exiting\n");
        exit(1);
    }
    return bpf;
}

void assoc_dev(int bpf, char* interface)
{
    struct ifreq bound_if;
    strcpy(bound_if.ifr_name, interface);
    if(ioctl( bpf, BIOCSETIF, &bound_if ) > 0) {
        printf("Cannot bind bpf device to physical device %s, exiting\n", interface);
        exit(1);
    }
    printf("Bound bpf device to physical device %s\n", interface);
}

int get_buf_len(int bpf)
{
    int buf_len = 1;
    // activate immediate mode (therefore, buf_len is initially set to "1")
    if( ioctl( bpf, BIOCIMMEDIATE, &buf_len ) == -1 ) {
        printf("Cannot set IMMEDIATE mode of bpf device\n");
        exit(1);
    }
    // request buffer length
    if( ioctl( bpf, BIOCGBLEN, &buf_len ) == -1 ) {
        printf("Cannot get bufferlength of bpf device\n");
        exit(1);
    }
    printf("Buffer length of bpf device: %d\n", buf_len);
    return buf_len;
}

void read_frames(int bpf, int buf_len)
{
    int read_bytes = 0;
    struct frame_t *frame;
    struct bpf_hdr *bpf_buf = (struct bpf_hdr*) malloc(buf_len);
    struct bpf_hdr *bpf_packet;
    int run_loop = 1;
    int i = 0;
    printf("Start reading frames\n");
    while(run_loop) {
        memset(bpf_buf, 0, buf_len);
        if((read_bytes = read(bpf, bpf_buf, buf_len)) > 0) {
            printf("Read %d\n", i);
            i++;
            // read all packets that are included in bpf_buf. BPF_WORDALIGN is used
            // to proceed to the next BPF packet that is available in the buffer.
            char* ptr = (char*)bpf_buf;
            while(ptr < ((char*)(bpf_buf) + read_bytes)) {
                bpf_packet = (struct bpf_hdr*)ptr;
                frame = (struct frame_t*)((char*) bpf_packet + bpf_packet->bh_hdrlen);
                frame->len = bpf_packet->bh_caplen;
                frame->payload_len = frame->len - (2*ETHER_ADDR_LEN) - ETHER_TYPE_LEN;
                // Do something with the frame
                printf("Got packet, length of frame: %ld, length of data: %ld\n",
                    frame->len, frame->payload_len);
                ptr += BPF_WORDALIGN(bpf_packet->bh_hdrlen + bpf_packet->bh_caplen);
            }
        } else {
            perror("Meh, couldn't read from bpf device");
            exit(1);
        }
    }
}

void read_single_frame(int bpf, int buf_len)
{
    int read_bytes = 0;
    int i;
    struct bpf_hdr* bpf_buf = malloc(buf_len);
    memset(bpf_buf, 0, buf_len);
    char *ptr;

    printf("Header length: %ld\n", sizeof(bpf_buf));
    read_bytes = read(bpf, bpf_buf, buf_len);
    if(read_bytes > 0) {
        printf("Got %d bytes\n", read_bytes);
    } else {
        printf("Got 0 bytes\n");
    }
    ptr = (char*)bpf_buf;
    for(i = 0; i < read_bytes; i++) {
        unsigned char byte = (unsigned char) *(ptr + i);
        printf("0x%02X ", byte);
    }
    printf("\n");
}

void write_single_frame(int bpf)
{
    ssize_t data_length = 0x4F;
    struct frame_t frame;
    memcpy(frame.header.ether_dhost, dest_mac, ETHER_HDR_LEN);
    memcpy(frame.header.ether_shost, src_mac, ETHER_HDR_LEN);
    frame.header.ether_type = 0x00;
    frame.len = (2*ETHER_ADDR_LEN) + ETHER_TYPE_LEN + data_length;
    // Fill frame with ramp
    unsigned char j;
    for (j = 0; j < data_length; j++) {
        frame.payload[j] = j;
    }
    ssize_t bytes_sent;
    bytes_sent = write(bpf, &frame, frame.len);
    if(bytes_sent > 0) {
        printf("Bytes sent: %ld\n", bytes_sent);
    } else {
        perror("Whoops! Does the device actually have an IP address?");
        exit(1);
    }
}

void write_frames(int bpf, const unsigned char *databuf, size_t datalen)
{
    size_t start = 0;
    struct frame_t *frame = malloc(ETHER_MAX_LEN);
    size_t bytes_to_send;
    ssize_t bytes_sent;
    memcpy(frame->header.ether_dhost, dest_mac, ETHER_HDR_LEN);
    memcpy(frame->header.ether_shost, src_mac, ETHER_HDR_LEN);
    frame->header.ether_type = 0x0000;
    do {
        // Clear frame
        bzero((void*)(frame+ETHER_PAYLOAD_START), ETHER_PAYLOAD_LEN);
        // Calculate remainder
        if((datalen - start) < ETHER_PAYLOAD_LEN) {
            bytes_to_send = datalen - start;
        } else {
            bytes_to_send = ETHER_PAYLOAD_LEN;
        }
        // Fill frame payload
        printf("Copying payload from %lu, length %lu\n", start, bytes_to_send);
        memcpy(frame->payload, (void*)(databuf + start), bytes_to_send);
        frame->len = ETHER_HDR_LEN + bytes_to_send;
        // Note we don't add the four-byte CRC, the OS does this for us.
        // Neither do we fill packets with zeroes when the frame length is
        // below the minimum Ethernet frame length, the OS will do the
        // padding.
        printf("Total frame length: %lu of maximum ethernet frame length %d\n",
            frame->len, ETHER_MAX_LEN - ETHER_CRC_LEN);
        bytes_sent = write(bpf, frame, frame->len);
        // Check results
        if(bytes_sent < 0 ) {
            perror("Error, perhaps device doesn't have IP address assigned?");
            exit(1);
        } else if(bytes_sent != frame->len) {
            printf("Error, only sent %ld bytes of %lu\n", bytes_sent, bytes_to_send);
        } else {
            printf("Sending frame OK\n");
        }
        start += bytes_to_send;
    } while (start < datalen);
    free(frame);
}

unsigned char* make_testdata(int len)
{
    unsigned char *testdata = (unsigned char*)malloc(len);
    int i;
    unsigned char j = 0;
    for(i = 0; i < len; i++) {
        testdata[i] = j;
        j++;
        if(j < sizeof(char)) {
            j = 0;
        }
    }
    return testdata;
}
#endif /* __OpenBSD__ */

void bpf(void) {
#ifdef __OpenBSD__
    char* interface = "lo";
    unsigned char* testdata;
    size_t testdata_len = 4530;
    int bpf;
    int buf_len;
    bpf = open_dev();
    assoc_dev(bpf, interface);
    buf_len = get_buf_len(bpf);
    //read_single_frame(bpf, buf_len);
    //read_frames(bpf, buf_len);
    testdata = make_testdata(testdata_len);
    write_frames(bpf, testdata, testdata_len);
#endif /* __OpenBSD__ */
}

void BM_bpf(benchmark::State& state) {
#ifdef __OpenBSD__
	while (state.KeepRunning()) bpf();
#else
	state.SkipWithError("Unsupported");
#endif
}

BENCHMARK(BM_bpf);

BENCHMARK_MAIN()
