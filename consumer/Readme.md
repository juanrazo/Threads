Juan Razo<br/>
Part 2 Threads<br/>
<br/>
<br/>
For this lab I started to review the sections for sleep and wake, producer consumer problem, and semaphores of the book.
The key to solve the multiple producers problem was in section 2.3.5 Semaphores. This section descrives Dijkstra's solution
to the producer consumer problem using the functions up() and down(). I saved the semaphore files under tthreads folder to
keep all of the utilities together under one foler. The algorithm I used for the counting semapore up and down comes from the 
OS book. Essentially all I included to the down() is a subtraction by 1 to the numAvailable after the while loop, as the book
metions. The same for the up(), increase the numAvailable before the if statement. For the binary semaphore I just used the 
mutex provided as the instructions suggest. I created a buffer struct by implementing a queue, the queue code comes from the
website tutorialspoint.com. The code for my producer consumer comes from section 2.3.5 in the OS book. The only difference
is how it produces and consumes the integers, which is just and increment and sum to a variable. To stop the consumer from
consuming once the 4 producers are done I created a flag. Essentially once every producer is done then just increment the
flag. The consumer checks if the flag is equal to 4 (THREADS-1) and the queue is empty after its done an iteration of 
consuming. 

