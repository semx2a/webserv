# REQUESTS

- Cookies Support > [mdn_web_docs: cookies](https://developer.mozilla.org/en-US/docs/Web/HTTP/Cookies)

```http
Set-Cookie: <cookie-name>=<cookie-value>
```

```http
HTTP/2.0 200 OK
Content-Type: text/html
Set-Cookie: yummy_cookie=choco
Set-Cookie: tasty_cookie=strawberry

[page content]
```

```http
GET /sample_page.html HTTP/2.0
Host: www.example.org
Cookie: yummy_cookie=choco; tasty_cookie=strawberry
```