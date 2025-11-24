# HTTP
🤯

# Nginx
Files structure:
```bash
> nginx-1.28.0 # src dir
> nginx-test # build dir
install_and_configure_nginx.sh # install script
nginx-1.28.0.tar.gz # archive
```

---

To install and configure nginx:
```bash
chmod +x install_and_configure_nginx.sh
./install_and_configure_nginx.sh
```

---

Here are the most useful commands: (maybe use sudo 😏):

* https://nginx.org/en/docs/beginners_guide.html

```bash
# 1. Test that the config file is valid
./nginx-test/sbin/nginx -t -c conf/nginx.conf

# 2. Start Nginx using your custom build
./nginx-test/sbin/nginx -c conf/nginx.conf

# 3. Reload configuration after editing
./nginx-test/sbin/nginx -s reload

# 4. Stop Nginx
./nginx-test/sbin/nginx -s stop

# 5. Remove nginx
rm -rf nginx-1.28.0 nginx-1.28.0.tar.gz nginx-test/
```

---

To test the default conf, we can go to


```
http://localhost:80
```

or

```bash
curl -v -i --http1.0 --get http://localhost:80
```

---

# Configuration file

You can take inspiration from the ’server’ section of the [NGINX configuration file](docs/nginx_config_guide.md).

* Define all the interface:port pairs on which your server will listen to (defining multiple websites served by your program).
* Set up default error pages.
* Set the maximum allowed size for client request bodies
https://nginx.org/en/docs/http/ngx_http_core_module.html#client_max_body_size
```
Syntax:	client_max_body_size size;
Default:	
client_max_body_size 1m;
Context:	http, server, location
```
```nginx
server {
    client_max_body_size 1m;

    location /kapouet/ {
        client_max_body_size 1m;
    }
}
```

Specify rules or configurations on a URL/route:
* List of accepted HTTP methods for the route.
```nginx
server {
    location /directory/ {
        allow_methods ".*";
    }
}
```

* HTTP redirection.
* Directory where the requested file should be located:
 (e.g., if URL `/kapouet` is rooted to `/tmp/www`, URL `/kapouet/pouic/toto/pouet` will search for `/tmp/www/pouic/toto/pouet`).
```nginx
server {
    location / {
        root /data/www;
    }

    location /kapouet/ {
        root /tmp/www;
    }
}
```

* Enabling or disabling directory listing.
https://nginx.org/en/docs/http/ngx_http_autoindex_module.html
```
Syntax:	autoindex on | off;
Default:	
autoindex off;
Context:	http, server, location
```
```nginx
server {
    location /directory/ {
        autoindex on;
        autoindex off;
    }
}
```

* Default file to serve when the requested resource is a directory.
* Uploading files from the clients to the server is authorized, and storage location
is provided.

---
