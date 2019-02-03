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

// A wait method
void wait_until_done(int connection, unsigned char id) {
	usleep(2000000);
}

// A wait longer method
void wait_until_done_longer(int connection, unsigned char id){
  usleep(3000000);
}

// A method to make moving the robot easier (horisontal postition)
void hPos(int connection, int place){
  switch (place) {
    case 0:
      //printf("Moving to left position\n");
      move_to_location(connection,1,0x01,0x5e);
      break;
    case 1:
      //printf("Moving to middle position\n");
      move_to_location(connection,1,0x01,0xa1);
      break;
    case 2:
      //printf("Moving to right position\n");
      move_to_location(connection,1,0x01,0xdf);
  }
  wait_until_done(connection,1);
}

// A method to make moving the robot easier (vertical postition)
void vPos(int connection, int place){
  switch (place) {
    case 0:
      //printf("lowering to bottom position\n");
      move_to_location(connection,2,0x01,0x1d);
      move_to_location(connection,3,0x01,0x57);
      move_to_location(connection,4,0x01,0x45);
      break;
    case 1:
      //printf("lowering to middle position\n");
      move_to_location(connection,2,0x01,0x35);
      move_to_location(connection,3,0x01,0x5a);
      move_to_location(connection,4,0x01,0x25);
      break;
    case 2:
      //printf("lowering to top position\n");
      move_to_location(connection,2,0x01,0x4f);
      move_to_location(connection,3,0x01,0x5c);
      move_to_location(connection,4,0x01,0x20);
      break;
  }
  wait_until_done(connection,1);
}

// A method which combines the two other methods
void move(int connection, int x, int y){
  hPos(connection,x);
  vPos(connection,y);
}

// An old medium reset method
void home(int connection){
  int i;
  for(i=2;i<5;i++){
    move_to_location(connection,i,0x01,0xaa);
    usleep(500000);
  }
  usleep(500000);
}

// A grab method
void grab(int connection){
  //printf("Grabbing\n");
  move_to_location(connection,5,0x01,0x4a);
  wait_until_done(connection,1);
}

// Does the oposite of the grab method
void place(int connection){
  //longer wait to prevent vibrations.
  //printf("Dropping\n");
  wait_until_done_longer(connection,1);
  move_to_location(connection,5,0x01,0xff);
  wait_until_done(connection,1);
}

// A method which does one whole move at a time
void transport(int connection,int x1, int y1, int x2, int y2){
  home(connection);
  move(connection,x1,y1);
  grab(connection);
  home(connection);
  move(connection,x2,y2);
  place(connection);
}

//***********Anything after this is either an old method, an old test or the
//Amain method***********



// reset the robot to a "safe" value
void reset(int connection,int hold){
  //printf("Resetting\n");
  int i;
  for(i=1;i<=4;i++){
    move_to_location(connection,i,0x01,0xff);
  }
  if(hold == 0){
    move_to_location(connection,5,0x01,0xff);
  }
  wait_until_done_longer(connection,1);
}

// An old complete reset method (straight up)
void superhome(int connection){
  int i;
  for(i=5;i>0;i--){
    move_to_location(connection,i,0x01,0xff);
    usleep(500000);
  }
  usleep(500000);
}

// Another reset method non recursive
void newhome(int connection){
	move_to_location(connection,2,0x01,0xff);
	usleep(500000);
	move_to_location(connection,3,0x01,0xff);
	usleep(500000);
	move_to_location(connection,4,0x01,0xff);
	usleep(500000);
	move_to_location(connection,1,0x01,0xff);
	usleep(500000);
}

/*
void shitvPos(int connection, int place){
  switch (place) {
    case 0:
      //printf("lowering to bottom position\n");
      move_to_location(connection,2,0x00,0xd4);
      move_to_location(connection,3,0x00,0xd1);
      move_to_location(connection,4,0x03,0x0b);
      break;
    case 1:
      //printf("lowering to middle position\n");
      move_to_location(connection,2,0x01,0x08);
      move_to_location(connection,3,0x00,0xbf);
      move_to_location(connection,4,0x03,0x0d);
      break;
    case 2:
      //printf("lowering to top position\n");
      move_to_location(connection,2,0x01,0x2b);
      move_to_location(connection,3,0x00,0x9d);
      move_to_location(connection,4,0x03,0x0b);
      break;
  }
  wait_until_done(connection,1);
}*/

// A method to show your unhappiness
void shakeHead(int connection){
	home(connection);
	int i;
	for(i=0;i<10;i++){
		move_to_location(connection,1,0x01,0x5e);
		usleep(250000);
		move_to_location(connection,1,0x01,0xdf);
		usleep(250000);
	}
}

// A method to ask us where to go (usefull for debugging)
move_to_input(int connection){
	int X;
	int Y;
	printf("Enter X : ");
	scanf("%d",&X);
	printf("Enter Y : ");
	scanf("%d",&Y);
	move(connection,X,Y);
}

// A method to show rebelion
void kickOver(int connection){
	home(connection);
	move(connection,1,2);
	move(connection,2,2);
	printf("YOU THOUGHT I WAS GONNA LEAVE IT STANDING ???\n");
}

//
int main(int argc, char* argv[]) {

	int connection = open_connection("/dev/ttyUSB0",B1000000);
	n = 3;
	int i;
	for(i=1;i<n;i++)
	{
		transport(connection,0,n-i,n-i,0);
	}
	transport(connection,n-1,0,n-2,1);
	for(i=n;i>1;i--)
	{
		if(i = 2)
		{
			transport(connection,n-2,1,n-3,0);
		}
		transport(connection,n-i,0,n-i,n-1);
	}
	transport(connection,n-3,0,n-1,n-1);
	/*
	move_to_location(connection,5,0x01,0xff);
  printf("move 1\n");
  transport(connection,0,2,2,0);
  printf("move 2\n");
  transport(connection,0,1,1,0);
  printf("move 3\n");
  transport(connection,2,0,1,1);
  printf("move 4\n");
  transport(connection,0,0,2,0);
  printf("move 5\n");
  transport(connection,1,1,0,0);
  printf("move 6\n");
  transport(connection,1,0,2,1);
  printf("move 7\n");
  transport(connection,0,0,2,2);
	//kickOver(connection);
	//shakeHead(connection);
  //superhome(connection);
  //newhome(connection);
  //move(connection,0,0);
	//wait_until_done(connection, 1);
  //move(connection,1,0);
	//wait_until_done(connection, 1);
	//move_to_input(connection);
	//move(connection,1,0);*/
	return 0;

}
