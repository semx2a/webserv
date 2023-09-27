<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $uploadDir = '/path/to/upload/folder/';
    $uploadFile = $uploadDir . basename($_FILES['userfile']['name']);
    
    if (move_uploaded_file($_FILES['userfile']['tmp_name'], $uploadFile)) {
        echo "Le fichier a été téléchargé avec succès.\n";
    } else {
        echo "Échec du téléchargement du fichier.\n";
    }
} else {
    // Affichez le formulaire de téléchargement de fichier
    echo '<form enctype="multipart/form-data" action="upload.php" method="POST">
    <input type="hidden" name="MAX_FILE_SIZE" value="30000" />
    Envoyez ce fichier : <input name="userfile" type="file" />
    <input type="submit" value="Envoyer le fichier" />
    </form>';
}
?>
