<?php

$from_dh22 = false ;

if ($from_dh22) {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, "http://192.168.88.237/");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $data = curl_exec($ch);

    $data = json_decode($data, true);
    ?>

    Temp: <?= $data['temp'] ?>

    Humidity: <?= $data['humidity'] ?> %


    <?
} else {
    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/weather?q=Vinnytsya&units=metric&appid=2c78241c36e203da1ce4c01dd8dcfa0b");
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $data = curl_exec($ch);

    $data = json_decode($data, true);
    ?>

    Temp: <?= $data['main']['temp'] ?>

    Humidity: <?= $data['main']['humidity'] ?> %

    Wind: <?= $data['wind']['speed'] ?> kmPh
    <?

}
?>