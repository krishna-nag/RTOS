# EchoEngine

The purpose of echo engine is to create a server, which when receives a character, it replies to the process, the same character by inverting the case. If its not an alphabet, it will just return the same character. 
This is acheived here by having one message queue for the server to which all the client requests will come. And, each client has its own message queue to which the server will reply.
The server uniquely is able to identify the client to reply to, by looking at the mtype field.

# Performance statistics

Performance was analysed by recording the time taken to process a request sent by client A. So, first, only client A kept requesting the server for about 20 seconds. The time taken to process these requests were being written to a file. So, in 20 senconds, lakhs of requests were made, and all these times were written in the file. I averaged the value of the times in this file, and came to a conclusion as to how much time was taken on an average by the server to handle client A's request.
This same thing was done again, when parallely client B was running. Average was recorded. Again it was done when B and C were running in parallel. Similarly until all 5 clients were requesting in parllel, the process was repeated, and the obtained values are as follows:

| # number of Clients | Time taken(micro seconds)  | 
|---------------------|-------------|
| 1                   | 3.6473      |
| 2                   | 3.9750      |
| 3                   | 5.8663      | 
| 4                   | 6.6631      | 
| 5                   | 6.9763      | 


As we can see, the time taken when multiple clients are requesting at the same time increases. This is because server is now busy, and is not readily available to immediately handle the request.
