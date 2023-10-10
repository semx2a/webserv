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