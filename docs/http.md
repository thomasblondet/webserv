# 1. Request

Request =
* Simple-Request
* Full-Request

## 1.1 Simple-Request

Simple-Request = "GET" SP Request-URI CRLF

```http
GET /pub/WWW/TheProject.html
```

HTTP/0.9 was the original release and it was extremely simple, supporting **only the HTTP GET method**. Only **HTML** files were included in HTTP responses, there were **no HTTP headers**, and there were **no HTTP status codes**.

## 1.2 Full-Request


![alt text](https://mdn.github.io/shared-assets/images/diagrams/http/messages/request-headers.svg){ width=50% }

Full-Request =
* Request-Line
* *( General-Header | Request-Header | Entity-Header )
* CRLF
* [ Entity-Body ]

```http
POST /users HTTP/1.0
Host: example.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 49

name=FirstName+LastName&email=bsmth%40example.com
```

### 1.2.1 Request-Line

Request-Line = **Method** SP **Request-URI** SP **HTTP-Version** CRLF

Method:
* "GET"
* "POST"
* "DELETE"
* "HEAD"

Servers should return the status code 501 (not implemented) if the method is 
unrecognized or not implemented.

Request-URI:
* absoluteURI (I don't think we need this)
* abs_path

```http
POST /users HTTP/1.0
```

```http
GET /pub/WWW/TheProject.html HTTP/1.0
```

### 1.2.2 Header

```http
Host: example.com
Content-Type: application/x-www-form-urlencoded
Content-Length: 49
```

### 1.2.3 Entity-Body

```http
name=FirstName+LastName&email=bsmth%40example.com
```

* if an HTTP/1.0 server receives a Simple-Request, it must respond with an
HTTP/0.9 Simple-Response.

* An HTTP/1.0 client capable of receiving a Full-Response should never generate
a Simple-Request.

---

# 2. Response

## 2.1 Simple-Response
Simple-Response = [ Entity-Body ]

**Simple request:**
```http
GET /index.html
```
**Simple response:**
```html
<html>
Welcome to the example.re homepage!
</html>
```
## 2.2 Full-Response

Full-Response =
* Status-Line
* *( General-Header | Response-Header | Entity-Header )
* CRLF
* [ Entity-Body ]

![alt text](https://mdn.github.io/shared-assets/images/diagrams/http/messages/response-headers.svg){ width=50% }

**Status-Line:**
```
<protocol> <status-code> <reason-phrase>
```

---

# 3 Method Definitions
## 3.1 GET
The GET method means retrieve whatever information (in the form of an
entity) is identified by the Request-URI.
* If the Request-URI refers to a data-producing process, it is the produced data
 which shall be returned as the entity in the response.
* A **conditional GET** method requests that the identified resource be
transferred only if it has been modified since the date given by the
**If-Modified-Since header**.

## 3.2 POST
The actual function performed by the POST method is determined by the
server and is usually dependent on the Request-URI.

* If a resource has been created on the origin server, the response
should be 201 (created) and contain an entity (preferably of type
"text/html") which describes the status of the request and refers to
the new resource.

* A valid Content-Length is required on all HTTP/1.0 POST requests. An
HTTP/1.0 server should respond with a 400 (bad request) message if it
cannot determine the length of the request message’s content.

## 3.3 HEAD
The HEAD method is **identical to GET** except that the server must 
**not return any Entity-Body in the response**. The metainformation contained in
the HTTP headers in response to a HEAD request should be identical to the 
information sent in response to a GET request.

# 4 Additional Request Methods
## 4.1 DELETE
The DELETE method requests that the origin server **delete the resource**
identified by the Request-URI.

---

# 5 Status Code Definitions

---

# 6 Header Fields

## 6.1 Header Field Definitions

This section defines the syntax and semantics of all commonly used
HTTP/1.0 header fields. For general and entity header fields, both
sender and recipient refer to either the client or the server.

1. Allow (entity-header field)
2. Authorization (request-header field) (I don't think we need this)
3. Content-Encoding (entity-header field)
4. Content-Length (entity-header field)
5. Content-Type (entity-header field)
6. Date (general-header field)
7. Expires (entity-header field)
8. From (request-header field)
9. If-Modified-Since (request-header field)
10. Last-Modified (entity-header field)
11. Location (response-header field)
12. Pragma (general-header field)
13. Referer (request-header field)
14. Server (response-header field)
15. User-Agent (request-header field)
16. WWW-Authenticate (response-header field) (I don't think we need this)

## 6.2 Additional Header Field Definitions

1. Accept (request-header field)
2. Accept-Charset (request-header field)
3. Accept-Encoding (request-header field)
4. Accept-Language (request-header field)
5. Content-Language (entity-header field)
6. Link (entity-header field)
7. MIME-Version (general-header field) (should be ignored??)
8. Retry-After (response-header field)
9. Title (entity-header field)
10. URI (entity-header field)

## 6.3 Summary

### 6.3.1 General-header field
<table>
  <tr>
    <th>Field</th>
    <th>Explanation</th>
  </tr>
  <tr>
    <td>Date</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Pragma</td>
    <td>?</td>
  </tr>
  <tr>
    <td>MIME-Version</td>
    <td>?</td>
  </tr>
</table>

### 6.3.2 Request-header field
<table>
  <tr>
    <th>Field</th>
    <th>Explanation</th>
  </tr>
  <tr>
    <td>Authorization</td>
    <td>?</td>
  </tr>
  <tr>
    <td>From</td>
    <td>?</td>
  </tr>
  <tr>
    <td>If-Modified-Since</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Referer</td>
    <td>?</td>
  </tr>
  <tr>
    <td>User-Agent</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Accept</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Accept-Charset</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Accept-Encoding</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Accept-Language</td>
    <td>?</td>
  </tr>
</table>

### 6.3.3 Response-header field
<table>
  <tr>
    <th>Field</th>
    <th>Explanation</th>
  </tr>
  <tr>
    <td>Location</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Server</td>
    <td>?</td>
  </tr>
  <tr>
    <td>WWW-Authenticate</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Retry-After</td>
    <td>?</td>
  </tr>
</table>

### 6.3.4 Entity-header field
<table>
  <tr>
    <th>Field</th>
    <th>Explanation</th>
  </tr>
  <tr>
    <td>Allow</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Content-Encoding</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Content-Length</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Content-Type</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Expires</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Last-Modified</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Content-Language</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Link</td>
    <td>?</td>
  </tr>
  <tr>
    <td>Title</td>
    <td>?</td>
  </tr>
  <tr>
    <td>URI</td>
    <td>?</td>
  </tr>
</table>
