/*
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 *
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>


/*
 *
 * Constants
 *
 */

/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */

static const char *directions[] = { "N", "E", "S", "W" };

static const char *msgs[] = {
        "approaching:",
        "region1:    ",
        "region2:    ",
        "region3:    ",
        "leaving:    "
};

static int carcount = 0;

struct semaphore* sem_carcount;
struct semaphore* sem_nw;
struct semaphore* sem_ne;
struct semaphore* sem_sw;
struct semaphore* sem_se;

/* use these constants for the first parameter of message */
enum { APPROACHING, REGION1, REGION2, REGION3, LEAVING };

static void
message(int msg_nr, int carnumber, int cardirection, int destdirection)
{
        kprintf("%s car = %2d, direction = %s, destination = %s\n",
                msgs[msg_nr], carnumber,
                directions[cardirection], directions[destdirection]);
}

/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        //(void) cardirection;
        //(void) carnumber;
        switch(cardirection){
            case 0:
                message(0, carnumber, cardirection, 2);
                P(sem_nw);
                message(1, carnumber, cardirection, 2);
		P(sem_sw);  
		message(2, carnumber, cardirection, 2);
                V(sem_nw);                              
                message(4, carnumber, cardirection, 2);
                V(sem_sw);
                break;
            case 1:
                message(0, carnumber, cardirection, 3);
                P(sem_ne);
                message(1, carnumber, cardirection, 3);
		P(sem_nw);
		message(2, carnumber, cardirection, 3);
                V(sem_ne);
                message(4, carnumber, cardirection, 3);
                V(sem_nw);
                break;
            case 2:
                message(0, carnumber, cardirection, 0);
                P(sem_se);
                message(1, carnumber, cardirection, 0);
                P(sem_ne);
		message(2, carnumber, cardirection, 0);
		V(sem_se);                               
                message(4, carnumber, cardirection, 0);
                V(sem_ne);
                break;
            case 3:
                message(0, carnumber, cardirection, 1);
                P(sem_sw);
                message(1, carnumber, cardirection, 1);
                P(sem_se);
		message(2, carnumber, cardirection, 1);
		V(sem_sw);                
                message(4, carnumber, cardirection, 1);
                V(sem_se);
                break;
            default:
                break;
        }
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        //(void) cardirection;
        //(void) carnumber;
        switch(cardirection){
             case 0:
                  message(0, carnumber, cardirection, 1);
                  P(sem_nw);
                  message(1, carnumber, cardirection, 1);
		  P(sem_sw);   
	          message(2, carnumber, cardirection, 1);               
		  V(sem_nw);
	          P(sem_se); 
	          message(3, carnumber, cardirection, 1);                 
                  V(sem_sw);
                  message(4, carnumber, cardirection, 1);
                  V(sem_se);
                break;
             case 1:
                  message(0, carnumber, cardirection, 2);
                  P(sem_ne);
                  message(1, carnumber, cardirection, 2);
		  P(sem_nw);
		  message(2, carnumber, cardirection, 2);
                  V(sem_ne);
	          P(sem_sw);
		  message(3, carnumber, cardirection, 2);
                  V(sem_nw);
                  message(4, carnumber, cardirection, 2);
                  V(sem_sw);
                break;
             case 2:
                  message(0, carnumber, cardirection, 3);
                  P(sem_se);
                  message(1, carnumber, cardirection, 3);
  		  P(sem_ne); 
	          message(2, carnumber, cardirection, 3);
                  V(sem_se);
		  P(sem_nw);
		  message(3, carnumber, cardirection, 3);
                  V(sem_ne);
                  message(4, carnumber, cardirection, 3);
                  V(sem_nw);
                break;
             case 3:
                  message(0, carnumber, cardirection, 0);
                  P(sem_sw);
                  message(1, carnumber, cardirection, 0);
		  P(sem_se); 
		  message(2, carnumber, cardirection, 0);
                  V(sem_sw);
	          P(sem_ne);
		  message(3, carnumber, cardirection, 0);
                  V(sem_se);
                  message(4, carnumber, cardirection, 0);
                  V(sem_ne);
                break;
             default:
                break;
         }
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
        (void) cardirection;
        (void) carnumber;
         switch(cardirection){
             case 0:
                  message(0, carnumber, cardirection, 3);
                  P(sem_nw);
                  message(1, carnumber, cardirection, 3);
                  message(4, carnumber, cardirection, 3);
                  V(sem_nw);
                break;
             case 1:
                  message(0, carnumber, cardirection, 0);
                  P(sem_ne);
                  message(1, carnumber, cardirection, 0);
                  message(4, carnumber, cardirection, 0);
                  V(sem_ne);
                break;
             case 2:
                  message(0, carnumber, cardirection, 1);
                  P(sem_se);
                  message(1, carnumber, cardirection, 1);
                  message(4, carnumber, cardirection, 1);
                  V(sem_se);
                break;
             case 3:
                  message(0, carnumber, cardirection, 2);
                  P(sem_sw);
                  message(1, carnumber, cardirection, 2);
                  message(4, carnumber, cardirection, 2);
                  V(sem_sw);
                break;
             default:
                break;
         }



}


