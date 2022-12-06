FMpool: 
    g++ FMpool.cpp common.cpp -o FMpool.o

client: 
    g++ client.cpp  common.cpp -o client.o

runPool_rdma: 
	sudo LD_PRELOAD=$libvma VMA_SPEC=latency ./FMpool.o 20.20.20.21 7560

runC_rdma: 
	sudo LD_PRELOAD=$libvma VMA_SPEC=latency ./client.o 20.20.20.110 7573 20.20.20.21 7560

runPool_normal: 
	./FMpool.o 20.20.20.21 7559

runC_normal: 
	./client.o 20.20.20.110 7572 20.20.20.21 7559

runPool_test: 
	./FMpool.o 127.0.0.1 7661

runC_test: 
	./client.o 127.0.0.1 8661 127.0.0.1 7661

clean: 
    rm FMpool.o client.o

##192.168.1.151 ./FMpool.o 192.168.1.151 7430
##192.168.1.35  ./client.o 192.168.1.35 8270 192.168.1.151 7430