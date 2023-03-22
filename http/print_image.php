<?php
header('Content-Type: image/jpeg');

// Récupérer l'image à partir de la stdin
$stdin = fopen('php://stdin', 'r');

// Supprimer les délimiteurs de multipart
$boundaryPos = strpos(fgets($stdin), "\r\n");

if ($boundaryPos !== false) {
    $boundary = substr(fgets($stdin), 0, $boundaryPos);

    // Obtenir le contenu du fichier
    $fileContent = '';
    while (($line = fgets($stdin)) !== false && strpos($line, $boundary) === false) {
        $fileContent .= $line;
    }

    // Afficher l'image
    echo $fileContent;
} else {
    echo "Erreur : impossible de récupérer l'image.";
}
?>
