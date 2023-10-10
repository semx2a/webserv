<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    
    // Path to user data file
    $userDataFilePath = "/var/www/testwebsite.com/data/users/$username.txt";
    
    // Check if the user already exists
    if (file_exists($userDataFilePath)) {
        echo "Username already exists. Please choose a different username.";
    } else {
        // Hash the password securely before storing it
        $hashedPassword = password_hash($password, PASSWORD_DEFAULT);
    
        // Create user data in JSON format
        $userData = [
            "username" => $username,
            "password" => $hashedPassword,
            "email" => $email,
            "fullname" => $fullname
        ];
    
        // Save user data to the file
        file_put_contents($userDataFilePath, json_encode($userData));
    
        echo "Account created successfully!";
    $username = $_POST["username"];
    $password = $_POST["password"];
    }
}
?>
POST /create_account.php HTTP/1.1
Host: localhost:4141
Connection: keep-alive
Content-Length: 27
Cache-Control: max-age=0
sec-ch-ua: "Not/A)Brand";v="99", "Google Chrome";v="115", "Chromium";v="115"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36
Origin: http://localhost:4141
Content-Type: application/x-www-form-urlencoded
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:4141/create_account.html
Accept-Encoding: gzip, deflate, br
Accept-Language: en-US,en;q=0.9,fr;q=0.8
Cookie: ajs_user_id=f98b71c5-59eb-582b-8ae6-222e139c3997; ajs_anonymous_id=6f8cc7db-db73-4f53-b4d5-12bcc18866f3

username=noe&password=miaousername=noe&password=miaouusername=noe&password=miaouusername=noe&password=miaouusername=noe&password=miaou