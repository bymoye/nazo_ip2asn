# ip2asn

a [iptoasn](https://iptoasn.com/) tools

Please download the latest version and unzip：
[ip2asn-v4.tsv](https://iptoasn.com/data/ip2asn-v4.tsv.gz) && [ip2asn-v6.tsv](https://iptoasn.com/data/ip2asn-v6.tsv.gz)

## Quickstart

```python

from nazo_ip2asn import Ip2Asn

ip2asn = Ip2Asn(ipv4file="ip2asn-v4.tsv", ipv6file="ip2asn-v6.tsv")

print(ip2asn.lookup(b"8.8.8.8"))

```

记录:

ipasn_v2的构造函数写法。
优点：内存降低(2~3M)
缺点：查询速度降低
所以，沿用原写法。
内存占用: 97M