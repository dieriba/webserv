#!/usr/bin/env python

import os
import cgi

# Retrieve the value from the BODY environment variable
body_env = os.environ.get('BODY', '')

# Split the value by '=' to extract the actual value
body_key = body_env.split('=')[0]
body_value = body_env.split('=')[1] if '=' in body_env else ''

# Set the response cookie header based on the extracted value
response_cookie = "={}".format(body_value) if body_value else ""

# Print the HTTP headers
print("Content-Type: text/html")
print("Set-Cookie:", body_key , "{}\r\n\r\n".format(response_cookie))

# Print the HTML content
print("<html>")
print("<head><title>Cookie Retrieval and Response</title></head>")
print("<body>")
print("<h1>Cookie Retrieval and Response</h1>")
print("<p>Value extracted from the 'BODY' environment variable: {}</p>".format(body_value))
print("</body>")
print("</html>")