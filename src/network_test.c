#include "network_test.h"
#include "syscall.h"
#include "common.h"
#include "c_io.h"
#include "dhcp.c"

udp_packet_t buf[ TEST_BUF_L ];


void receive( packet_t buf[], uint8_t size );

void test_udp( void );

int32_t network_test(){
        c_printf("-------------------------- Test UDP creation and send stub\n");
        test_udp();
        return 0;
}

void receive( packet_t buf[], uint8_t size ){
        c_printf("-------------------------- Test receive, %d packets\n", size);

        for ( int i = 0; i < size; ++i ){
	        c_printf("-------------------------- Packet %d\n", i );
                for ( int b = 0; b < PACKET_DUMP_LEN; ++b ){
                        if ( b % 4 == 0 ){ c_printf( "   " ); };
                        if ( b % 16 == 0 ){ c_printf( "\n" ); };
                        c_printf( "%x ", ((p_t *)(&buf[i]))->data[b] );
                }
	        c_printf("\n\n");
        }
	
}

void test_udp(){
        uint32_t result = reqport( SEND_PORT, &receive );
        if ( result != 0 ){
                return;
        }
	c_printf( "Requesting port %x, result = %x\n", SEND_PORT, result );

        for ( int i  = 0; i < TEST_BUF_L; ++i ){
	        uint16_t dest_port = 0;
		if ( i < 4 ){
		        dest_port = DEST_PORT;
		} else if ( i == 4 ){
		        dest_port = 12;
		}
		if ( i <= 4 ){
		  new_ipv4_header( &buf[ i ].ip_header, (ipv4_addr_t){{127,0,0,1}},
				   (ipv4_addr_t){{127,0,0,1}}, UDP_PROTOCOL,
				   UDP_HEADER_L + 5, 64, FLAG_DF, 0, 0 );
                  new_udp_header( &buf[ i ].udp_header, SEND_PORT, dest_port,
				  4 );
		  ipv4_header_hton( &buf[ i ].ip_header );
		  udp_header_hton( &buf[ i ].udp_header );
		  if ( i == 0 ){
		          buf[ i ].data[0] = 0xDE;
			  buf[ i ].data[1] = 0xAD;
			  buf[ i ].data[2] = 0xBE;
			  buf[ i ].data[3] = 0xEF;
		  } else if ( i == 1 ){
		          buf[ i ].data[0] = 0xF0;
			  buf[ i ].data[1] = 0x0;
			  buf[ i ].data[2] = 0xD;
			  buf[ i ].data[3] = 0x5;
		  }

		} else if ( i == 3 ){
	                dhcp_discover( (dhcp_packet_t *)(&buf[ i ]) );
			dhcp_data_hton( &((dhcp_packet_t *)(&buf[ i ]))->dhcp_data );
		}

                buf[ i ].udp_header.checksum = udp_checksum(
                        &buf[ i ], &(ipv4_addr_t){{127,0,0,1}},
                        &(ipv4_addr_t){{127,0,0,1}} );
        }
        c_printf("-------------------------- Sent packets\n");
	c_printf( "Sending %d packets\n", TEST_BUF_L );
	sleep( 1 );
	    
        send( (packet_t *)&buf[0], TEST_BUF_L );
	c_printf("-------------------------- Releasing port %x\n", SEND_PORT );
	relport( SEND_PORT );
	c_printf("-------------------------- Released port %x\n", SEND_PORT );
}
