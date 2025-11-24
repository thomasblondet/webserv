# Nginx Configuration Guide with Examples

This document expands your template with **complete, realistic Nginx examples**, so you can understand how to configure:
- Listening interfaces and ports
- Error pages
- Maximum client body size
- Method restrictions
- Redirections
- Root directories per location
- Directory listing
- Default index files
- File uploads

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

---

## Combined Example Server
Here is a full, realistic config showing all concepts.

```nginx
server {
    listen 80;
    server_name example.com;

    root /var/www/public;
    index index.html;
    error_page 404 /errors/404.html;

    client_max_body_size 5m;

    location / {
        try_files $uri $uri/ =404;
    }

    location /kapouet/ {
        root /tmp/www;
        client_max_body_size 1m;
    }

    location /old/ {
        return 301 /new/;
    }

    location /directory/ {
        autoindex on;
    }

    location /upload/ {
        limit_except POST { deny all; }
        fastcgi_pass unix:/run/php/php8.2-fpm.sock;
        include fastcgi_params;
        fastcgi_param SCRIPT_FILENAME /var/www/upload.php;
    }
}
```

