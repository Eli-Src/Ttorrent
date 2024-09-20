from collections import OrderedDict
import socket
import os
import bencodepy

def decode_compact_ip_port(data):
    # Extract the IP address (first 4 bytes)
    ip_bytes = data[:4]
    # Extract the port number (next 2 bytes)
    port_bytes = data[4:]

    # Convert bytes to integers
    ip_address = '.'.join(str(byte) for byte in ip_bytes)
    port_number = int.from_bytes(port_bytes, byteorder='big')

    return ip_address, port_number

def run_client():
    # Define the server address and port
    server_address = 'router.bittorrent.com'
    server_port = 6881
    node_id = os.urandom(20)
    node_id = "abcdefghij0123456789"

    # Create a TCP/IP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    magnet_id = "AFBQLEF3NJ24V4XWWJKFUJRMQKM5OSMQ"
    test = {
        "t": "ohmygawd", 
        "y": "q", 
        "q": "get_peers", 
        "a": {
            "info_hash": magnet_id, 
            "id": node_id
        }
    }

    try:
        # Connect the socket to the server
        client_socket.connect((server_address, server_port))
        print(f"Connected to {server_address} on port {server_port}")

        # Send a message to the server
        message = bencodepy.encode({"t":"aa", "y":"q", "q":"ping", "a":{"id":"abcdefghij0123456789"}})
        client_socket.sendall(message)
        print(f"Sent: {message}")

        # Receive a response from the server
        response = client_socket.recv(10240)
        print(f"Received:")
        print(response)
        # decoded = bencodepy.decode(response)
        # for i in decoded:
        #     if type(decoded[i]) == OrderedDict:
        #         for j in decoded[i]:
        #             print(j, "\t", decoded[i][j])
        #     else: print(i, '\t', decoded[i])

    finally:
        # Close the socket connection
        client_socket.close()
        print("Connection closed")

if __name__ == "__main__":
    run_client()
