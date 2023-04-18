import socket
import argparse
import time


def client_program( arg ):
    if ( int(arg.port[0]) > 65535 or int(arg.port[0]) < 1024 ) :
        print( "Error: port number must be in the range 1024 to 65535" )
        return
    text = '0'*1000
    text = text.encode()
    # host = socket.gethostname()  # as both code is running on same pc
    host = arg.host[0]
    port = arg.port[0] 
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print( "connect ", host, "port", port )
    client_socket.connect((host, port))  # connect to the server
    now_time = time.time()
    amount = 0
    while( time.time() -  now_time < arg.time[0] ) :
        # print( time.time() -  now_time  )
        client_socket.send(text) 
        amount += 1
        client_socket.recv(1024)
    
    finish_messgae = "sent: " + str(amount) + " kb rate: "
    
    if ( amount/arg.time[0] > 1000 ) :
        amount /= 1000
        finish_messgae += str(amount/arg.time[0]) + " mbps" 
    else :
        finish_messgae += str(amount/arg.time[0]) + " kbps" 
    
    
    print( finish_messgae )
    client_socket.close()  # close the connection


def server_program( arg ) :
    if ( int(arg.port[0]) > 65535 or int(arg.port[0]) < 1024 ) :
        print( "Error: port number must be in the range 1024 to 65535" )
        return
    print(arg.port[0] )
    port = arg.port[0] 

    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  # get instance
    
    server_socket.bind(('127.0.0.1', port))  # bind host address and port together
    print( "open host:", '127.0.0.1', "port", port )

    server_socket.listen(1)
    conn, address = server_socket.accept()  # accept new connection
    print("Connection from: " + str(address))
    amount = 0
    now_time = time.time()
    while True:
        data = conn.recv(1024)
        amount += 1
        if not data:
            amount -= 1
            break
        # print("from connected user: " + str(len(data)))
        conn.send(("get " + str( len(data))).encode())  # send data to the client
    finish_time = time.time() - now_time
    print( finish_time )
    finish_message = 'recieved ' + str(amount) + ' kb, ' 
    if ( amount / finish_time > 1000 ) :
        amount = amount / 1000
        finish_message +=  str(amount / finish_time) + " mbps"
    else :
        finish_message +=  str(amount / finish_time) + " kbps"
    print( finish_message )
    conn.close()  # close the connection
    
def main() :
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument("-s", "--server", action="store_true")
    parser.add_argument("-c", "--client", action="store_true")
    parser.add_argument("-p", "--port", nargs=1, type=int, default=[3000] )
    parser.add_argument("-t", "--time", nargs=1, type=int )
    parser.add_argument("-h", "--host", nargs=1, type=str, default='127.0.0.1' )
    args = parser.parse_args()
    if ( args.server ) :
        # print( args.time )
        # print( args.port )
        # print( args.host )
        print("Start server")
        server_program(args)
    elif ( args.client) :
        print("client send", args.time[0], "second")
        client_program(args)
    else :
        print("Error: missing or additional arguments")
    pass
    
if __name__ == '__main__' :
    main()