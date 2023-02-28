## Client/Server Arch using socket in C

#### ADJIBADE Ahmed - Paris 8

### Operations :

✉️ Send message

🧮 Calculate the factorial

📷 Transfert image

### How it works :

To use this process you have to start the server in one terminal and then clients in others.

When a client is connected, the server create a thread to handle all the operations related to this client.

### Instruction to run :

Open a new terminal in at the folder and run :

```
make -B
```

After the binary files are generated, launch one terminal on server side :

```
./server <server-ip-address> <port>
```

when the server terminal display :

```
Server listening on port <port> ...
```

Launch one terminal on client side :

```
./client <server-ip-address> <port>
```

🍀 Enjoy !
