## Open Book Exam
### Question 2

sample_usage.c shows an example usage of the coroutine definition, by taking a simple example.
snakes_ladders.c shows the implementation of snakes and ladders using coroutines.

To compile, use: gcc snakes_and_ladders.c -o sn -lpthread -lrt.

The "-lpthread -lrt" flag is required, when using semaphores.

Sample output of snake and ladder program. (This keeps changing with every turn).

\# # # # # # # # # # <br>
\# # # # # # # # # # <br>
\# # # # # # # # # # <br>
\# # # # # # 1# # # # <br>
\# # # # # # # # # # <br>
\# # # # # # # # # # <br>
\# # # # # # # # # # <br>
\# # # # # # # # # # <br>
\# # # # # # # # 3# # <br>
\# # # 2# # # # # 4# <br>
