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

As you can see in every example the producer never produces more than two items without the consumer consuming atleast one item. And the consumer never consumers more than two items without the producer producing one. You can also observe that the indexes at which items are produced and consumed is consistent with a circular buffer and eachother. Because of the random sleep intervals the two dont work in strict alternating fashion this is by design. So the bounded buffer of two is working as expected and the producer and consumer are executing as expected. You can also see that in every example they both execute ten times each as that is the amount of items that are in total introduced to the buffer. So the examples show that the code is working as expected.

![image](https://github.com/user-attachments/assets/58a870e8-0416-4d67-90a0-629ed3a4f2e5)

![image](https://github.com/user-attachments/assets/2104d957-d425-4c79-9bea-9c6360693fde)

![image](https://github.com/user-attachments/assets/97ceb30c-ef41-4fb9-87d6-1336d9c4a2c9)

## Problems

The code doesn't execute on Wasp, but I am assuming that is because making semaphores on the machine is prohibited. Which would make sense since it seems like you could cause some trouble!
