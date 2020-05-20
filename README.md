# MarketDataPlayground
Playground for doing market data stuffs. -- Things like reading a websocket stream, creation of automated market maker, stock price visualizations.

Minimum working product for exercising some engineering skills at home.

Project Goals:

1a) Creation of C++ application that can listen on a websocket to a live feed of binary data following the NYSE pillar specification.
    Spec can be found [here](https://www.nyse.com/market-data/real-time/integrated-feed).
    
1b) Emulate real-time data stream by altering NYSE supplied sample .pcap file to use target IP address of my choosing use a tool such as
    UDP/TCP for a faux-live playback.
    Sample data can be found [here](ftp.nyxdata.com/).

2) Creation of binary parser to decode previously mentioned data stream. Only going to decode minimal amount of payloads required to
   visualize price data in the sample.
   
3) Build python frontend for displaying the information. Exploration of different plotting libaries will be main goal of this segment.

4) Make use of IPC technologies to facilitate flow of information to and from disparate processes (C++ -> Python communication).

Future Goals:

5) Further explore scaling capabilities of MQ software by providing different pub/sub brokers to better filter what stocks 
   we want to visualize.

6) Explore containerization technologies. Most likely this will translate to putting the C++, Python, and RabbitMQ server process all
   in different docker images and making sure to keep all functionality intact. 
