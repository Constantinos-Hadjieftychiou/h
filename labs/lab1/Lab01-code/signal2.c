/** A program that will continually alarm every 1 second.
 *
 *  We would need to reset the alarm once the signal is delivered. The natural place
 * to do that is in the signal handler.
 * After the first SIGALRM is delivered and "tick" is printed, another alarm is schedule
 * via alarm(1). The process will resume after the pause(), but since it is in a loop, it
 * will return to a suspended state. The result is an alarm clock buzzing every 1
 * second. Running with time utility, after about 4 seconds, we saw 4 tick.
 *
 */

#include <stdio.h>
#include <signal.h>

void tick(int sig)
{
  printf("tick\n");
  alarm(1); // set a new alarm for 1 second
}

int main()
{
  // set up alarm handler
  signal(SIGALRM, tick);

  // schedule the first alarm
  alarm(1); // set an alarm clock for delivery of a signal

  // pause in a loop
  while (1)
  {
    pause();
  }
  return 0;
}
