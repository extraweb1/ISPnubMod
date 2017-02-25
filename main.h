/*
 * main.h
 *
 * Created: 18.02.2017 21:21:27
 * Author: Michael Groene
 *
 */ 


#ifndef MAIN_H_
#define MAIN_H_


// mode for compatibility with original ISPnub-Board
// functionality is decreased
#define COMPATIBILITY_MODE_ISPNUB_ORIGINAL 0
#define DONT_USE_YELLOW_LED 0


//states
#define S_INIT 0
#define S_IDLE 10
#define S_PROGRAMMING 20
#define S_SLEEP 30
#define S_WAKEUP 31
#define S_NO_MORE 40	//counter empty
#define S_NO_PROGRAM 41




#endif /* MAIN_H_ */