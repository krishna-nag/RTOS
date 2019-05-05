# EchoEngine: Socket

The purpose of Echo engine is to create a server, which when receives a character, replies by reversing the case.
If it is a special character, then it replies the same.

# Performance statistics

Performance was analysed in a similar way to the Echo engine. Sample files were created, different ones for each client. The clients would continuosly keep requesting that particular file, and be getting that.
This was agin done with increasing number of simultaneous clients.

| # number of Clients | Time taken(micro seconds)  | 
|---------------------|-------------|
| 1                   | 30.8245      |
| 2                   | 47.5641      |
| 3                   | 72.5864      | 
| 4                   | 91.0885      | 
| 5                   | 104.2113      | 


As we can see, the time taken when multiple clients are requesting at the same time increases. This is because server is now busy, and is not readily available to immediately handle the request.
