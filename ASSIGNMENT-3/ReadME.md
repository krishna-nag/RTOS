# Simulation of VOIP using the PulseAudio Library
<br>
In this assignment, we use sockets, and the PulseAudio library to simulate a VOIP. There are two programs client and server.
Client program records stuff from the microphone and transmits it to the server connected via a socket. The server program, it just plays whatever it receives.
Thus running client program on one system, and server program on the other will help us achieve half duplex communication. For a 
full duplex one, we will have to run ClientA and ServerB on computer A, and ClientB and ServerA on computer B.
