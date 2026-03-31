#include "WirelessMouseDesktopV2.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "MouseControl.h"

#define BUFLEN 512 // Max length of buffer
#define PORT 8192  // The port on which to listen for incoming data

int main(void) {
	int s;
	struct sockaddr_in si_me, si_other;
	int recv_len;
	uint32_t slen = sizeof(si_other);
	char buf[BUFLEN];

	init_uinput();

	// Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("Could not create socket : %d (%s)\n", errno, strerror(errno));
	}
	DEBUG("Socket created.\n");

	// Prepare the sockaddr_in structure
	si_me.sin_family = AF_INET;
	si_me.sin_addr.s_addr = INADDR_ANY;
	si_me.sin_port = htons(PORT);

	// Bind
	if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
		printf("Bind failed with error code : %d (%s)\n", errno, strerror(errno));
		exit(1);
	}
	DEBUG("Bind done\n");

	// keep listening for data
	while (1) {
		DEBUG("Waiting for data...\n");

		// clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);

		// try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&si_other, &slen)) == -1) {
			printf("recvfrom() failed with error code : %d (%s)\n", errno, strerror(errno));
			exit(EXIT_FAILURE);
		}

		// print details of the client/peer and the data received
		DEBUG("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		if (recv_len == sizeof(data_t)) {
			HandleUDP((data_t *)buf);
		} else {
			printf("Received packet of invalid size: %d expected %lu\n", recv_len, sizeof(data_t));
		}
	}

	close(s);

	return 0;
}
