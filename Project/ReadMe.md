# Voice over RTP

## Problem Statement

We want a system, where voice can be transmitted from one computer to the other over the internet. Here, we also want conference calls to work. For this the different streams of audio coming from different sources have to be synchronised. <br> 
We also want it to work across networks.

## Solution

We here want three main features:<br>
1. Real Time transmission and reception
2. Synchronisation
3. Packet should travel across network (cross the NAT)

For the first feature, we could use UDP. <br>
UDP (User datagram protocol) is a transport layer protocol. It is used mainly for establishing low- latency and loss- tolerating connections between applications on the internet. It also doesn’t support error checking, which saves bandwidth, as compared to TCP. <br>

But, UDP does not carry any timing information, which is required by most real time applications. So we introduce RTP.

<br>
Hence, for the second feature, we use a protocol called RTP. <br>
RTP (Real time transport protocol) is a real time end-to-end transport protocol that runs over UDP. RTP helps recovering lost segments and resequencing of the packets for the application layer. It doesn’t guarantee real time delivery of data but helps manage the data to the best effect. These are common issues during UDP. <br>

#### RTP packet format:
First 32 bits of the header are control bits. 
	•	V, version number = 2, currently
	•	P, padding bit = indicates if there are extra padding bytes at the end of the RTP packet
	•	X, extension bit = Indicates presence of an extension header after the fixed header
	•	CC, count field = number of contributing source identifiers (CSRC) following the fixed header
	•	M, marker bit = Used as a general marker, defined by a profile. If it is set, it means that the current data has some special relevance for the application.
	•	PT, payload type field = Identifies the payload format, which defines application specific extensions or modifications to RTP (A receiver MUST ignore packets with payload types that it does not understand) 
	•	Sequence number = incrementing counter which is started by a source from a random number
	•	Timestamp = generation instant of the first octet in the data packet (helps in synchronization)
	•	Synchronization source identifier (SSRC) = randomly generated value that uniquelly identifies the synchronization source within a session (unique, for one session)
	•	Contributing source identifiers (CSRC) = identifies the contributing sources

As we can see, RTP provides us the payload type identification, source identification, sequence numbering and timestamping, which help in recovering  lost segments and resequencing of the packets.


