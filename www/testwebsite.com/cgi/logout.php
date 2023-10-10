<?php
// Start or resume the session
session_start();

// Check if the user is already logged out
if (!isset($_SESSION['user'])) {
    header("Location: index.html"); // Redirect to the home page or another appropriate page
    exit();
}

// Unset all session variables
session_unset();

// Destroy the session
session_destroy();

// Redirect the user to the home page or another appropriate page after logout
header("Location: index.html");
exit();
?>
