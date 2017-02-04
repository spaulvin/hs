<?php

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/weather?q=Vinnytsya&units=metric&appid=2c78241c36e203da1ce4c01dd8dcfa0b");
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

while (true) {
    $data = curl_exec($ch);


    $data = json_decode($data, true);


    $fp = fsockopen("192.168.88.233", 23, $errno, $errstr, 30);
//    $date = date("H:i:s", $data['dt']);
    $date = date("H:i:s");

    fwrite($fp, "html16 {$date}<br>t: {$data['main']['temp']} <br> h: {$data['main']['humidity']} w: {$data['wind']['speed']}");
    echo fgets($fp, 128);
    fclose($fp);

    sleep(10);
}

?>