# CS 33211 Producer-Consumer Problem

This project implements the Producer-Consumer problem using two separate processes that operate through shared memory. The point of this project is to use proper synchronization techniques to accomplish this.

My implementation is based on the Chapter Five slides (slide 64) and uses two counting semaphores and a binary semaphore.

In total, the producer generates a total of ten items, placing them in a circular buffer with two slots, while the consumer retrieves these items. The semaphores guarantee that the producer waits when the buffer is full and the consumer waits when it is empty, preventing race conditions. A random sleep is also implemented so that the code doesn't complete immediately, this is to make it more "satisfying" to watch, make it easier to see what step is currently happening, and to introduce some randomness into the turn of events.

All three files are commented in more depth to describe what the program is doing on a line-by-line basis and why certain design choices were made.

## To Compile and Run

Run these commands in your Linux terminal.

```bash
gcc producer.c -pthread -lrt -o producer
gcc consumer.c -pthread -lrt -o consumer
./producer & ./consumer
```

## Examples

![image](https://github.com/user-attachments/assets/83f1785e-3c2f-4faf-907a-a6c575dcb3e0)

![image](https://github.com/user-attachments/assets/82799f8b-f251-41ca-933d-8996841f2457)

![image](https://github.com/user-attachments/assets/164ffd2e-12df-4e47-9b19-2fcf006102a5)

## Problems

The code doesn't execute on Wasp, but I am assuming that is because making semaphores on the machine is prohibited. Which would make sense since it seems like you could cause some trouble!