/*
 * approachintersection()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */

static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{
        int cardirection;
        int cardestination;
        int state_enter = 0x0001;
        int threadfinish = 0; // while loop variable

        /*
         * Avoid unused variable and function warnings.
         */
        (void) unusedpointer;
        (void) carnumber;
        (void) gostraight;
	(void) turnleft;
        (void) turnright;
        /*
         * cardirection is set randomly.
         */
	//handling the u turn

        cardirection = random() % 4;
        cardestination = random() % 4;

	//different number
	while(cardirection == cardestination){
		cardestination = random() % 4;
	}

        while (!threadfinish){
            if (state_enter == 0x0001){
                P(sem_carcount);
                if (carcount<3){
                    carcount++;
                    V(sem_carcount);
                    state_enter = 0x0002;
                }
                else {
                    V(sem_carcount);
                    state_enter = 0x0001;
                }
            }
            else if (state_enter == 0x0002){
                switch(cardirection){
                    case 0:
                        switch(cardestination){
                            case 0:
                            break;
                            case 1:
                               turnleft(cardirection, carnumber);
                            break;
                            case 2:
                               gostraight(cardirection, carnumber);
                            break;
                            case 3:
                               turnright(cardirection, carnumber);
                            break;
                            default:
                            break;
                        }
                    break;
                    case 1:
                        switch(cardestination){
                            case 0:
                               turnright(cardirection, carnumber);
                            break;
                            case 1:
                            break;
                            case 2:
                               turnleft(cardirection, carnumber);
                            break;
                            case 3:
                               gostraight(cardirection, carnumber);
                            break;
                            default:
                            break;
                        }
                    break;
                    case 2:
                        switch(cardestination){
                            case 0:
                               gostraight(cardirection, carnumber);
                            break;
                            case 1:
                               turnright(cardirection, carnumber);
                            break;
                            case 2:
                            break;
                            case 3:
                               turnleft(cardirection, carnumber);
                            break;
                            default:
                            break;
                        }
                    break;
                    case 3:
                        switch(cardestination){
                            case 0:
                               turnleft(cardirection, carnumber);
                            break;
                            case 1:
                               gostraight(cardirection, carnumber);
                            break;
                            case 2:
                               turnright(cardirection, carnumber);
                            break;
                            case 3:
                            break;
                            default:
                            break;
                        }
                    break;
                    default:
                    break;
                }
                state_enter = 0x0003;
            }
            else if (state_enter==0x0003){
                P(sem_carcount);
                carcount--;
                V(sem_carcount);
                threadfinish = 1;
            }
        } // end of while loop



}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

        /*
         * Start NCARS approachintersection() threads.
         */

        sem_carcount = sem_create("sem_carcount", 1);
        sem_nw = sem_create("sem_nw", 1);
        sem_ne = sem_create("sem_ne", 1);
        sem_sw = sem_create("sem_sw", 1);
        sem_se = sem_create("sem_se", 1);

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {

                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }

        return 0;
}
