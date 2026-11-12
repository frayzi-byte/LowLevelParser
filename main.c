#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <arpa/inet.h>

#define MAX_LENGHT 256

typedef char string[MAX_LENGHT];

int main() 
{
//вводная часть 
    string option;

    scanf("%255s", option);
    printf("You choosed : %s\n", option);
    printf("Your script starting in 3 seconds..\n");
    sleep(3);
//сама прога
    unsigned char *buffer = malloc(1500);

    if (buffer == NULL) {
        perror("malloc");
        return 1;
    }

    int if_index = if_nametoindex(option);

    if (if_index == 0) {
        perror("if_nametoindex");
        return 1;
    }

    struct sockaddr_ll addr = {0};
    addr.sll_family = AF_PACKET;
    addr.sll_ifindex = if_index;
    addr.sll_protocol = htons
    (ETH_P_ALL);

    int s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (s == -1) {
        perror("socket error");
        return -1;
    }

    int b = bind(s, (struct sockaddr *)&addr, sizeof(addr));

    if (b == -1) {
        perror("bind error");
        return -1;
    }

    int i = 0;

    while (i < 100) {

        ssize_t received = recvfrom(s, buffer, 1500, 0, NULL, NULL);
    
        if (received == -1) {
            perror("recv");
            return -1;
        }

        if (received < sizeof(struct ethhdr)) {
            continue;
        }

        i += 1;

        struct ethhdr *eth = (struct ethhdr *)buffer;

        printf("[SNDR_MAC]%02X:%02X:%02X:%02X:%02X:%02X",
            eth->h_source[0],
            eth->h_source[1],
            eth->h_source[2],
            eth->h_source[3],
            eth->h_source[4],
            eth->h_source[5]
        );

        printf("[DEST_MAC]%02X:%02X:%02X:%02X:%02X:%02X",
            eth->h_dest[0],
            eth->h_dest[1],
            eth->h_dest[2],
            eth->h_dest[3],
            eth->h_dest[4],
            eth->h_dest[5]
        );

        printf("[PROTOCOL]%04X\n", ntohs(eth->h_proto));

    }
    

    free(buffer);
    close(s);

    return 0;
}