Utilities_ChannelManager
========================

The m+m manager application displays a single window view of the connections within a YARP network, with features designed to make management of an m+m installation easier. Simple YARP network ports are shown as rectangles with a title consisting of the IP address and port number of the port, and the YARP name for the port as the body of the rectangle, prefixed with ‘In’ for input–only ports, ‘Out’ for output–only ports and ‘I/O’ for general ports.

m+m services are shown as rectangles with a title consisting of the name provided by the service, with the primary YARP network connection as the first row in the body of the rectangle, prefixed with ‘S’ to indicate that it is a service connection. Secondary YARP network connections appear as rows below the primary connection, prefixed with ‘In’ for input–only connections and ‘Out’ for output–only connections. m+m Input / Output services do not have a visual appearance that is distinct from other m+m services – the connections that are allowed, however, are more restricted. Both m+m services and clients can have multiple secondary YARP network ports.

m+m simple clients are shown as rectangles with a title consisting of the IP address and port number of their connection to a service, with a row containing the YARP network connection prefixed with ‘C’. m+m adapters are similar to m+m simple clients, except that they have additional rows above the client–service YARP network connection for the secondary YARP network connections, with prefixes of ‘In’ for input–only connections and ‘Out’ for
    output–only connections.

Connections between ports are shown as lines with one of three thicknesses and one of three colours. The thinnest lines represent simple YARP network connections, which have no explicit behaviours. The middle thickness lines represent connections between Input / Output services; these connections have specific behaviours. The thickest lines represent connections between clients and services, which are not modifiable by this tool. TCP/IP connections, which are the default, are shown in teal, UDP connections are shown in purple and other connections are shown in orange. Note that the tool can only create TCP/IP or UDP connections.

Note that the m+m manager application requires an m+m installation, and will not execute properly unless there is a 'reachable' YARP server running.
