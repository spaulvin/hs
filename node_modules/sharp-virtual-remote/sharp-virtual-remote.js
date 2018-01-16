/**
 * Sharp Virtual Remote
 * A Node.JS module that controls a Sharp TV with virtual remote option.
 *
 * @module sharp-virtual-remote
 */

var net = require('net');
var dgram = require('dgram');
var EventEmitter = require('events').EventEmitter;
var commands = require('./commands.json');

/**
 * @constructor
 * @param {string} ip - Optional: The IP of the TV
 * @param {number} port - Optional: The port
 */
var SharpVirtualRemote = function(ip, port) {
  EventEmitter.call(this);

  // Set IP if given
  if(ip) {
    this.ip = ip;
    
    // Set port if given
    this.port = port || 4660;

    this.emit('available', ip, port);
  } else {
    this.search();
  }
}
  SharpVirtualRemote.prototype = Object.create(EventEmitter.prototype);

  /**
   * Search for the IP address of the TV on the network
   * Broadcasts a query that is recognized by the TV, the TV responds.
   */
  SharpVirtualRemote.prototype.search = function() {
    var _self = this;

    var query = 'vr_query_tv_version_782';
    var sybasesrvmonPort = 4950;

    dgram.createSocket('udp4')

    // Start listening and configure
    .bind(function() {
      //console.log(':' + this.address().port, 'listening')
      this.setBroadcast(true);
    })

    // Receive response
    .on('message', function(response, info) {
      //console.log(':' + this.address().port, 'received: ' + response + ' from ' + info.address);
      
      // Response usually looks like: vr_tv_query_rsp 4660 mb90 XX:XX:XX:XX:XX:XX 1986 0 1
      // Extracting the port from it
      var port = response.toString().split(' ')[1];

      _self.ip = info.address;
      _self.port = port;
      _self.emit('available', _self.ip, _self.port);

      this.close();
    })

    // Broadcast query
    .send(query, 0, query.length, sybasesrvmonPort, '255.255.255.255');
  }

  /**
   * Sends one of the commands listed in `./commands.json`
   * @param {string} command - The key of the command listed in `./commands.json`
   */
  SharpVirtualRemote.prototype.send = function(command) {
    var _self = this;

    if(typeof commands[command] == 'undefined') throw new Error('Command `' + command + '` not defined');

    if(!this.ip) throw new Error('Can not send command, IP undefined');

    var smaclmgrPort = 4660;

    console.log('Connecting to ' + _self.ip + ':' + smaclmgrPort);

    this.connection = new net.Socket()
    .connect(smaclmgrPort, _self.ip, function() {
      _self.connected = true;
      console.log('Connected to ' + _self.ip + ':' + smaclmgrPort);

      console.log('Sending command `' + command + '`');

      this.write(Buffer.from(commands[command].toString()));

      this.destroy();
    });
  }

/**
 * Command Line Interface
 */
var command = process.argv.splice(2).shift();
if(command) {
  var tv = new SharpVirtualRemote()
  .on('available', function() {
    this.send(command);
  });
}

module.exports = SharpVirtualRemote;

/*

The Smart Remote App also sends an M-SEARCH before sending the query.
Allthough the query seems to be all that is needed to find the TV.

var msearch = 'M-SEARCH * HTTP/1.1\r\n' + 
  'HOST: 239.255.255.250.1900\r\n' + 
  'MAN: "ssdp:discover"\r\n' + 
  'MX: 10\r\n' + 
  'ST: urn:dial-multiscreen-org:service:dial:1\r\n\r\n';

dgram.createSocket('udp4')
.send(msearch, 0, msearch.length, '1900', '239.255.255.250', function(err, bytes) {
  this.close();
});

Another service is exposed at port 1986: licensedaemon

This service sends status messages.

`SETFOLLOWTVDEVICESTATUS1` is send by the remote on connect
`follow_tv_status:1` is the TV's response
tv_portal_status:1 when the web portal is opened
tv_portal_status:0 when the web portal is closed
`tv_status:0` on power off

var licensedaemonPort = 1986;
var licensedaemon = new net.Socket();
.on('message', console.log)
.connect(licensedaemonPort, ip, function() {
  console.log(':' + licensedaemonPort, 'listening');
  
  licensedaemon.write('SETFOLLOWTVDEVICESTATUS 1\n');
   console.log(':1986', 'sent', 'SETFOLLOWTVDEVICESTATUS 1');
});

*/