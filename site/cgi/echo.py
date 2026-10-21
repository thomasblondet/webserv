#!/usr/bin/python3
import json
import os
import sys

length = int(os.environ.get("CONTENT_LENGTH", "0"))
body = sys.stdin.read(length) if length else ""

print("Content-Type: application/json; charset=utf-8")
print()
print(json.dumps({
    "method": os.environ.get("REQUEST_METHOD", ""),
    "query": os.environ.get("QUERY_STRING", ""),
    "body": body,
}, ensure_ascii=False, indent=2))
