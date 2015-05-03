/*
 * catlock.c
 *
 * 30-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: Please use LOCKS/CV'S to solve the cat syncronization problem in 
 * this file.
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

static int numBowl = 1; 

static int bowl1;

static int bowl2;


struct lock * bowl;
struct lock * mutux;
/*
 * Number of food bowls.
 */

#define NFOODBOWLS 2

/*
 * Number of cats.
 */

#define NCATS 6

/*
 * Number of mice.
 */

#define NMICE 2


/*
 * 
 * Function Definitions
 * 
 */



/* who should be "cat" or "mouse" */
static void lock_eat(const char *who, int num, int bowl, int iteration)
{
        kprintf("%s: %d starts eating: bowl %d, iteration %d\n", who, num, bowl, iteration);
        clocksleep(1);
        kprintf("%s: %d ends eating: bowl %d, iteration %d\n", who, num, bowl, iteration);
}

/*
 * catlock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long catnumber: holds the cat identifier from 0 to NCATS -
 *      1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static void catlock(void * unusedpointer, unsigned long catnumber)
{
        /*
         * Avoid unused variable warnings.
         */
	
	//int number = ((unsigned int)random() % 2);
	//bowl-> bowl_num = number;
	
	int i;
	for(i = 0; i < 4;){
		int number = ((unsigned int)random() % 2) + 1;
		while(i < 4 && (numBowl < 3) && ((number == 1 && bowl1 == 0) || (number == 2 && bowl2 == 0))){
        		lock_acquire(bowl);
			if(!bowl1){
				bowl1 = 1;
			}
			else if(!bowl2){
				bowl2 = 1;
			}
			numBowl++;
			lock_eat("cat", catnumber, number, i);
			numBowl--;
			if(number == 1){
				bowl1 = 0;
			}
			else if(number == 2){
				bowl2 = 0;
			}
			i++;
			lock_release(bowl);
		}
	}
        (void) unusedpointer;
        (void) catnumber;
}
	

/*
 * mouselock()
 *
 * Arguments:
 *      void * unusedpointer: currently unused.
 *      unsigned long mousenumber: holds the mouse identifier from 0 to 
 *              NMICE - 1.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Write and comment this function using locks/cv's.
 *
 */

static void mouselock(void * unusedpointer, unsigned long mousenumber)
{

        /*
         * Avoid unused variable warnings.
         */
	
	int i;
	for(i = 0; i < 4;){
		int number = ((unsigned int)random() % 2) + 1;
		while(i < 4 && (numBowl < 3) && ((number == 1 && bowl1 == 0) || (number == 2 && bowl2 == 0))){
        		lock_acquire(bowl);
			if(!bowl1){
				bowl1 = 1;
			}
			else if(!bowl2){
				bowl2 = 1;
			}
			numBowl++;
			lock_eat("mouse", mousenumber, number, i);
			numBowl--;
			if(number == 1){
				bowl1 = 0;
			}
			else if(number == 2){
				bowl2 = 0;
			}
			i++;
			lock_release(bowl);
		}
	}
        (void) unusedpointer;
        (void) mousenumber;
}


/*
 * catmouselock()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up catlock() and mouselock() threads.  Change
 *      this code as necessary for your solution.
 */

int catmouselock(int nargs, char ** args)
{
        int index, error;
	bowl = lock_create("bowl");
	mutux = lock_create("mutux");
	bowl1 = 0;
	bowl2 = 0;
        /*
         * Avoid unused variable warnings.
         */
	
        (void) nargs;
        (void) args;
        /*
         * Start NCATS catlock() threads.
         */

	kprintf("\n");
	
	
	//This is for the cats to create threads
        for (index = 0; index < NCATS; index++) {
           
                error = thread_fork("catlock thread", NULL, index, catlock, NULL);
                
                /*
                 * panic() on error.
                 */

                if (error) {
			panic("catlock: thread_fork failed: %s\n", strerror(error));
                }
        }

        /*
         * Start NMICE mouselock() threads.
         */

	//This is for the mices to create threads
        for (index = 0; index < NMICE; index++) {
   
                error = thread_fork("mouselock thread", NULL, 
                                    index, 
                                    mouselock, 
                                    NULL
                                    );
      
                /*
                 * panic() on error.
                 */

                if (error) {
         
                        panic("mouselock: thread_fork failed: %s\n", 
                              strerror(error)
                              );
                }
        }

        return 0;
}

/*
 * End of catlock.c
 */
