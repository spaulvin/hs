# sharp-virtual-remote
A Node.JS module that controls a Sharp TV with virtual remote option.

## Usage
```javascript
var tv = new SharpVirtualRemote()
.on('available', function() {
  this.send('menu');
});
```

The module will find the TV on the network by broadcasting a query packet over UDP. The TV responds with some information like the port to be connected to.
When the response from the TV has been received the module emits an `available` event.

A fixed IP and custom port can be given, the port defaults to 4660 so is optional.
```javascript
var tv = new SharpVirtualRemote('192.168.0.111', 1234)
.on('available', function() {
  this.send('menu');
});
```

## Command Line Interface
`node sharp-virtual-remote <command>`

Where `<command>` is one of the commands in [commands.json](./commands.json)

## Setup
[Sharp Smart Remote](https://itunes.apple.com/nl/app/sharp-smart-remote/id618870928?mt=8)

> [Setup]
> The TV and device must be connected to the same Wi-Fi network.
> Enable “Virtual Remote” on the TV (Menu->Settings->Other Settings).
> Start the app and it will search for your TV. Once found, select it and you are in control.
> 
> [Supported Languages]
> This app supports English, Turkish, French, German, Dutch and Danish.
> 
> [Supported Models]
> This app only supports SHARP TV’s (with ‘IP Control’) for the European market.
> 
> 32”:
> LC-32LE350E-BK, LC-32LE350V-BK, LC-32LE351E-BK, LC-32LE351K-BK, LC-32LE352E-BK
> LC-32LE350E-WH, LC-32LE350V-WH, LC-32LE351E-WH, LC-32LE351K-WH, LC-32LE352E-WH
> 
> 39”:
> LC-39LE350E-BK ,LC-39LE350V-BK, LC-39LE351E-BK, LC-39LE351K-BK, LC-39LE352E-BK
> LC-39LE350E-WH, LC-39LE350V-WH, LC-39LE351E-WH, LC-39LE351K-WH, LC-39LE352E-WH
> 
> 42”:
> LC-42LE760E, LC-42LE760EN, LC-42LE761E, LC-42LE761EN, LC-42LE761K, > LC-42LE762E, LC-42LE762EN
> 
> 50”:
> LC-50LE760E, LC-50LE760EN, LC-50LE761E, LC-50LE761EN, LC-50LE761K, LC-50LE762E, LC-50LE762EN