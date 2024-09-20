import socket

def udp_server(host='127.0.0.1', port=12345):
    # Create a UDP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # Bind the socket to the address and port
    server_address = (host, port)
    print(f'Starting UDP server on {server_address}')
    sock.bind(server_address)

    try:
        while True:
            # Wait to receive a message
            print('Waiting to receive a message...')
            data, address = sock.recvfrom(4096)  # Buffer size is 4096 bytes

            print(f'Received {len(data)} bytes from {address}')
            print(data.decode('utf-8'))

            # Optionally send a response
            if data:
                sent = sock.sendto(data, address)
                print(f'Sent {sent} bytes back to {address}')

    except KeyboardInterrupt:
        print('UDP server shutting down')

    finally:
        # Close the socket
        sock.close()

if __name__ == '__main__':
    udp_server()

