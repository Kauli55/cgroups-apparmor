<?php

#Montée en puissance niveau calcul --> Charge CPU
$debut=microtime(true);
echo"Début du test pour la montée en charge du CPU.\n";

$a=1;
for($i=0;$i<100000;$i++)
{
    $a+=$i
}

$fin=microtime(true);
$temps_execution = $fin - $debut;
echo"Fin du test pour la montée en charge du CPU.\n";
echo"Durée du test : $temps_execution secondes \n";

?>