<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    // Get user input (username, current_password, new_password)
    $username = $_POST["username"];
    $currentPassword = $_POST["current_password"];
    $newPassword = $_POST["new_password"];
    
    // Path to user data file
    $userDataFilePath = "/sgoinfre/goinfre/Perso/seozcan/CCORE/github/webserv_github/www/testwebsite.com/data/users/$username.txt";
    
    // Check if the user data file exists
    if (file_exists($userDataFilePath)) {
        // Read user data from the file
        $userData = json_decode(file_get_contents($userDataFilePath), true);
    
        // Check if the provided current password matches the stored hashed password
        if (password_verify($currentPassword, $userData["password"])) {
            // Hash the new password securely before updating it
            $hashedNewPassword = password_hash($newPassword, PASSWORD_DEFAULT);
    
            // Update the password in the user data
            $userData["password"] = $hashedNewPassword;
    
            // Save the updated user data to the file
            file_put_contents($userDataFilePath, json_encode($userData));
    
            echo "Password changed successfully!";
        } else {
            // Incorrect current password
            echo "Incorrect current password. Password not changed.";
        }
    } else {
        // User does not exist
        echo "User not found. Please check your username.";
    }
}
?>
