import socket
import os
import cv2
import time

# Server configuration
server_ip = '192.168.43.24'
server_port = 12345

while True:
    # Capture a photo using OpenCV (you need to have OpenCV installed)
    camera = cv2.VideoCapture(0)
    ret, frame = camera.read()
    if ret:
        timestamp = int(time.time())
        filename = f"captured_photo_{timestamp}.jpg"
        cv2.imwrite(filename, frame)
    else:
        print("Failed to capture a photo.")
        exit(1)

    # Connect to the server
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((server_ip, server_port))

    # Send the captured photo to the server
    with open(filename, "rb") as file:
        data = file.read(1024)
        while data:
            client_socket.send(data)
            data = file.read(1024)

    print(f"Photo {filename} sent to the server.")

    # Close the client socket
    client_socket.close()

    # Wait for one second before capturing the next photo
    time.sleep(1)
