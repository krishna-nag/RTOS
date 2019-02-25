# DatabaseEngine

The purpose of Database engine is to create a server, which when receives a file request, it replies to the process, with the string that contains the file data.
If the file is larger than the message storage, then the file is sent in chunks of 200 bytes. The algorithm is very similar to Echo engine, but here we are sending files, which is more realistic.

# Performance statistics

Performance was analysed in a similar way to the Echo engine. Sample files were created, different ones for each client. The clients would continuosly keep requesting that particular file, and be getting that.
This was agin done with increasing number of simultaneous clients.

| # number of Clients | Time taken(micro seconds)  | 
|---------------------|-------------|
| 1                   | 13.67      |
| 2                   | 15.55      |
| 3                   | 17.32      | 
| 4                   | 18.15      | 
| 5                   | 19.78      | 


As we can see, the time taken when multiple clients are requesting at the same time increases. This is because server is now busy, and is not readily available to immediately handle the request.
