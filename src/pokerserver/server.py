#!/usr/bin/env python

import BaseHTTPServer
import CGIHTTPServer
import cgitb
import sys, socket
DEFAULT_PORT = 9313

def print_usage():
    print "Usage: ./server.py [port]"

def start_server():
    hostname = socket.gethostname()

    # Figure out which port to run the server on
    port = DEFAULT_PORT
    if len(sys.argv) == 2:
        try:
            port = int(sys.argv[1])
        except:
            print_usage()
            exit(1)

    server = BaseHTTPServer.HTTPServer
    handler = CGIHTTPServer.CGIHTTPRequestHandler

    # Enable cgi logging for debugging
    cgitb.enable()
    server_address = ("", port)
    handler.cgi_directories= ["/cgi"]

    print("Server starting on %s:%d" % (hostname, port))
    httpd = server(server_address, handler)
    httpd.serve_forever()

if __name__ == "__main__":
    start_server()
