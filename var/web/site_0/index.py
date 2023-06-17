import cgi
import os

print("Content-Type: text/html")
print()

form = None

# Check the request method
request_method = os.environ.get("REQUEST_METHOD", "").upper()

# Handle GET request
if request_method == "GET":
    # Parse the query string
    query_string = os.environ.get("QUERY_STRING", "")
    form = cgi.parse_qs(query_string, keep_blank_values=True)

# Handle POST request
elif request_method == "POST":
    form = cgi.FieldStorage()

# Output the key-value pairs in an HTML list
if form:
    print("<html>")
    print("<body>")
    print("<ul>")
    for key in form.keys():
        value = form.getvalue(key)
        print(f"<li><strong>{key}:</strong> {value}</li>")
    print("</ul>")
    print("</body>")
    print("</html>")