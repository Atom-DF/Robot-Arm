#include "dynamixel.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void move_to_location(int connection, unsigned char id,
			unsigned char loc_h, unsigned char loc_l) {

	unsigned char cs = ~ ( id + 0x07 + 0x03 + 0x1e + loc_l + loc_h +
				0x30 + 0x00);

	unsigned char arr[] = { 0xff, 0xff, id, 0x07, 0x03, 0x1e, loc_l,
                                       loc_h, 0x30, 0x00, cs };

	int buff_len = 100;
	unsigned char buff[buff_len];

	int bytes_read = write_to_connection(connection,arr,11,buff,buff_len);

}

void wait_until_done(int connection, unsigned char id) {
	usleep(2000000);
}
void wait_until_done_longer(int connection, unsigned char id){
  usleep(3000000);
}
void reset(int connection,int hold){
  printf("Resetting\n");
  int i;
  for(i=1;i<=4;i++){
    move_to_location(connection,i,0x01,0xff);
  }
  if(hold == 0){
    move_to_location(connection,5,0x01,0xff);
  }
  wait_until_done_longer(connection,1);
}
void pickup(int connection){
  printf("Moving\n");
  int i;
  move_to_location(connection,1,0x01,0x82);
  move_to_location(connection,2,0x01,0x01);
  move_to_location(connection,3,0x01,0x90);
  move_to_location(connection,4,0x01,0x01);
  wait_until_done(connection,1);
}
void grab(int connection){
  printf("Grabbing\n");
  move_to_location(connection,5,0x01,0x4f);
  wait_until_done(connection,1);
}
void drop(int connection){
  printf("Dropping\n");
  move_to_location(connection,5,0x01,0xff);
  wait_until_done(connection,1);
}
void place(int connection){
  printf("Moving\n");
  move_to_location(connection,2,0x01,0x01);
  move_to_location(connection,4,0x01,0x01);
  wait_until_done(connection,1);
}
int main(int argc, char* argv[]) {
  printf("***STARTING***\n");
	int connection = open_connection("/dev/ttyUSB0",B1000000);
  reset(connection,0);
  pickup(connection);
  grab(connection);
  reset(connection,1);
  place(connection);
  usleep(1500000);//extra wait to prevent vibrations.
  drop(connection);
  reset(connection,0);
  printf("***STOPPING***\n");
	return 0;

}
