<?php
// Récupérer l'image à partir de la stdin
$stdin = fopen('php://stdin', 'r');
$stderr = fopen('php://stderr', 'w');

// Supprimer les délimiteurs de multipart
$boundaryPos = strpos(fgets($stdin), "\r\n");

if ($boundaryPos !== false) {
    $boundary = substr(fgets($stdin), 0, $boundaryPos);

    // Obtenir le contenu du fichier et les en-têtes
    $fileContent = '';
    $mimeType = '';
    $isInHeaders = true;
    while (($line = fgets($stdin)) !== false && strpos($line, $boundary) === false) {
        // Ignorer les en-têtes si nous avons déjà terminé
        if (!$isInHeaders) {
            $fileContent .= $line;
            continue;
        }

        // Extraire le type MIME de l'en-tête Content-Type
        if (preg_match('/^Content-Type:\s*(.+)$/i', $line, $matches)) {
            $mimeType = trim($matches[1]);
        }
        
        // Vérifier si nous avons atteint la fin des en-têtes
        if (trim($line) === "\r\n") {
            $isInHeaders = false;
        }
    }

    // Utiliser le type MIME extrait pour définir l'en-tête Content-Type
    if (!empty($mimeType)) {
        echo "Content-Type: $mimeType\r\n\r\n";
    } else {
        echo "Content-Type: image/jpeg\r\n\r\n"; // Utiliser une valeur par défaut si nous n'avons pas réussi à extraire le type MIME
    }

    // Afficher l'image ou la vidéo
    $output = $fileContent;
    fprintf($stderr, "Aperçu du contenu (premiers 100 caractères) :\n%s\n", substr($output, 0, 100));
    echo $output;
} else {
    echo "Erreur : impossible de récupérer l'image.";
}
?>