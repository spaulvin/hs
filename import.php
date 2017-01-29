<?php
require "vendor/autoload.php";

use M3uParser\M3uParser;

$m3uParser = new M3uParser();
$data = $m3uParser->parseFile('/home/paul/Downloads/m.m3u');

$pdo = new PDO("mysql:host=127.0.0.1;dbname=mediatomb;charset=utf8","mediatomb","pwgen");
$insert = $pdo->prepare("INSERT INTO mt_cds_object (parent_id,object_type,upnp_class,dc_title,location,resources,update_id,mime_type,flags) VALUES (:parent_id,:object_type,:upnp_class,:dc_title,:location,:resources,:update_id,:mime_type,:flags)");

$delete = $pdo->prepare("DELETE FROM mt_cds_object WHERE parent_id=2");

$parent_id =2;
$object_type = 10;
$resources = "0~protocolInfo=http-get%3A%2A%3Avideo%2Fx-avi%3A%2A~~";
$update_id = 0;
$mime_type = "video/ffmpeg";
$flags = 1;
$upnp_class = "object.item.videoItem";

$delete->execute();

foreach ($data as $entry) {
    echo $entry->getPath() . "\n";
    if ($entry->getExtInf()) {
        $dc_title = $entry->getExtInf()->getTitle();
        $location = $entry->getPath();

        echo $dc_title . "\n";

        $insert->bindParam("parent_id",$parent_id);
        $insert->bindParam("object_type",$object_type);
        $insert->bindParam("upnp_class",$upnp_class);
        $insert->bindParam("dc_title",$dc_title);
        $insert->bindParam("location",$location);
        $insert->bindParam("resources",$resources);
        $insert->bindParam("update_id",$update_id);
        $insert->bindParam("mime_type",$mime_type);
        $insert->bindParam("flags",$flags);
        $insert->execute();
    }
}