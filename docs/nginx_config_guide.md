# Nginx Configuration Guide with Examples

This document expands your template with **complete, realistic Nginx examples**, so you can understand how to configure:
1. Listening interfaces and ports
2. Error pages
3. Maximum client body size
4. Method restrictions
5. Redirections
6. Root directories per location
7. Directory listing
8. Default index files
9. File uploads
10. Execution of CGI based on file extension

---
## What is virtual hosts?
The concept of virtual hosts **allows more than one Web site on one system or Web server**. The servers are **differentiated by their host name**. Visitors to the Web site are **routed by host name or IP address** to the correct virtual host. Virtual hosting allows companies sharing one server to each have their own domain names. For example www.company1.com and www.company2.com can both be hosted on the same server.

* [How nginx processes a request?](https://nginx.org/en/docs/http/request_processing.html)

1. Name-based virtual servers
```nginx
server {
    listen      80;
    server_name example.org www.example.org;
    ...
}

server {
    listen      80;
    server_name example.net www.example.net;
    ...
}

server {
    listen      80;
    server_name example.com www.example.com;
    ...
}
```

2. Mixed name-based and IP-based virtual servers
```nginx
server {
    listen      192.168.1.1:80;
    server_name example.org www.example.org;
    ...
}

server {
    listen      192.168.1.1:80;
    server_name example.net www.example.net;
    ...
}

server {
    listen      192.168.1.2:80;
    server_name example.com www.example.com;
    ...
}
```

---

## 1. Server Listening Interfaces and Ports
Define all the IP:port pairs where your server will listen. One Nginx instance may serve several websites.

```nginx
http {
    # Website 1: listens on all interfaces, port 80
    server {
        listen 80;
        server_name example.com;
    }

    # Website 2: listens on localhost only
    server {
        listen 127.0.0.1:8080;
        server_name local.example;
    }

    # Website 3: HTTPS
    server {
        listen 443 ssl;
        server_name secure.example.com;
        ssl_certificate     /etc/ssl/cert.pem;
        ssl_certificate_key /etc/ssl/key.pem;
    }
}
```

---

## 2. Default Error Pages
Specify custom error pages for common HTTP errors.

```nginx
server {
    error_page 404 /errors/404.html;
    error_page 500 502 503 504 /errors/50x.html;

    location /errors/ {
        root /var/www/default;
    }
}
```

---

## 3. Maximum Allowed Client Body Size

https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size

Controls maximum upload size or POST body size.

```nginx
server {
    client_max_body_size 5m;    # server-wide

    location /kapouet/ {
        client_max_body_size 1m; # override for this route
    }
}
```

---

## 4. Accepted HTTP Methods for a Route
Nginx does not have `allow_methods` directive; instead you use:
- `limit_except` (standard technique), or
- returning 405 manually.

### Example: Allow only GET and HEAD
```nginx
server {
    location /directory/ {
        limit_except GET HEAD {
            deny all;
        }
    }
}
```

### Example: Allow every method (default behavior)
```nginx
location /directory/ {
    # no restrictions → all methods allowed
}
```

### Example: Reject everything except POST
```nginx
location /submit/ {
    limit_except POST { deny all; }
}
```

---

## 5. HTTP Redirection
Redirect one route to another.

### Permanent redirect (301)
```nginx
location /old-path/ {
    return 301 /new-path/;
}
```

### Temporary redirect (302)
```nginx
location /temp/ {
    return 302 https://example.com/maintenance;
}
```

---

## 6. Directory Where Files Are Served (root)
The `root` directive defines the physical filesystem directory.

### Example
```nginx
server {
    # Default root
    root /data/www;

    location /kapouet/ {
        root /tmp/www;    # /kapouet/pouic → /tmp/www/pouic
    }
}
```

### How `root` works:
URL `/kapouet/pouic/toto` → filesystem: `/tmp/www/pouic/toto`

---

## 7. Directory Listing (autoindex)
https://nginx.org/en/docs/http/ngx_http_autoindex_module.html

Show files in a folder (disabled by default).

```nginx
server {
    location /directory/ {
        autoindex on;   # enable listing
    }

    location /private/ {
        autoindex off;  # disable
    }
}
```

---

## 8. Default File for Directory Requests (index)
Defines which file is served when requesting a folder.

```nginx
server {
    index index.html index.htm default.html;

    location /blog/ {
        index home.html index.html;
    }
}
```

Requesting `/blog/` will first try `home.html`.

---

## 9. File Upload Handling
Nginx by itself **does not store uploaded files**; it passes the request body to:
- upstream servers (FastCGI, proxy), or
- modules like `dav_methods` in WebDAV.

### Example: Accepting uploads via FastCGI (PHP, Python, etc.)
```nginx
server {
    client_max_body_size 20m;

    location /upload {
        fastcgi_pass unix:/run/php/php8.2-fpm.sock;
        include fastcgi_params;
        fastcgi_param SCRIPT_FILENAME /var/www/upload.php;
    }
}
```

### Example: Storing files using WebDAV
```nginx
server {
    client_max_body_size 50m;

    location /files/ {
        root /var/www/storage;
        dav_methods PUT DELETE MKCOL COPY MOVE;
        create_full_put_path on;
    }
}
```

### Example: Storing files using nginx-upload-module
* [nginx-upload-module](https://github.com/fdintino/nginx-upload-module)
```nginx
server {
}
```

---

## 10. Execution of CGI

* [Common Gateway Interface: wikipedia](https://en.wikipedia.org/wiki/Common_Gateway_Interface)
* [The Common Gateway Interface (CGI) Version 1.1](https://datatracker.ietf.org/doc/html/rfc3875)

A typical use case: 
1. A web user submits a web form on a web page that uses CGI. The form's data is sent to the web server within a HTTP request with a URL denoting a CGI script. 
2. The web server then launches the CGI script in a new computer process, passing the form data to it.
3. The CGI script passes its output, usually in the form of HTML, to the Web server.
4. The server relays it back to the browser as its response to the browser's request.

```nginx
server {
}
```

---

## Combined Example Server
Here is a full, realistic config showing all concepts.

```nginx
```

