all: finwait_server finwait_server_linger finwait_client

finwait_server: finwait_server.go
	go build  -o $@ $^
finwait_client: finwait_client.go
	go build -o $@ $^
finwait_server_linger: finwait_server_linger.go
	go build -o $@ $^

.PHONY: all
