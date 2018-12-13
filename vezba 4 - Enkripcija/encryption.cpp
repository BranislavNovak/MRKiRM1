/* 
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        MRKiRM1
    Godina studija: IV
    Skolska godina: 2018/2019
    Semestar:       Letnji (VIII)
    
    Ime fajla:      encryption.cpp
    ********************************************************************
*/

// We do not want the warnings about the old deprecated and unsecure CRT functions since these examples can be compiled under *nix as well
#ifdef _MSC_VER
	#define _CRT_SECURE_NO_WARNINGS
#endif
#define ROWS 3
#define COLUMNS 7
// Include libraries
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include "conio.h"
#include "pcap.h"
#include "protocol_headers.h"

// Function declarations
void packet_handler(unsigned char *param, const struct pcap_pkthdr *packet_header, const unsigned char *packet_data);	// Callback function invoked by WinPcap for every incoming packet
unsigned char* encrypt_data(const unsigned char* packet_data, unsigned char* app_data, int app_length);					// Returns a copy of packet with encrypted application data
void print_message_as_table(unsigned char* data, int rows_max, int columns_max);										// Prints application data using table format
int find_key(int key, int * keys, int keys_size);																		// Finds specific key in array of keys and returns its index
void encrypt_data_help(unsigned char* packet_data, unsigned char* app_data, int app_length);

#define ETHERNET_FRAME_MAX 1518		// Maximal length of ethernet frame
unsigned int rows_key[ROWS] = {1, 0, 2};
unsigned int columns_key[COLUMNS] = {1, 0, 6, 2, 4, 3, 5};

// Main function captures packets from the file
int main()
{
	pcap_t* device_handle;
	char error_buffer[PCAP_ERRBUF_SIZE];
	
	// Open the capture file 
	if ((device_handle = pcap_open_offline("original_packets.pcap", // Name of the device
								error_buffer	  // Error buffer
							)) == NULL)
	{
		printf("\n Unable to open the file %s.\n", "example.pcap");
		return -1;
	}

	// Open the dump file 
	pcap_dumper_t* file_dumper = pcap_dump_open(device_handle, "encrypted_packets.pcap");
	

	if (file_dumper == NULL)
	{
		printf("\n Error opening output file\n");
		return -1;
	}

	// Check the link layer. We support only Ethernet for simplicity.
	if(pcap_datalink(device_handle) != DLT_EN10MB)
	{
		printf("\nThis program works only on Ethernet networks.\n");
		return -1;
	}

	struct bpf_program fcode;

	// Compile the filter
	if (pcap_compile(device_handle, &fcode, "ip and udp", 1, 0xffffff) < 0)
	{
		 printf("\n Unable to compile the packet filter. Check the syntax.\n");
		 return -1;
	}

	// Set the filter
	if (pcap_setfilter(device_handle, &fcode) < 0)
	{
		printf("\n Error setting the filter.\n");
		return -1;
	}

	// Read and dispatch packets until EOF is reached
	pcap_loop(device_handle, 0, packet_handler, (unsigned char*) file_dumper);

	// Close the file associated with device_handle and deallocates resources
	pcap_close(device_handle);

	return 0;
}

// Callback function invoked by WinPcap for every incoming packet
void packet_handler(unsigned char* file_dumper, const struct pcap_pkthdr* packet_header, const unsigned char* packet_data)
{
	/* DATA LINK LAYER - Ethernet */

	// Retrive the position of the ethernet header
	ethernet_header * eh = (ethernet_header *)packet_data;
	
	/* NETWORK LAYER - IPv4 */

	// Retrieve the position of the ip header
	ip_header* ih = (ip_header*) (packet_data + sizeof(ethernet_header));
	
	// TRANSPORT LAYER - UDP
	if(ih->next_protocol != 17)
	{
		return;
	}
	
	// Retrieve the position of udp header
	udp_header* uh = (udp_header*) ((unsigned char*)ih + ih->header_length * 4);

	// Retrieve the position of application data
	unsigned char* app_data = (unsigned char *)uh + sizeof(udp_header);

	// Total length of application data
	int app_length = ntohs(uh->datagram_length) - sizeof(udp_header);

	// Encrypt data using tranposition of rows and columns
	unsigned char * encrypted_packet = encrypt_data(packet_data, app_data, app_length);

	// Dump encrypted packets
	pcap_dump((unsigned char*) file_dumper, packet_header, encrypted_packet);
}

// Returns a copy of packet with encrypted application data
unsigned char* encrypt_data(const unsigned char* packet_data, unsigned char* app_data, int app_length)
{
	// Reserve memory for copy of the packet
	unsigned char encrypted_packet[ETHERNET_FRAME_MAX];
	unsigned char encrypted_packet_tmp[ETHERNET_FRAME_MAX];

	// TODO 1: Define keys
	unsigned char keys[ROWS][COLUMNS];
	
		
	// TODO 2: Print original message in table format
	print_message_as_table((unsigned char*)app_data, ROWS, COLUMNS);

	// TODO 3: Create a copy of the packets (copy headers and initialize application data with zeros)
	memcpy(encrypted_packet, packet_data, ETHERNET_FRAME_MAX);
	memset(encrypted_packet + ETHERNET_FRAME_MAX - app_length, 'X', app_length);

	//print_message_as_table((unsigned char*)encrypted_packet + ETHERNET_FRAME_MAX - app_length, ROWS, COLUMNS);
	
	// TODO 4: Find new row and column indices using old row and column indices
	// TODO 5: Encrypt application data
	//encrypt_data((unsigned char*)encrypted_packet[ETHERNET_FRAME_MAX - app_length], (unsigned char*)app_data, app_length);

	memcpy(encrypted_packet_tmp, encrypted_packet, ETHERNET_FRAME_MAX);
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			encrypted_packet_tmp[ETHERNET_FRAME_MAX - app_length + i*COLUMNS + j]  = app_data[rows_key[i]*COLUMNS + columns_key[j]]; 
		}
	}
	// TODO 6: Print encrypted message
	printf("\n\n rows: {1, 0, 2}  columns: {1, 0, 6, 2, 4, 3, 5}\n\n");
	print_message_as_table((unsigned char*)encrypted_packet_tmp + ETHERNET_FRAME_MAX - app_length, ROWS, COLUMNS);

	return encrypted_packet;
}

void print_message_as_table(unsigned char* data, int rows_max, int columns_max)
{
	for(int i = 0; i < rows_max; i++)
	{
		for(int j = 0; j < columns_max; j++)
		{
			printf(" %c ", data[i*columns_max + j]);
		}
		printf("\n");
	}
}

void encrypt_data_help(unsigned char* packet_data, unsigned char* app_data, int app_length)
{
	unsigned char encrypted_packet_help[ETHERNET_FRAME_MAX];
	memcpy(encrypted_packet_help, packet_data, ETHERNET_FRAME_MAX);
	memset(encrypted_packet_help + ETHERNET_FRAME_MAX - app_length, 'X', app_length);

	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLUMNS; j++)
		{
			encrypted_packet_help[ETHERNET_FRAME_MAX - app_length + rows_key[i]*COLUMNS + columns_key[j]]  = packet_data[i*COLUMNS + j]; 
		}
	}
}