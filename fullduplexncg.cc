#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include   "ns3/point-to-point-module.h"
#include   "ns3/applications-module.h"
#include "ns3/netanim-module.h"
// Default Network Topology
//
// 10.1.1.0
// n0	n1
// point-to-point

// run the program ./waf --run scratch/fullduplexncg
//
using namespace ns3;
NS_LOG_COMPONENT_DEFINE    ("FirstScriptExample");
int
main (int argc, char *argv[])
{
//CommandLine cmd (__FILE__);
//cmd.Parse (argc, argv);
bool tracing = true;
Time::SetResolution (Time::NS);
LogComponentEnable   ("UdpEchoClientApplication",   LOG_LEVEL_INFO);
LogComponentEnable   ("UdpEchoServerApplication",   LOG_LEVEL_INFO);
NodeContainer nodes;
nodes.Create (2);
PointToPointHelper pointToPoint;
pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("10Mbps"));
pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
NetDeviceContainer devices;
devices = pointToPoint.Install (nodes);
InternetStackHelper stack;
stack.Install (nodes);
Ipv4AddressHelper address;
address.SetBase ("10.1.1.0", "255.255.255.0");
Ipv4InterfaceContainer interfaces = address.Assign   (devices);
UdpEchoServerHelper echoServer (9);
ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
serverApps.Start (Seconds (1.0));
serverApps.Stop (Seconds (10.0));
UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
echoClient.SetAttribute ("MaxPackets", UintegerValue (4));
echoClient.SetAttribute  ("Interval", TimeValue (Seconds (1.0)));
echoClient.SetAttribute ("PacketSize", UintegerValue (1024));
ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
clientApps.Start (Seconds (2.0));
clientApps.Stop (Seconds (10.0));
AnimationInterface anim("1.xml");
anim.SetConstantPosition(nodes.Get (0), 10.0, 10.0);
anim.SetConstantPosition(nodes.Get (1), 20.0, 30.0);
if (tracing==true)
{
pointToPoint.EnablePcapAll("p2p");
}
Simulator::Run ();
Simulator::Destroy ();
return 0;
}
