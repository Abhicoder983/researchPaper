#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/wifi-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("IoTUAVAuthExample");

// Simulated authentication delay (in ms)
double authDelayMs = 13.0;

// Function to simulate authentication
void SimulateAuth ()
{
  NS_LOG_UNCOND ("Authentication completed at time: " 
                 << Simulator::Now ().GetMilliSeconds () << " ms");
}

int main (int argc, char *argv[])
{
  uint32_t nDevices = 10;

  CommandLine cmd;
  cmd.AddValue ("nDevices", "Number of IoT devices", nDevices);
  cmd.Parse (argc, argv);

  // Create nodes
  NodeContainer iotNodes;
  iotNodes.Create (nDevices);

  NodeContainer uavNode;
  uavNode.Create (1);

  // WiFi setup
  WifiHelper wifi;
  wifi.SetStandard (WIFI_STANDARD_80211b);

  YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  phy.SetChannel (channel.Create ());

  WifiMacHelper mac;
  mac.SetType ("ns3::AdhocWifiMac");

  NetDeviceContainer iotDevices = wifi.Install (phy, mac, iotNodes);
  NetDeviceContainer uavDevice = wifi.Install (phy, mac, uavNode);

  // Mobility
  MobilityHelper mobility;
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (iotNodes);

  mobility.SetMobilityModel ("ns3::ConstantVelocityMobilityModel");
  mobility.Install (uavNode);

  // Internet stack
  InternetStackHelper stack;
  stack.Install (iotNodes);
  stack.Install (uavNode);

  Ipv4AddressHelper address;
  address.SetBase ("10.1.1.0", "255.255.255.0");

  Ipv4InterfaceContainer iotIf = address.Assign (iotDevices);
  Ipv4InterfaceContainer uavIf = address.Assign (uavDevice);

  // UDP Server (UAV)
  uint16_t port = 8080;
  UdpServerHelper server (port);
  ApplicationContainer serverApp = server.Install (uavNode.Get (0));
  serverApp.Start (Seconds (1.0));
  serverApp.Stop (Seconds (10.0));

  // UDP Clients (IoT Devices)
  for (uint32_t i = 0; i < nDevices; i++)
  {
    UdpClientHelper client (uavIf.GetAddress (0), port);
    client.SetAttribute ("MaxPackets", UintegerValue (1));
    client.SetAttribute ("Interval", TimeValue (Seconds (1.0)));
    client.SetAttribute ("PacketSize", UintegerValue (190)); // message size

    ApplicationContainer clientApp = client.Install (iotNodes.Get (i));
    clientApp.Start (Seconds (2.0 + i * 0.1));
    clientApp.Stop (Seconds (10.0));
  }

  // Schedule authentication simulation
  Simulator::Schedule (MilliSeconds (authDelayMs), &SimulateAuth);

  Simulator::Stop (Seconds (10.0));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}