import socket
import threading
import time
from datetime import datetime
import os
import argparse
import zlib

# For arguments
parser = argparse.ArgumentParser()

parser.add_argument("-f", "--folderpath", help= "specify a folder path for the server to run from", nargs="?", default="")
parser.add_argument("-p", "--port", help="specify a port number to listen on", nargs="?", default = 8080)

args = parser.parse_args()

print(f"running on path: {args.folderpath} and port: {args.port}")
# First, create a socket to listen for incoming connections
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", int(args.port)))
server_socket.listen()

# This function will handle incoming requests
def handle_request(client_socket):
    # Read the request from the client (using Internet Explorer byte size for GET)
    request = client_socket.recv(2083).decode("utf-8")

    # Split the request into lines
    request_lines = request.split("\r\n")

    # Get the request line (the first line of the request)
    request_line = request_lines[0]

    # Split the request line into individual elements
    request_line_elements = request_line.split(" ")

    # Get the method (the first element) and the path (the second element)
    method = request_line_elements[0]
    path = os.path.join(args.folderpath, request_line_elements[1])

    # Get the HTTP headers from the request
    headers = {}
    for line in request_lines[1:]:
        if ": " in line:
            key, value = line.split(": ", 1)
            headers[key] = value
    

    # Check if the path is for a static file
    if os.path.isdir(path[1:]):

        # Prohibit user from accessing upper level directories
        if path.__contains__(".."):
            response_header = "HTTP/1.1 1337 naughty path\r\n"
            client_socket.send(response_header.encode("utf-8"))
            client_socket.close()
            return

        index_path = os.path.join(path[1:], "index.html")
        if os.path.isfile(index_path):
            # Open the index.html file
            with open(index_path, "r") as f:
                # Read the contents of the file
                index_contents = f.read()

        else:
            # Generate an HTML page containing a listing of the files and directories in the directory
            index_contents = generate_listing(path)
        
        # Make a directory list for easier sorting later
        dirlist = []
        for line in index_contents.splitlines():
            if line.__contains__("href="):
                dirlist.append(line.split(sep="'")[1])

        # Check if the client included an "If-Modified-Since" header
        if "If-Modified-Since" in headers:
            # Get the timestamp from the header
            try:
                modified_since = time.strptime(headers["If-Modified-Since"], "%a, %d %b %Y %H:%M:%S")
            except:
                response_header = "HTTP/1.1 400 Bad Request\r\n"
                client_socket.send(response_header.encode("utf-8"))
                client_socket.close()
                return

            # Get the modification time of the file
            file_modified_time = time.gmtime(os.path.getmtime(path[1:]))

            # Check if the file was modified since the timestamp in the "If-Modified-Since" header
            if file_modified_time <= modified_since:
                # If the file has not been modified, send a "304 Not Modified" response
                response_header = "HTTP/1.1 304 Not Modified\r\n"
                client_socket.send(response_header.encode("utf-8"))
                client_socket.close()
                return

        # Check if the client included an "If-Unmodified-Since" header
        if "If-Unmodified-Since" in headers:
            # Get the timestamp from the header
            try:
                unmodified_since = time.strptime(headers["If-Modified-Since"], "%a, %d %b %Y %H:%M:%S")
            except:
                response_header = "HTTP/1.1 400 Bad Request\r\n"
                client_socket.send(response_header.encode("utf-8"))
                client_socket.close()
                return
            # Get the modification time of the file
            file_modified_time = time.gmtime(os.path.getmtime(path[1:]))

            # Check if the file was modified since the timestamp in the "If-Unmodified-Since" header
            if file_modified_time > unmodified_since:
                # If the file has been modified, send a "412 Precondition Failed" response
                response_header = "HTTP/1.1 412 Precondition Failed\r\n"
                client_socket.send(response_header.encode("utf-8"))
                return

        # Send a response header to the client
        response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: {}\r\n".format(len(index_contents))

        response_header = add_headers(response_header, {"Host", "Connection", "User-Agent", "Accept", "Accept-Encoding"}, headers)
    
        # Add the "Last-Modified" header to the response, using the modification time of the file
        if "If-Modified-Since" in headers or "If-Unmodified-Since" in headers:
            file_modified_time_str = time.strftime("%a, %d %b %Y %H:%M:%S %Z", file_modified_time)
            response_header += "Last-Modified: {}\r\n".format(file_modified_time_str)

        # Add an empty line to indicate the end of the headers
        response_header += "\r\n"

        if "Accept-Encoding" in headers:
            if headers["Accept-Encoding"].__contains__("gzip"):
                compressed_contents = zlib.compress(index_contents.encode("utf-8"))
                client_socket.send(response_header.encode("utf-8"))
                client_socket.send(compressed_contents)
                return


        # Send the response header to the client
        client_socket.send(response_header.encode("utf-8"))

        # Send the static content to the client
        client_socket.send(index_contents.encode("utf-8"))

    # Otherwise, send a 404 response
    else:
        response_header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 0\r\n"
        
        send_response_header(client_socket, response_header, {"Host", "Connection", "User-Agent"}, headers, "utf-8")

    # Close the client socket
    client_socket.close()

def send_response_header(client_socket:socket.socket, response_header:str, include_headers:list, headers, encoding:str):
    #Add headers
    response_header = add_headers(response_header, include_headers, headers)
    
    #Standard encoding
    if encoding == "":
        encoding = "utf-8"
    
    client_socket.send(response_header.encode(encoding))
    

def add_headers(response_header:str, include_headers:list, headers):
    #Add requested headers
    for header_name in include_headers:
        if header_name in headers:
            response_header += f"{header_name}: {headers[header_name]}\r\n"

    # Add an empty line to indicate the end of the headers
    response_header += "\r\n"

    return response_header


def generate_listing(path):
    # Generate an HTML page containing a listing of the files and directories in the directory
    listing = "<html><head><title>Directory Listing</title></head><body>\n"
    listing += "  <h1>Directory Listing</h1>\n"
    listing += "    <hr>"
    listing += "<ul>\n"

    for item in os.listdir(path[1:]):
            # Get the last-modified timestamp of the item
        last_modified = os.path.getmtime(os.path.join(path[1:], item))

        # Format the timestamp as a human-readable string
        last_modified_string = datetime.fromtimestamp(last_modified).strftime("%Y-%m-%d %H:%M:%S")

        # Add the item and its last-modified date to the HTML response
        listing += f"      <li><a href='{item}'>{item}</a> (last modified: {last_modified_string})</li>\n"

    listing += "    </ul>"
    listing += "<hr>\n"
    listing += "</body></html>"
    return listing

# Loop forever to handle incoming requests
while True:
    # Accept an incoming connection
    client_socket, client_address = server_socket.accept()

    # Handle the request in a separate thread
    thread = threading.Thread(target=handle_request, args=(client_socket,))
    thread.start()

