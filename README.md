# ESB
This project is to solve congestion and overloading problem on network services with large number of clients’ requests with following functionalities:
-	Dynamic connection binding. The Hub would offer centralized service management. All the servers in a group would register themselves on the Hub. Thus, a client can query different servers available from one end-point, the Hub.
-	Load Balancing. Servers with the same functionalities can be registered on the same Hub. The Hub would order the list of the registered servers by their load status automatically. Therefore, the Hub knows the idlest servers in the lists of different type of services. Thus, when a client sends a server-connection request to the Hub with a service ID, the Hub would respond the client with the idlest service from the service list of registered servers.
-	High Level Application Program Interfaces (APIs) for developers. Developer can use this set of APIs to design C/S structure applications with the benefit of the system, including managed load control and one-end-point connection binding. Programmer would never care with the detail network protocol and communication with a Hub, since these would be hidden by the APIs.  

1. Development
  - Language: VC++ with Visual Studio 2015
  - Thirdparty Library: gSoap
2. Target Platform
  - Windows 7 or higher
3. Test Samples
  - ESB/samples/database query/
4. Test Results
  - see Report.pdf
