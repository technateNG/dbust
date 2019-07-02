# ![dbust logo](images/dbust2.png) dbust 0.1
Discover implict pages by brute-forcing website URI's.

## ![dbust logo](images/dbust2.png) Main features:
* ultra-fast (written in C++, nonblocking sockets, poll)
* HEAD request by default
* reconnecting until succeeds 
* no more "too much files opened" error
* low memory footprint

## ![dbust logo](images/dbust2.png) Instalation
Compiled program is a single ELF executable. Only supported OS is Linux.

### Dependencies
Program only dynamic dependency is OpenSSL.

### Released Binaries
Every release has shipped binaries on project releases page [HERE](https://github.com/technateNG/dbust/releases).

## ![dbust logo](images/dbust2.png) Do It Yourself
Compilation is actually very easy:

```
git clone https://github.com/technateNG/dbust && cd dbust
cmake -DCMAKE_BUILD_TYPE=Release .
make
```
After some time the compilation should succeed. Executable is in the src directory.

If you want to contribute to the project than you should compile the tests as well.
Tests executable has separate compilation path then normal executable.
To do that, options need to be set like this:
```
cmake -DCMAKE_BUILD_TYPE=Debug -DMAKE_TESTS=ON .
```
Compiled dbust-tests binary should be in the test directory.
## ![dbust logo](images/dbust2.png) Examples
Help:
```
Usage: dbust [OPTION...]

-u, --url <string> (*)                                               target url
-d, --dictionary <string> (*)                                        path to dictionary
-s, --sockets <int> (100)                                            number of concurrent sockets
-t, --timeout <int> (10)                                             duration in seconds to reconnect try
-e, --file-extensions <[string]> ()                                  file extensions to search for
-c, --status-codes <[string]> (200, 201, 400, 401, 403, 500)         valid status codes
-y, --delay <int> (100)                                              delay in milis between requests
--user-agent <string> (dbust)                                        request user agent
--get                                                                use GET instead HEAD
-h, --help                                                           this help

Examples:
dbust -u https://www.example.com/dir/ -d /home/user/dict.txt
dbust -c '200,201,400,401,403,500' -e 'php,txt' -s 1000 -u http://192.168.0.92:8080/ -d /home/user/dict.txt
```
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
## ![dbust logo](images/dbust2.png) In future:
* Cleaner code and better tests coverage
* Multithreading support
* Recursive search

## ![dbust logo](images/dbust2.png) Licence
This application is open sourced under GNU Public Licence ver. 3.
