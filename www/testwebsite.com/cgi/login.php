<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    // Path to user data file
    $userDataFilePath = "/var/www/testwebsite.com/data/users/$username.txt";
    
    // Check if the user data file exists
    if (file_exists($userDataFilePath)) {
        // Read user data from the file
        $userData = json_decode(file_get_contents($userDataFilePath), true);
    
        // Check if the provided password matches the stored hashed password
        if (password_verify($password, $userData["password"])) {
            // Login successful
            echo "Login successful!";
        } else {
            // Incorrect password
            echo "Incorrect password. Please try again.";
        }
    } else {
        // User does not exist
        echo "User not found. Please check your username.";
    }
 }
?>
