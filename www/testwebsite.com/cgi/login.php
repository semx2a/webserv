<?php
if ($_SERVER["REQUEST_METHOD"] === "POST") {
    $username = $_POST["username"];
    $password = $_POST["password"];

    // Process the data (e.g., validation and authentication).

    // For this example, let's just display the received data.
    echo "Login Successful:<br>";
    echo "Username: " . htmlspecialchars($username) . "<br>";
}
?>
