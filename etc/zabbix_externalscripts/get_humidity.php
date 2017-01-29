#! /usr/bin/php
<?php

$ch = curl_init();
curl_setopt($ch, CURLOPT_URL, "http://192.168.88.237");
curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
$data = curl_exec($ch);
curl_close($ch);

print_r(json_decode($data, true)["humidity"]);