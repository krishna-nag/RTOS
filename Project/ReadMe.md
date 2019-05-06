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

#### Server to make it work across network

We are runnong a server in digital ocean, which takes packets from one source and sends it to all the other connected sockets.

## 1.0 Implementation:

|-------------------------------------------------------|
|----UDP_Header----|-----RTP Header-----|----Message----|
|-------------------------------------------------------|

We used pulseaudio for recording and playback purposes on C. 
The need for RTP was already mentioned before. And the header is how we use it to help us overcome the difficulties of UDP. Most times RTP works out when we use it for video conferencing. But since we are only majorly focussing on voice calls we have not implemented the RTCP part of the protocol. RTCP deals with maintainence of the quality of service, which is not of major worry to us. 
The buffer that we send across the network is a character array. So we identify the header values and concatenate them to an empty string and finally add in the message and pass it onto the sendto() function call. 
Whatever pulseaudio records acts as the message part of the packet. We then use the RTP header to implement synchronization amongst packets and to identify the source. 
After that we play t number of packets from the same source and then move onto the next source. In a round robin fashion. Various numbers had to be attempted before we zeroed in on the optimal number. If the number is too small then the voice cuts too much. And if it's too much then the lag might be too much.

We first imlemented a simple UDP version of the assignment. So instead of using TCP we used UDP. Once that worked, we built on it and implemented the RTP version of it. 
In all the cases we first tried text out and then moved onto pulseaudio.

## 1.1 Gaps in Implementation:

In our application most of the RTP headers were not being taken advantage of. And so we did not implement those. The header bits that we have implemented are only source ID and sequence number. To ensure that the right packet is played one after the other. 
But we think that it shouldn't be too hard to extend this to implement all the headers as RTP requires. 

## 1.2 Challenges Faced:

This is going to be a bit tiring for the reader. We faced way too many challenges and some which we are not sure why it suddenly stopped being a challenge. 
1. Identifying and removing the header proved to be such a challenge that we cracked only at the very last minute. We were using string copying to do that, but later we realized instead of doing that if we were to take advantage of the pointer implementing the received and sent buffer then it shouldn't be this hard. And that's what we did finally.
2. Once we identified how to do this there were issues with pulseaudio since now at a given time BUFSIZE-HEADER_SIZE bytes were to be captured in the first place. 
3. We attempted to introduce the time stamp for the message into the header. But since we are working with a string for the buffer being sent across the network, we were unaware of how to appropriately encode it such that all information with millisecond precision could be captured.
4. Stitching together the correct packets at the server end was harder than usual. Even figuring out that the packets received were different was real difficult. Majorly because it always worked seamlessly with text. Then finally we used a text buffer of exactly BUFSIZE characters to see what happens to the packet when we send it or modify it.
5. We think there is some shortcoming in terms of our equipment. So some of our testing was not fruitful. There was some delay and a lot of cuts when we had more than one person talking. Even if there is more than one client connected the same issue occurs.

Identifying these tiny errors is what we spent most of our time on. The underlying algorithm and code to build on was already available to us thanks to Prof. Girish's assignments. 

(Perhaps this section wasn't as long as we expected it would be!)






