var TelegramBot = require('node-telegram-bot-api');
var mysql      = require('mysql');

var connection = mysql.createConnection({
    host     : 'zabbix-db',
    user     : 'zabbix',
    password : 'my_password',
    database : 'zabbix'
});

connection.connect();


var token = '458174897:AAHpZ9CK0nKGpp9EAWThqaYlgfem-4gN_g4';
// Включить опрос сервера
var bot = new TelegramBot(token, {polling: true});

bot.onText(/gate/, (msg, match) => {
    response(msg,"ok");
});

bot.onText(/triggers/, (msg, match) => {
    let q = `SELECT DISTINCT host, t.description, t.state
FROM triggers t INNER JOIN functions f ON ( f.triggerid = t.triggerid )
  INNER JOIN items i ON ( i.itemid = f.itemid )
  INNER JOIN hosts h ON ( i.hostid = h.hostid )
  INNER JOIN events e ON ( e.objectid = t.triggerid )
  INNER JOIN hosts_groups hg ON ( hg.hostid = h.hostid )
  INNER JOIN groups g ON ( g.groupid = hg.groupid )
WHERE (e.eventid DIV 100000000000000) IN (0)
      AND (e.object-0)=0 AND t.value =0
      AND h.status =0 AND i.status =0
      AND t.status =0
  AND g.groupid=8
GROUP BY f.triggerid
ORDER BY t.lastchange DESC`;

    connection.query(q, (error, results, fields) =>{
        if (error) throw error;
        response(msg,JSON.stringify(results));
    }
    );
});

// Простая команда без параметров.
// bot.on('message', function (msg) {
//     response(msg,"Command not supported");
// });

response = (msg, response)=>{
    bot.sendMessage(msg.from.id, response);
}