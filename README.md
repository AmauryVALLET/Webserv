# WebServ
this project aims to implement an HTTP server using C++


Usefull curl command lines

MAX BODY SIZE
	curl -X POST -H "Content-Type: plain/text" --data "1234567890" http://localhost:8080/test/maxbody

TEST CHUNK POST file
	curl -v -H 'Transfer-Encoding: chunked' -H 'Content-Type: text/html' --data-binary '@www/index.html' http://127.0.0.1:8080/test/test.txt

DELETE file
	curl -X DELETE http://localhost:8080/upload/file/1a-ark-nova-regle.pdf

Valgrind
	valgrind ./webserv config_files/test.config   
