# ucsc-cmpe252a-dht
This project implements a distributed hash table as described in the chord protocol. This is a part of the CMPE252A a.k.a Computer Netoworks course requirement at UC Santa Cruz. 

**Prerequisites**
- Install openssl with
`brew install openssl`
- Update $(LDFLAGS) $(CPPFLAGS) to the path openssl is installed
`export LDFLAGS="-L/usr/local/opt/openssl/lib"`
`export CPPFLAGS="-I/usr/local/opt/openssl/include"`
`export PATH="/usr/local/opt/openssl/bin:$PATH"`

**Building**
The binary is generated with make. `make chord`
 Run the program without arguments as `./chord`
To clean run, `make clean`

**Vim settings**
`set tabstop=4`
`set shiftwidth=4`
`set expandtab`