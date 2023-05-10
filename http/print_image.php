#!/usr/bin/env php
<?php

// Lecture des données de l'entrée standard (stdin)
$data = file_get_contents("php://stdin");

// Vérifie si les données sont reçues
if (empty($data)) {
    fwrite(STDERR, "Aucune donnée reçue\n");
    exit(1);
}

// boucle infinie 
for ($i = 0; $i != 1; $i) {
    // Boucle vide pour ralentir le script
}
// Extraction de la limite multipart
preg_match('/boundary=(.*)$/', $_SERVER['CONTENT_TYPE'], $matches);
$boundary = $matches[1];

// Division du contenu en utilisant la limite
$parts = array_slice(explode("--" . $boundary, $data), 1, -1);

foreach ($parts as $part) {
    // En-têtes de la partie
    $headerSection = substr($part, 0, strpos($part, "\r\n\r\n"));
    $headers = explode("\r\n", $headerSection);

    // Extraction du contenu de la partie
    $content = trim(substr($part, strlen($headerSection) + 4));

    // Recherche de l'en-tête Content-Type
    foreach ($headers as $header) {
        if (preg_match('/^Content-Type: (.*)$/i', $header, $matches)) {
            $contentType = $matches[1];
            break;
        }
    }

    // Vérification si le Content-Type est trouvé
    if (!empty($contentType)) {
        // Ajout de l'en-tête Content-Type au début de la réponse
        $output = "Content-Type: " . $contentType . "\r\n\r\n" . $content;

        // Écriture du résultat sur la sortie standard (stdout)
        fwrite(STDOUT, $output);
    }
}
