## Take home
### Question 1

Server uses a library called inotify, which generates events when any file in the directory is modified.
Server looks for such events, and when all sensor files are modified, the data is bundles, and sent along with timestamp to the client.
Client recieves this, and writes it into the csv file.

Sample output :

Fri Mar 22 15:26:54 2019,99,53,1520,1740,2266, <br>
Fri Mar 22 15:26:55 2019,99,53,1520,1740,2266, <br>
Fri Mar 22 15:26:55 2019,99,53,1520,1740,2266,<br>
Fri Mar 22 15:26:55 2019,66,67,1863,2563,2035,<br>
Fri Mar 22 15:26:57 2019,57,73,1222,1915,2270,<br>
Fri Mar 22 15:26:59 2019,79,61,1229,2312,2853,<br>
Fri Mar 22 15:27:01 2019,92,67,1209,1694,2327,<br>
Fri Mar 22 15:27:03 2019,64,78,1650,2872,2287,<br>
Fri Mar 22 15:27:05 2019,74,54,1256,2757,2764,<br>
Fri Mar 22 15:27:07 2019,67,56,1100,1829,2725,<br>
Fri Mar 22 15:27:09 2019,87,55,1748,2387,2390,<br>
Fri Mar 22 15:27:11 2019,59,71,1157,2459,2580,<br>
Fri Mar 22 15:27:13 2019,85,79,1111,2130,2577,<br>
Fri Mar 22 15:27:15 2019,71,83,1701,2841,2810,<br>
Fri Mar 22 15:27:17 2019,73,82,1662,2559,2971,<br>
Fri Mar 22 15:27:19 2019,69,59,1478,1642,2683,<br>
Fri Mar 22 15:27:21 2019,80,62,1811,2652,2581,<br>
Fri Mar 22 15:27:23 2019,63,52,1867,2473,2602,<br>
Fri Mar 22 15:27:25 2019,79,63,1884,2230,2937,<br>
Fri Mar 22 15:27:27 2019,58,68,1923,1834,2851,<br>
Fri Mar 22 15:27:29 2019,87,85,1882,2650,2446,<br>
Fri Mar 22 15:27:31 2019,63,89,1022,2074,2792,<br>
Fri Mar 22 15:27:33 2019,50,76,1345,2839,2195,<br>


We can see that after it gets calibrated initially, we can see a time stamp every 2 seconds.
