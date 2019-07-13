# dbust
Dbust is a directory busting tool.

Main purpouse is to discover implict pages by brute-forcing website URI's.

## Main features:
* ultra-fast (4k requests/sec is achievable, it's written in C++ and with nonblocking sockets)
* HEAD request by default
* reconnecting (don't lost tries)
* no more "too much files opened" error
* with control (you preciesly choose how much connections you want)
* low memory footprint
* maintained;)

## Instalation
Compiled dbust is a single ELF executable. It's developed and build on linux and with linux in mind.
But because project uses unix sockets and poll standard - it's possible that with minor changes, 
solution can be compiled and working on other unixes.

### Dependencies
Dbust only dependency is OpenSSL which almost every Linux distribution has installed by default.

### Released Binaries
In releases section on github are shipped binaries with working lastest stable version.

## Do It Yourself
Compilation is actually very easy:

```
git clone https://github.com/technateNG/dbust && cd dbust
cmake .
make
```
After some time compilation should succeed. Executable is in src directory.

## Tips
Dbust is really, really fast and have some catches.  
Executable by default have enabled 100 sockets and delay set on 100 miliseconds. 
This setup should give reasonable speed which doesn't alarm every security solution. But there are scenarios where 
you want uleash full power to get results quickly.

I made some benchmarks which shows me that:
* 1 HTTP socket = 4k requests/second in local network
* 1 HTTPS socket = 12 requests/second in local network

Maximum on my network card was 4k per sec on both HTTP and HTTPS.

I'm pretty sure that this ratios can be improved in futher commits (with multithreading).

For max speed on HTTP you should set:
```
--delay 0 --sockets 10
```

For max speed on HTTPS you should set:
```
--delay 0 --sockets 400
```
## Examples
Local HTTP busting:
```
./dbust --url http://192.168.122.134 --dictionary /home/user/Downloads/DirBuster-Lists/directory-list-2.3-small.txt
```
Output:
```
[*] (0%) 0/87664
/
/manual
[*] (1%) 876/87664
[*] (2%) 1752/87664
[*] (3%) 2628/87664
[*] (4%) 3504/87664
[*] (5%) 4380/87664
/secret
[*] (6%) 5256/87664
[*] (7%) 6132/87664
[*] (8%) 7008/87664
[*] (9%) 7884/87664
/f3
[*] (10%) 8760/87664
[*] (11%) 9636/87664
[*] (12%) 10512/87664
[*] (13%) 11388/87664
```
Local HTTP busting nulled stderr:
```
./dbust --url http://192.168.122.134 --dictionary /home/user/Downloads/DirBuster-Lists/directory-list-2.3-small.txt 2>/dev/null
```
Output:
```
/
/manual
/secret
/f3
```
Local HTTPS busting:
```
./dbust --url https://192.168.122.134 --dictionary /home/user/Downloads/DirBuster-Lists/directory-list-2.3-small.txt
```
Output:
```
[*] (0%) 0/87664
/
/manual
[*] (1%) 876/87664
[*] (2%) 1752/87664
[*] (3%) 2628/87664
[*] (4%) 3504/87664
[*] (5%) 4380/87664
/secret
[*] (6%) 5256/87664
[*] (7%) 6132/87664
[*] (8%) 7008/87664
```
