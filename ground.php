<?php
     $client_data = file_get_contents("php://input");
     $r = explode("&",$client_data);
     $handle = fopen("data/ds18b20.txt", w);
     for ($i = 0; $i < count($r); $i++)
     {
     echo $r[$i] . "\n";
     fwrite($handle, $r[$i]);
     fwrite($handle, ",");
     };
     header("Content-type: text/json", TRUE, 200);
     fclose($handle);
     echo "Success\n";

?>
