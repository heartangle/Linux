
.PHONY:all
all:select_server select_client
select_client:select_client.cc
	g++ -o $@ $^
select_server:select_server.cc
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f select_server select_client
