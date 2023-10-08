<!DOCTYPE html>
<html>
<head>
    <title><?php echo $pageTitle; ?></title>
    <link rel="stylesheet" type="text/css" href="style.css">
</head>
<body>
    <div class="topnav">
        <a href="index.html">Home</a>

        <?php
        // Check if a user is logged in (you should implement your authentication logic here)
        $loggedIn = false; // Set this to true if a user is logged in

        if ($loggedIn) {
            echo '<a href="account_settings.html">Account Settings</a>';
            echo '<a href="logout.php">Logout</a>';
        } else {
            echo '<a href="create_account.html">Create Account</a>';
            echo '<a href="login.html">Login</a>';
        }
        ?>

    </div>
    <h1><?php echo $pageTitle; ?></h1>
