FMpool: 
    g++ FMpool.cpp common.cpp -o FMpool.o

client: 
    g++ client.cpp  common.cpp -o client.o

runPool_rdma: 
	sudo LD_PRELOAD=$libvma VMA_SPEC=latency ./FMpool.o 20.20.20.21 7559

runC_rdma: 
	sudo LD_PRELOAD=$libvma VMA_SPEC=latency ./client.o 20.20.20.22 7572 20.20.20.21 7559

runPool_normal: 
	./FMpool.o 20.20.20.21 7559

runC_normal: 
	./client.o 20.20.20.110 7572 20.20.20.21 7559

clean: 
    rm FMpool.o client.o