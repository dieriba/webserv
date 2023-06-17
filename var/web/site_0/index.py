#!/usr/bin/env python3
import os

form = None

# Check the request method
request_method = os.environ.get("REQUEST_METHOD", "").upper()

# Handle GET request
if request_method == "GET":
    # Parse the query string
    query_string = os.environ.get("QUERY_STRING", "")
    form = {}
    for param in query_string.split("&"):
        key, value = param.split("=")
        form[key] = value

# Handle POST request
elif request_method == "POST":
    content_length = int(os.environ.get("CONTENT_LENGTH", 0))
    post_data = os.environ.get("wsgi.input").read(content_length).decode()
    form = {}
    for param in post_data.split("&"):
        key, value = param.split("=")
        form[key] = value

# Output the key-value pairs in an HTML list
if form:
    print("<html>")
    print("<body>")
    print("<ul>")
    for key, value in form.items():
        print(f"<li><strong>{key}:</strong> {value}</li>")
    print("</ul>")
    print("</body>")
    print("</html>")