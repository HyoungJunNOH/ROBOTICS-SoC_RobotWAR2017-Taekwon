#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include "robot_motion.h"
#include "robot_protocol.h"
#include "uart_api.h"
#include <termios.h>
static struct termios inittio, newtio;

void init_console(void)
{
    tcgetattr(0, &inittio);
    newtio = inittio;
    newtio.c_lflag &= ~ICANON;
    newtio.c_lflag &= ~ECHO;
    newtio.c_lflag &= ~ISIG;
    newtio.c_cc[VMIN] = 1;
    newtio.c_cc[VTIME] = 0;

    cfsetispeed(&newtio, B115200);

    tcsetattr(0, TCSANOW, &newtio);
}

void Order_to_Robot(int motion_number)
{
  int ret;

  init_console();

  ret = uart_open();
  if (ret < 0) return EXIT_FAILURE;

  uart_config(UART1, 57600, 8, UART_PARNONE, 1);

  switch(motion_number)
  {
	  case 2: printf("default_motion()"); default_motion();			break;
	  case 4: printf("move_forward()"); move_forward();				break;
	  case 5: printf("move_forwardx2()"); move_forwardx2();			break;
	  case 6: printf("move_back()"); move_back();					break;
	  case 8: printf("turn_left()"); turn_left();					break;
	  case 9: printf("turn_right()"); turn_right();					break;
	  case 11: printf("arm_attack_1()"); arm_attack_1();			break;
	  case 12: printf("arm_attack_2()"); arm_attack_2();			break;
	  case 13: printf("arm_attack_3()"); arm_attack_3();			break;
	  case 14: printf("arm_attack_4()"); arm_attack_4();			break; // 뚝배기
	  case 15: printf("back_step_kick_1()"); back_step_kick_1();	break;
	  case 17: printf("back_step_kick_2()"); back_step_kick_2();	break;
	  case 21: printf("move_and_attack_1()"); move_and_attack_1();	break;
	  case 22: printf("move_and_attack_2()"); move_and_attack_2();	break;
	  case 23: printf("move_and_attack_3()"); move_and_attack_3();	break;
	  case 24: printf("move_and_attack_4()"); move_and_attack_4();	break;
	  case 34: printf("detail_turn_left()"); detail_turn_left();	break;
	  case 35: printf("detail_turn_right()"); detail_turn_right();	break;
	  case 37: printf("attack_combo_1()"); attack_combo_1();		break;
	  case 41: printf("attack_combo_2()"); attack_combo_2();		break;
  }
	uart_close();
  return;
}
