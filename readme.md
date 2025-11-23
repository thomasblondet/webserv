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
