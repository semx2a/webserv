<?php
if ($_SERVER["REQUEST_METHOD"] === "DELETE") {
    $username = $_REQUEST["username"];
    $password = $_REQUEST["password"];
    $consent = $_REQUEST["consent"];

    // Check if the consent checkbox is selected
    if ($consent !== "on") {
        echo "Please consent to delete your account by checking the box.";
    } else {
        // Perform user deletion logic here (e.g., remove user data from files or database).
        // For demonstration purposes, we'll just display a message.
        echo "User with username '$username' has been deleted.";
    }
}
?>
