<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    // Process the data (e.g., validation and account creation).
    
    // For this example, let's just display the received data.
    echo "Account Created:<br>";
    echo "Username: " . htmlspecialchars($username) . "<br>";
}
?>
