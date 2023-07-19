#!/usr/bin/env python
import cgi
import os

# Function to set a cookie
def set_cookie(name, value):
    print(f"Set-Cookie: {name}={value};")
    print("Content-type: text/html\n")

# Retrieve the value of the 'login' cookie
login_cookie = os.environ.get('COOKIE')

# Check if the 'login' cookie exists
if login_cookie:
    # Cookie is present, so extract the login name and display it
    login_name = login_cookie.split('=')[1]
    print("Content-type: text/html\r\n\r\n")
    print(f"Hello, {login_name}!")

else:
    # Cookie is not present, so display the registration page
    print("Content-type: text/html\r\n\r\n")
    print("<html>")
    print("<head><title>Registration</title></head>")
    print("<body>")
    print("<h1>Registration Page</h1>")
    print("<form method='POST' action=\"http://localhost:8080/cgi-bin/set-cookie.py\">")
    print("Login name: <input type='text' name='login_name'><br>")
    print("<input type='submit' value='Register'>")
    print("</form>")
    print("</body>")
    print("</html>")

    # Process the form submission
    form = cgi.FieldStorage()
    login_name = form.getvalue('login_name')

    if login_name:
        # Set the 'login' cookie with the provided login name
        set_cookie("login", login_name)