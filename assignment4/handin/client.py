import socket
import time
import argparse
import zlib

parser = argparse.ArgumentParser()

parser.add_argument("-f", "--folderpath", help="specify folder path for your GET query", nargs="?", default="")
parser.add_argument("-e", "--acceptencodings", help="specify which encodings to accept", nargs="*")
parser.add_argument("-a", "--accept", help="specify which content-types are acceptable", nargs ="*")
parser.add_argument("-ims", "--ifmodsince", help="specify date-time for when the If-Modified-Since header (Y-m-d H:M:S)", nargs="?", default=None)
parser.add_argument("-ius", "--ifunmodsince", help="specify date-time for when the If-Unmodified-Since header (Y-m-d H:M:S)", nargs="?", default=None)
parser.add_argument("-p", "--port", help="specify port number to send on", nargs ="?", default= 8080)
args = parser.parse_args()

date_time_format = "%Y-%m-%d %H:%M:%S"

if args.ifunmodsince:
    unmod_time = time.strptime(args.ifunmodsince, date_time_format)
if args.ifmodsince:
    mod_time = time.strptime(args.ifmodsince, date_time_format)

# First, create a socket to send the request through
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the server
client_socket.connect(("localhost", int(args.port)))

# Create the request line
request_line = f"GET {args.folderpath} HTTP/1.1\r\n"

# Create the headers
headers = "Host: localhost\r\nUser-Agent: Python-Test-Client\r\nConnection: close\r\n"
if args.accept:
    headers += "Accept: "
    for content_types in args.accept:
        headers += f"{content_types}, "
    headers = headers[:-2]
    headers += "\r\n"

if args.ifmodsince:
    headers += "If-Modified-Since: {}\r\n".format(time.strftime("%a, %d %b %Y %H:%M:%S", mod_time))
if args.ifunmodsince:
    headers += "If-Unmodified-Since: {}\r\n".format(time.strftime("%a, %d %b %Y %H:%M:%S", unmod_time))

if args.acceptencodings:
    headers += "Accept-Encoding: "
    for encoding in args.acceptencodings:
        headers += f"{encoding}, "
    headers = headers[:-2]
    headers += "\r\n"

# Create the full request
request = request_line + headers + "\r\n"

# Send the request to the server
client_socket.send(request.encode("utf-8"))

# Receive the response in chunks
response = b""
while True:
    chunk = client_socket.recv(1024)
    if not chunk:
        break
    response += chunk

# Split the response into header and body
header_body = response.split(b"\r\n\r\n", maxsplit=1)
if len(header_body) == 2:
    header, body = header_body
else:
    header = header_body[0]
    body = b""

header = header.decode()

# Make 'dictionary' of headers
headers = {}
for line in header.split("\r\n")[1:]:
    if ": " in line:
        key, value = line.split(": ", 1)
        headers[key] = value

# Print the response
print(header)

if not body:
    client_socket.close()
    exit

print("\r\n")

#Possibly decompress body and then print
if not "Encoding" in headers:
    print(body.decode())
elif headers["Encoding"] == "gzip":
    decomp_body = zlib.decompress(body)
    try:
        print(decomp_body.decode())
    except:
        print(decomp_body)

# Close the socket
client_socket.close()
