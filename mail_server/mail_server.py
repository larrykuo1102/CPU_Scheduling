from distutils.command.clean import clean
import socket




helo_message = 'HELO {}\r\n'.format(socket.gethostname())

mailfrom_message = 'MAIL FROM: {}\r\n' #.format('larrykuo.tech@gmail.com')

rept_message = 'RCPT TO: {}\r\n' #.format('m11115046@mail.ntust.edu.tw')

data_message = 'DATA\r\n{}.\r\n' #.format('Hi \r\n It\'s a test mail\r\n testing my localhost mail server\r\n')

quit_message = 'QUIT\r\n'



# socket.AF_INET

# mailserver = ('localhost', 25)
def send_mail() :
    

    # host = socket.gethostname()  # as both code is running on same pc
    port = 25  # socket server port number
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # client_socket = socket.socket()  # instantiate
    client_socket.connect(('localhost', port))  # connect to the server
    response = client_socket.recv(1024).decode()
    print( "Server response: ",response[4:] )
    if ( response[:3] != '220' ) :
        raise 
    
    print('C: ', helo_message)
    client_socket.send( helo_message.encode())

    response = client_socket.recv(1024).decode()
    print( "Server response: ",response[4:] )
    if ( response[:3] != '250' ) :
        raise
    # print( "Server response: ", client_socket.recv(1024).decode()) # helo response


    user_input = 'larrykuo.tech@gmail.com'
    print('C: ', mailfrom_message.format(user_input))
    client_socket.send(mailfrom_message.format(user_input).encode())

    response = client_socket.recv(1024).decode()
    print( "Server response: ",response[4:] )
    if ( response[:3] != '250' ) :
        raise 

    user_input = input("What is the address yout want to send?\n")
    print('C: ', rept_message.format(user_input))
    client_socket.send(rept_message.format(user_input).encode())

    response = client_socket.recv(1024).decode()
    print( "Server response: ",response[4:] )
    if ( response[:3] != '250' ) :
        raise

    user_input = input("What is the content you want to say\nEnd with enter \'quit\'\n")
    # print(user_input)
    data = ''
    while( user_input != 'quit' ) :
        data += user_input + '\r\n'
        user_input = input()
        # print(user_input)
        
    print( 'C: ', data_message.format(data) )
    client_socket.send(data_message.format(data).encode())

    response = client_socket.recv(1024).decode()
    print( "Server response: ",response )

    
    print('C: ', quit_message)
    client_socket.send(quit_message.encode())

    response = client_socket.recv(1024).decode()
    print( "Server response: ",response[4:] )

try :
    send_mail()
    print("Success send!")
except ValueError as e :
    
    user_input = input("Failed!\nDo you want to try again?(y/n)")
    if ( user_input.capitalize() == 'Y' ) :
        send_mail()
    else :
        quit()
