from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.log import setLogLevel

class SingleSwitchTopo(Topo):
    def __init__(self, n=2, **opts):
        Topo.__init__(self, **opts)
        h1 = self.addHost('h1', cpu=.5/n)
        h2 = self.addHost('h2', cpu=.5/n)
        self.addLink(h2, h1, bw=200, delay='100ms', loss=1)

def perfTest():
    topo = SingleSwitchTopo(n=2)
    net = Mininet(topo=topo,host=CPULimitedHost, link=TCLink)
    net.start()
    h1, h2 = net.get('h1', 'h2')
    net.iperf((h1, h2), seconds=60)
    net.stop()

if __name__=='__main__':
    setLogLevel('info')
    perfTest()