# ucsc-cmpe252a-dht
This project implements a distributed hash table as described in the chord protocol. This is a part of the CMPE252A a.k.a Computer Netoworks course requirement at UC Santa Cruz. 

**Prerequisites**
1. Install openssl

On mac,
`brew install openssl`

2.Update $(LDFLAGS) $(CPPFLAGS) to the path openssl is installed

**Building**
The binary is generated with make. Run the program without arguments
`make chord`
`./chord`

And to clean run,
`make clean`



**Mandatory vim settings to contribute:**
set tabstop=4
set shiftwidth=4
set expandtab