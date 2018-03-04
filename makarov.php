<?php
require __DIR__ . '/vendor/autoload.php';

use \Curl\Curl;

$curl = new Curl();

$cookie = "/tmp/makarov.txt";
$curl->setCookieFile($cookie);
$curl->setCookieJar($cookie);

$curl->get('http://doctor-makarov.com.ua');
preg_match("/input type=\"hidden\" name=\"return\" value=\"(.*)\"/",$curl->response, $matches);
preg_match("/input type=\"hidden\" name=\"(.*)\" value=\"1\"/",$curl->response,$matches2);

$curl->post('http://doctor-makarov.com.ua/profil?task=user.login', [
    'username' => 'viktoriya',
    'password' => '2514422Vfvf',
    'return' => $matches[1],
    $matches2[1]=>1
], true);

$curl->get("http://doctor-makarov.com.ua/zapis-na-priem", [
    'idspec'=>1,
    'cdate'=>1516575660
]);
var_dump($curl->response);

//искать div class = recept
//<div class="norecept yesrecept">10:00<div class="dop_text">Занято</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6959">10:30</a><div class="dop_text">Свободно</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6962">11:00</a><div class="dop_text">Свободно</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6963">11:30</a><div class="dop_text">Свободно</div></div><div class="norecept yesrecept">12:00<div class="dop_text">Занято</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6966">12:30</a><div class="dop_text">Свободно</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6960">13:00</a><div class="dop_text">Свободно</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6961">13:30</a><div class="dop_text">Свободно</div></div><div class="recept" title=""><a href="/zapis-na-priem?task=addclient&amp;cdate=1516575660&amp;id=6964">15:00</a><div class="dop_text">Свободно</div></div>
//Порядковый номер td внутри tf - это будет день недели 1-ПН 7-ВС

//http://doctor-makarov.com.ua/zapis-na-priem?task=addclient&cdate=1516575660&id=6959

//http://doctor-makarov.com.ua/zapis-na-priem?task=addclient&amp;addtest=1&amp;cdate=1516575660&amp;id=6959

//fio:Шепотько Александр Павлович
//phone:0972834766
//ttfsp_email:vikusyyaa@gmail.com
//ttfsp_3:10 мес
//ttfsp_5:проф осмотр
//add:1
