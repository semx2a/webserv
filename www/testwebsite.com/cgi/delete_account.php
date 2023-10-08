<?php
if ($_SERVER["REQUEST_METHOD"] === "DELETE") {
    $username = $_REQUEST["username"];
    $password = $_REQUEST["password"];
    $consent = $_REQUEST["consent"];

    // Check if the consent checkbox is selected
    if ($consent !== "on") {
        echo "Please consent to delete your account by checking the box.";
    } else {
        // Path to user data file
        $userDataFilePath = "/var/www/testwebsite.com/data/users/$username.txt";
        
        // Check if the user data file exists
        if (file_exists($userDataFilePath)) {
            // Delete the user data file to remove the account
            unlink($userDataFilePath);
        
            echo "User with username '$username' has been deleted.";
        } else {
            // User does not exist
            echo "User not found. Deletion failed.";
        }
        // Perform user deletion logic here (e.g., remove user data from files or database).
        // For demonstration purposes, we'll just display a message.
    }
}
?>
