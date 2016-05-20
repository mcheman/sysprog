#include "dhcp.h"

dhcp_packet_t p;

packet_t dhcp_discover_p = {.packet_data = { 0x00, 0x44, 0x00, 0x43, 0x00, 0xEE,
			    0x6e, 0x66, 0x01, 0x01, 0x06, 0x00, 0x4c, 0xb0,
			    0x8f, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x57,
			    0x18, 0xc0, 0x20, 0x97, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x82,
			    0x53, 0x63, 0x35, 0x01, 0x03, 0xff
	}
};


void dhcp_handshake(){
}

dhcp_packet_t *dhcp_discover( dhcp_packet_t *dest ){
	new_ipv4_header( &(dest->ip_header),
			 (ipv4_addr_t){.addr={255,255,255,255}},
			 (ipv4_addr_t){.addr={0,0,0,0}}, UDP_PROTOCOL,
			 UDP_HEADER_L + DHCP_DATA_STRUCT_L + 3,
			 64, FLAG_DF, 0, 0 );

	new_udp_header( &(dest->udp_header), DHCP_SEND_PORT, DHCP_DEST_PORT,
		         90 + 3 );

	dest->dhcp_data.op = 1;
	dest->dhcp_data.htype = 1;
	dest->dhcp_data.hlen = 6;
	dest->dhcp_data.hops = 0;
	dest->dhcp_data.xid = 0xDEADBEEF;
	dest->dhcp_data.secs = 0;
	dest->dhcp_data.flags = DHCP_B_FLAG;
	dest->dhcp_data.cli_addr = (ipv4_addr_t){.addr={0x01,0x23,0x45,0x67}};
	dest->dhcp_data.your_ip = (ipv4_addr_t){.addr={0x89,0xab,0xcd, 0xef}};
	dest->dhcp_data.serv_ip = (ipv4_addr_t){.addr={0x00,0x11,0x22,0x33}};
	dest->dhcp_data.gate_ip = (ipv4_addr_t){.addr={0x44,0x55,0x66,0x77}};
	dest->dhcp_data.cookie = 0x63825363;
	dest->dhcp_data.cli_hw_addr[0] = 0x0;
	dest->dhcp_data.cli_hw_addr[1] = 0x0;
	dest->dhcp_data.cli_hw_addr[2] = 0x0;
	dest->dhcp_data.cli_hw_addr[3] = 0x0;
	dest->data[0] = DHCP_MESSAGE_TYPE;
	dest->data[1] = 1;
	dest->data[2] = DHCP_MESSAGET_DISCOVER;
	dest->data[3] = 0xFF;

	dest->udp_header.checksum = udp_checksum( (udp_packet_t *)dest,
						  &(dest->ip_header.dest_ip),
						  &(dest->ip_header.source_ip) );

	return dest;
}

packet_t *dhcp_discover_copy( ){
	new_ipv4_header( &(dhcp_discover_p.header) ,
			 (ipv4_addr_t){.addr={255,255,255,255}},
			 (ipv4_addr_t){.addr={0,0,0,0}}, UDP_PROTOCOL,
			 UDP_HEADER_L + DHCP_DATA_STRUCT_L + 3,
			 64, FLAG_DF, 0, 0 );
	return &dhcp_discover_p;
}

void dhcp_data_hton( dhcp_data_t *p ){
	p->xid = htonl( p->xid );
	p->secs = htons( p->secs );
	p->flags = htons( p->flags );
	p->cookie = htonl( p->cookie );
}

void dhcp_data_ntoh( dhcp_data_t *p ){
	p->xid = ntohl( p->xid );
	p->secs = ntohs( p->secs );
	p->flags = ntohs( p->flags );
	for ( int i = 0; i < CLI_HW_ADDR_L; ++i ){
		p->cli_hw_addr[i] =
			ntohl( p->cli_hw_addr[i] );
	}
	p->cookie = ntohl( p->cookie );
}
