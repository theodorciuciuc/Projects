I used the structure mesaj_TCP to communicate with the TCP clients. It has a
field type, to know which command the client gave, a field len for the total
length of the packet and the field payload for the actual data. The server
open a TCP socket for the clients and an UDP socket for the programs that
generate traffic for the TCP clients. If an UDP packet is received, the server
parses the data and makes a human-readable string to send to the TCP clients
that are subscribed to that topic.

The TCP clients connect to the server, read commands from the user and show
the contents of the topics they are subscribed to.


