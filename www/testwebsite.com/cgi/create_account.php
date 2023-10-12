<?php

// Get user input (username, password, email, fullname)
$username = $_POST["username"];
$password = $_POST["password"];
$email = $_POST["email"];
$fullname = $_POST["fullname"];

// Path to user data file
$userDataFilePath = "/mnt/nfs/homes/nfauconn/repos/webserv/www/testwebsite.com/data/users/$username.txt";

if (!file_exists($userDataFilePath)) {
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

    // Display success message
    echo "Account successfully created";
} else {
    // Account already exists
    echo "Account with username '$username' already exists. Please choose a different username.";
}
?>
